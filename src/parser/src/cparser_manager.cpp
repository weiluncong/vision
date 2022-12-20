#include "cparser_manager.h"

CParserManager::CParserManager(QObject *parent)
    : QObject(parent)
{
    proto_pool_ = CProtoPool::GetCProtoPool();
    data_center_ = CDataCenter::GetCDataCenter();
    signal_manager_ = CSignalManager::GetCSignalManager();

    signal_parser_ = QStringList({"adas_udp_receiver-adasmap_diagnosis.AdasMapDiagnosisInfo",
                      "adas_udp_receiver-adasmap_diagnosis.AdasMapHeartBeats",
                      "aeb_com-AebProto.AebResult",
                      "aeb_com-AebProto.AebTriggerAndDiagnostic",
                      "capilot-ProcessStatus-adas_udp_receiver"});

    InitParseFunc();
}

CParserManager::~CParserManager()
{
    for (auto i : parse_pools_)
    {
        i->ShutDown();
        delete i;
    }
    SafeClear(parse_pools_);
    SafeClear(parse_functions_);
    SafeClear(signal_parser_);
}

void CParserManager::InitParseFunc()
{
    camera_parser_ = std::shared_ptr<CameraParser>(new CameraParser());
    swc_parser_ = std::shared_ptr<CSwcParser>(new CSwcParser());
    cobject_parser_ = std::shared_ptr<CObjectParser>(new CObjectParser());
    camline_parser_ = std::shared_ptr<CCamLineParser>(new CCamLineParser());
    cpoint_set_parser_ = std::shared_ptr<CPointSetParser>(new CPointSetParser());
    csda_parser_ = std::shared_ptr<CSdaParser>(new CSdaParser());
    csda_lidar_parser_ = std::shared_ptr<CSDALidarParser>(new CSDALidarParser());
    csda_pediction_parser_ = std::shared_ptr<CSDAPredictionParser>(new CSDAPredictionParser());
    cvision_parser_ = std::shared_ptr<CSdaVisionParser>(new CSdaVisionParser());

    // object parser
    AddParserFun("FusionProto.FusObjects", &CObjectParser::ParseObjects, cobject_parser_);
    AddParserFun("CameraProto.CamObjects", &CObjectParser::ParseObjects, cobject_parser_);
    AddParserFun("RadarProto.RadarObjects", &CObjectParser::ParseObjects, cobject_parser_);

    // line parser
    AddParserFun("CameraProto.CamLines", &CCamLineParser::ParseCamLines, camline_parser_);
    AddParserFun("VpCameraProto.CamLines", &CCamLineParser::ParseVpCamLines, camline_parser_);

    // freespace parser
    AddParserFun("FusionProto.FusFreeSpace", &CPointSetParser::ParseFreeSpace, cpoint_set_parser_);
    AddParserFun("FusionProto.RadarFreeSpace", &CPointSetParser::ParseFreeSpace, cpoint_set_parser_);
    AddParserFun("FusionProto.VisionFreeSpace", &CPointSetParser::ParseFreeSpace, cpoint_set_parser_);
    AddParserFun("CameraProto.CamFreeSpace", &CPointSetParser::ParseFreeSpace, cpoint_set_parser_);

    // lidar parser
    AddParserFun("LidarObjectProto.Objects", &CObjectParser::ParseLidarObjects, cobject_parser_);
    AddParserFun("LidarFreeSpaceProto.FreeSpaceData", &CSDALidarParser::ParseLidarFreeSpace, csda_lidar_parser_);

    // vision parser
    AddParserStructFun("Struct.ParsingImage", &CSdaVisionParser::ParseSemantic, cvision_parser_);
    AddParserFun("VpCameraProto.CamObjects", &CObjectParser::ParseVisionDynamicObjects, cobject_parser_);
    AddParserFun("VpCameraProto.CamFreeSpace", &CSdaVisionParser::ParserVisionFreespace, cvision_parser_);
    AddParserFun("VpCameraProto.CamTsr", &CObjectParser::ParsesVisionStaticObjects, cobject_parser_);

    // sda env parser
    AddParserFun("localization.InsData", &CSdaParser::ParseIns, csda_parser_);
    AddParserFun("hdmap.StaticHDMapInfo", &CSdaParser::ParseHDMap, csda_parser_);
    AddParserFun("idmap.StaticIDMapInfo", &CSdaParser::ParseIdmapStatic, csda_parser_);
    // AddParserFun("prediction.RNPEnvOut", &CSdaParser::ParseRNPEnvOut, csda_parser_);
    AddParserFun("localization.CurrentLaneId", &CSdaParser::ParseCurrentLane, csda_parser_);

    // sda prediction parser
    AddParserFun("prediction.RNPObjectOut", &CSDAPredictionParser::ParsePredictions, csda_pediction_parser_);
    AddParserFun("prediction.RNPObjectDebugOut", &CCamLineParser::ParsePredictObjectDebug, camline_parser_);

    // sda planningpath parser
    AddParserFun("dnp.PlanningPaths", &CCamLineParser::ParsePlanningPath, camline_parser_);
}

void CParserManager::Parser(const QList<cReplayData> &list)
{
    for (auto &data : list)
    {
        HandleMetaData(data.head.timestamp, TOQSTR(data.head.name), data.mdata);
    }
    WaitForFinished();
    emit signal_manager_->SigLoadFinished();
}

void CParserManager::Parser(double timestamp, const std::string &topic_name, const std::string &data)
{
    if (!TOQSTR(topic_name).contains("capilot") && !TOQSTR(topic_name).contains("ProcessStatus") &&
        !TOQSTR(topic_name).contains("topic"))
    {
        HandleMetaData(timestamp, TOQSTR(topic_name), data);
    }
}

void CParserManager::HandleMetaData(double timestamp, const QString &topic_name, const std::string &data)
{
    if (data_center_->data_start_time_ == 0)
    {
        start_time_ = timestamp;
        data_center_->data_start_time_ = start_time_;
    }
    end_time_ = timestamp;
    QString swc_name;
    QString package_msg_name;
    SplitTopicName(topic_name, swc_name, package_msg_name);
    double time = end_time_ - start_time_;

    if (topic_name.isEmpty() || package_msg_name.isEmpty())
    {
        qDebug() << "topic_name =" << topic_name << ", package_msg_name = " << package_msg_name;
        return;
    }

    if (parse_functions_.keys().contains(package_msg_name))
    {
        if (!parse_pools_.contains(swc_name))
        {
            parse_pools_[swc_name] = new CThreadPool(1);
        }

        parse_pools_[swc_name]->submit(std::bind(&CParserManager::ParseMessage, this, TOSTR(topic_name),
                                                 TOSTR(package_msg_name), data, time));
    }
    else if (parse_struct_functions_.keys().contains(package_msg_name))
    {
        if (!parse_pools_.contains(swc_name))
        {
            parse_pools_[swc_name] = new CThreadPool(1);
        }
        parse_pools_[swc_name]->submit(std::bind(&CParserManager::ParseStruct, this, TOSTR(topic_name),
                                                 TOSTR(package_msg_name), data, time));
    }
    else if (topic_name.contains("_usb") || topic_name.contains("-usb") ||
            topic_name.contains("RawData-fc") || topic_name.contains("RawData_fc") ||
            topic_name.contains("RawImage-fc"))
    {
        camera_parser_->ParseCamera(topic_name, data, time);
        return;
    }
    else
        RecodeMsgAndGetSignals(topic_name, TOSTR(package_msg_name), data, time);
}

void CParserManager::SplitTopicName(const QString &topic_name, QString &swc_name, QString &package_msg_name)
{
    QString temp_name;
    int pos = 0;
    swc_name = topic_name.split("-")[0];
    temp_name = topic_name.mid(topic_name.split("-")[0].size() + 1);
    pos = temp_name.indexOf("-");

    if (pos != -1)
    {
        package_msg_name = temp_name.mid(0, pos);
    }
    else
    {
        package_msg_name = temp_name;
    }
}

void CParserManager::ParseMessage(const std::string &topic_name, const std::string &package_msg_name,
                                  const std::string &data, double time)
{
    if (parse_functions_.keys().contains(TOQSTR(package_msg_name)))
    {
        std::shared_ptr<google::protobuf::Message> msg = std::shared_ptr<google::protobuf::Message>(proto_pool_->GetProtoMessage(topic_name, package_msg_name, data));
        if (!msg)
            return;

        parse_functions_[TOQSTR(package_msg_name)](TOQSTR(topic_name), *msg, time);
    }
}

void CParserManager::ParseStruct(const std::string &topic_name, const std::string &package_msg_name,
                                  const std::string &data, double time)
{
    if (parse_struct_functions_.keys().contains(TOQSTR(package_msg_name)))
    {
        parse_struct_functions_[TOQSTR(package_msg_name)](TOQSTR(topic_name), data, time);
    }
}

void CParserManager::WaitForFinished()
{
    QSet<QString> parse_finish;
    while (parse_finish.size() < parse_pools_.size())
    {
        for (auto &i : parse_pools_.keys())
        {
            if (parse_pools_[i]->queue_.Empty())
                parse_finish.insert(i);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    data_center_->data_end_time_ = end_time_;
    parse_finish.clear();
    for (auto i : parse_pools_)
    {
        i->ShutDown();
        delete i;
    }
    SafeClear(parse_pools_);
}

void CParserManager::RecodeMsgAndGetSignals(const QString &topic_name, const std::string &package_msg_name, const std::string &data, double time)
{
    google::protobuf::Message *msg = proto_pool_->GetProtoMessage(TOSTR(topic_name), package_msg_name, data);
    if (!msg)
        return;
    if (FLAGS_v_online)
    {
        //parser online signal
        for(auto siganl_name : on_signal_parser_list_)
        {
            if(siganl_name.contains(topic_name))
            {
                swc_parser_->ParseOnlineSwcData(*msg, siganl_name, time);
            }
        }
    }
    else
    {
        //recode off msg data
        data_center_->InsertValue(topic_name, time, msg);
    }

    //get all signals
    if (!msg_parsed_.contains(topic_name))
    {
        bool flag = false;
        swc_parser_->GetMsgSignalName(topic_name, *msg, flag);
        if (flag)
        {
            msg_parsed_.append(topic_name);
            emit UpdateExplorerBoxModel(data_center_->dat_msg_signal_names_);
        }
    }
}
