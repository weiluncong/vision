#include "creceive_data.h"
#include "cexplorer_box.h"

CReceiveData::CReceiveData()
{
    parser_manager_ = new CParserManager();
    proto_pool_ = CProtoPool::GetCProtoPool();
    data_center_ = CDataCenter::GetCDataCenter();
    signal_manager_ = CSignalManager::GetCSignalManager();
    CExplorerBox::GetCExplorerBox()->Clear();
    connect(parser_manager_, &CParserManager::UpdateExplorerBoxModel, CExplorerBox::GetCExplorerBox(), &CExplorerBox::UpdateModelList);
    connect(CExplorerBox::GetCExplorerBox(), &CExplorerBox::CurveSignal, parser_manager_, &CParserManager::AddOnSignal);
    InitReceive();
}

CReceiveData::~CReceiveData()
{
    receive_flag_ = false;
    condition_var_.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (auto i : camera_captures_)
    {
        SAFE_DELETE(i);
    }
    SafeClear(camera_captures_);
    SAFE_DELETE(parser_manager_);
}

/*************************************zmq data receive*********************************/
void CReceiveData::InitReceive()
{
    switch (FLAGS_v_model_setting)
    {
    case 0:
        ReceiveData(FLAGS_v_data_address_pc, FLAGS_v_proto_address_pc);
        // ReceiveCameraData();
        DeliverData();
        break;
    case 1:
        // ReceiveDDSData(dds_topic_map_.keys());
        ReceiveData(FLAGS_v_data_address_soc1, FLAGS_v_proto_address_soc1);
        // ReceiveCanData(ZCAN_CANFDNET_400U_TCP, 4, "400", "192.168.0.179");
        // ReceiveCanData(ZCAN_CANFDDTU_CASCADE_TCP, 10, "600", "192.168.0.178");
        // ReceiveCameraData();
        DeliverData();
        break;
    case 2:
        ReceiveData(FLAGS_v_data_address_soc1, FLAGS_v_proto_address_soc1);
        ReceiveData(FLAGS_v_data_address_soc2, FLAGS_v_proto_address_soc2);
        DeliverData();
        break;
    default:
        break;
    }
}

void CReceiveData::ReceiveData(const std::string &data_address, const std::string &proto_address)
{
    std::thread t([&]
                  {
                    bool proto_flag = true;
                    zmq::message_t msg;
                    zmq::context_t *context = new zmq::context_t();
                    std::shared_ptr<zmq::socket_t> socket = std::make_shared<zmq::socket_t>(*context, zmq::socket_type::sub);
                    socket->set(zmq::sockopt::subscribe, "");
                    socket->connect(data_address);
                    while (receive_flag_)
                    {
                        if (switch_flag_)
                        {
                            if(proto_flag)
                            {
                                proto_pool_->UpdateProtoContent(proto_address);
                                proto_flag = false;
                            }
                            if(FLAGS_v_capilot_version >= 3.2)
                            {
                                socket->recv(msg);
                                this->msgs_queue_.Enqueue(msg.to_string());
                            }
                            else
                            {
                                socket->recv(msg);
                                char topic_name[kTopicNameMaxLen];
                                memset(topic_name, 0, kTopicNameMaxLen);
                                memcpy(topic_name, msg.to_string().c_str(), msg.to_string().size());
                                socket->recv(msg);
                                double timestamp;
                                std::istringstream is(msg.to_string());
                                is >> timestamp;
                                size_t batch = 1;
                                // std::cout << "origin: " << topic_name << "----" << std::fixed << timestamp << std::endl;
                                socket->recv(msg);
                                std::string msg_data = msg.to_string();

                                std::string msg_str;
                                msg_str.resize(kTopicNameMaxLen + kTimestampLen + kBatchLen + msg_data.size());
                                memcpy(&msg_str[0], &topic_name, kTopicNameMaxLen);
                                memcpy(&msg_str[kTopicNameMaxLen], &timestamp, kTimestampLen);
                                memcpy(&msg_str[kTopicNameMaxLen + kTimestampLen], &batch, kBatchLen);
                                memcpy(&msg_str[kTopicNameMaxLen + kTimestampLen + kBatchLen], msg_data.c_str(), msg_data.size());
                                this->msgs_queue_.Enqueue(msg_str);    
                            }
                            this->condition_var_.notify_one();
                        }
                        else
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        }                   
                    }
                    zmq_close(&socket); });
    t.detach();
}

void CReceiveData::DeliverData()
{
    std::thread t([this]
                  {
                      std::string msg;
                      std::string topic_name;
                      double timestamp;
                      size_t batch = 0;
                      std::string msg_data;
                      while (this->receive_flag_)
                      {
                        {
                            std::unique_lock<std::mutex> recv_lock(this->recv_mutex_);
                            if(this->msgs_queue_.Empty())
                                this->condition_var_.wait(recv_lock);
                        }
                        if(this->msgs_queue_.Dequeue(msg))
                        {
                            /** @brief parse data*/
                            this->SplitRecvData(msg.data(), msg.size(), topic_name, timestamp, batch, msg_data);
                            if(FLAGS_v_parser)
                            {
                                // std::cout << "split: " << topic_name << ">>>" << std::fixed  << timestamp << ">>>" << batch << std::endl;
                                this->parser_manager_->Parser(timestamp, topic_name, msg_data);
                            }
                            /** @brief save data for record*/
                            this->RecordData(topic_name, timestamp, msg_data);
                        }
                      } });
    t.detach();
}

void CReceiveData::SplitRecvData(const char *data, size_t size,
                                 std::string &topic_name, double &timestamp,
                                 size_t &batch, std::string &msg_data)
{
    assert(size > kTopicNameMaxLen + kTimestampLen);

    /// get topic name
    char topic_name_array[kTopicNameMaxLen];
    memcpy(topic_name_array, data, kTopicNameMaxLen);
    topic_name = topic_name_array;
    // int pos = topic_name.find_first_of("-");
    // if (pos != topic_name.npos)
    // {
    //     int length = topic_name.length() - pos;
    //     topic_name = topic_name.substr(pos + 1, length);
    // }
    /// get timestamp
    memcpy(&timestamp, &data[kTopicNameMaxLen], kTimestampLen);
    if (timestamp_gap_ == 0)
        timestamp_gap_ = KTime().getTime() - timestamp;
    timestamp += timestamp_gap_;
    /// get batch
    memcpy(&batch, &data[kTopicNameMaxLen + kTimestampLen], kBatchLen);
    /// get msg data
    auto ptr = &data[kTopicNameMaxLen + kTimestampLen + kBatchLen];
    size_t len = size - kTopicNameMaxLen - kTimestampLen - kBatchLen;
    msg_data = std::string(ptr, len);
}

void CReceiveData::RecordData(const std::string &topic_name, double timestamp, const std::string &msg_data)
{
    if (FLAGS_v_total_record || FLAGS_v_point_record)
    {
        if (!FLAGS_v_lidar_record && lidar_list_.contains(TOQSTR(topic_name)))
        {
            return;
        }
        RawData rdata(topic_name, timestamp, msg_data);
        data_center_->InsertRecordData(timestamp, rdata.rawData());
        if (FLAGS_v_point_record)
        {
            emit this->signal_manager_->SigUpdateDataBufSize();
            if (this->data_center_->data_buf_.size() >= FLAGS_v_point_record_size)
            {
                auto it = this->data_center_->data_buf_.begin();
                this->data_center_->data_buf_.erase(it);
            }
        }
    }
}

/***************************************camera data receive*********************/
void CReceiveData::ReceiveCameraData()
{
    if (CreateCameraCapture())
    {
        qDebug() << "create camera capture failed, no usb camera!!";
        return;
    }
    std::thread t([=]
                  {
        while (receive_flag_)
        {
            if (switch_flag_)
            {
                for (size_t i = 0; i < camera_captures_.size(); i++)
                {
                    cv::Mat m;
                    *camera_captures_.at(i) >> m;
                    if (!m.empty())
                    {
                        double timestamp = KTime().getTime();
                        std::vector<unsigned char> data;
                        cv::imencode(".jpg", m, data, {cv::IMWRITE_JPEG_QUALITY, 30});
                        std::string raw_data;
                        raw_data.resize(data.size());
                        memcpy(&raw_data[0], &data[0], data.size());
                        std::string topic_name = "Idraw-Struct.CameraData-usb%1" + std::to_string(i);
                        if(FLAGS_v_parser)
                        {
                            parser_manager_->Parser(timestamp, topic_name, raw_data);
                        }
                        /** @brief save data for record*/
                        this->RecordData(topic_name, timestamp, raw_data);
                    }
                    else
                    {
                        qDebug() << "usb camera " << i << " no data to receive";
                    }
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        } });
    t.detach();
}

bool CReceiveData::CreateCameraCapture()
{
    for (auto i : camera_captures_)
    {
        delete i;
    }
    SafeClear(camera_captures_);
    for (int i = 0; i < FLAGS_v_camera_num; ++i)
    {
        cv::VideoCapture *capture = new cv::VideoCapture(i);
        if (!capture->isOpened())
        {
            qDebug() << "camera open failed, camera num =" << i;
            continue;
        }
        capture->set(cv::CAP_PROP_FPS, 15);
        camera_captures_.push_back(capture);
    }

    return camera_captures_.empty();
}

/***************************************DDS data receive*********************/
void CReceiveData::ReceiveDDSData(const QStringList &topics, const std::string &config_path)
{
    if (participant_ != 0)
        return;
    if (!config_path.empty())
    {
        ddsrt_unsetenv("AUTOCOREDDS_URI");
        ddsrt_setenv("AUTOCOREDDS_URI", config_path.data());
    }

    /** @brief 创建participant，domain_id_为0*/
    participant_ = dds_create_participant(domain_id_, NULL, NULL);
    if (participant_ < 0)
    {
        std::cout << "dds创建Participant失败, 错误: " << dds_strretcode(-participant_) << std::endl;
        return;
    }
    for (auto src_topic : topics)
    {
        dds_entity_t topic;
        dds_entity_t reader;
        dds_listener_t *listener = nullptr;

        if (!CreateDDSListener(&listener))
        {
            continue;
        }

        if (!CreateDDSTopic(participant_, TOSTR(src_topic), &topic))
        {
            dds_delete_listener(listener);
            continue;
        }

        if (!CreateDDSReader(participant_, topic, &reader, listener))
        {
            dds_delete_listener(listener);
            continue;
        }
    }
}

void CReceiveData::DDSDataCallBack(dds_entity_t reader, void *arg)
{
    CReceiveData *this_ptr = static_cast<CReceiveData *>(arg);
    if (nullptr == this_ptr)
    {
        qDebug() << "lister call back arg is nullptr";
        return;
    }

    if (!this_ptr->switch_flag_)
        return;

    int i = 0;
    void *ptrs[MAX_SAMPLES] = {0};
    dds_sample_info_t info[MAX_SAMPLES];
    DDSData_Msg *msg;
    std::string data;
    int n;
    n = dds_take(reader, ptrs, info, MAX_SAMPLES, MAX_SAMPLES); // 从队列中获取数据

    for (i = 0; i < n; i++)
    {
        if (info[i].valid_data)
        {
            msg = (DDSData_Msg *)ptrs[i]; // 数据已获取
            // std::cout << "=== [Subscriber] Received : " << std::endl;
            // std::cout << "topic:" << msg->topic << std::endl;
            // std::cout << "time:" << std::fixed << msg->timestamp << std::endl;
            // std::cout << "data max size:" << msg->payload._maximum << std::endl;
            // std::cout << "data size:" << msg->payload._length << std::endl;
            // std::cout << "Message:" << msg->payload._buffer << std::endl
            //           << std::endl;
            this_ptr->DDSMsgToString(msg);
        }
    }

    dds_return_loan(reader, ptrs, n);
}

bool CReceiveData::CreateDDSListener(dds_listener_t **listener)
{
    if (listener == nullptr)
        return false;

    *listener = dds_create_listener(this);
    if ((*listener) == nullptr)
        return false;

    dds_lset_data_available(*listener, DDSDataCallBack);
    return true;
}

bool CReceiveData::CreateDDSTopic(dds_entity_t participant, const std::string &topic, dds_entity_t *res_topic)
{
    if (res_topic == nullptr)
        return false;

    *res_topic = dds_create_topic(participant, &DDSData_Msg_desc,
                                  topic.c_str(), NULL, NULL);
    if (*res_topic >= 0)
        return true;

    std::cout << "dds_create_topic:" << dds_strretcode(-*res_topic) << std::endl;
    return false;
}

bool CReceiveData::CreateDDSReader(dds_entity_t participant, dds_entity_t dds_topic,
                                   dds_entity_t *res_reader, dds_listener_t *listener)
{
    if (res_reader == nullptr)
        return false;

    dds_qos_t *qos = dds_create_qos();
    dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_MSECS(200));
    dds_qset_history(qos, DDS_HISTORY_KEEP_ALL, 5);
    *res_reader = dds_create_reader(participant, dds_topic, qos, listener);
    if (*res_reader >= 0)
    {
        dds_delete_qos(qos);
        return true;
    }

    std::cout << "dds_create_reader:" << dds_strretcode(-*res_reader) << std::endl;
    return false;
}

QString CReceiveData::ConvertDDSTopic(const QString &dds_topic)
{
    if (!dds_topic_map_.contains(dds_topic))
        return dds_topic;

    if (dds_topic_map_[dds_topic].isEmpty())
    {
        QString deal_src_topic = dds_topic;
        deal_src_topic = deal_src_topic.replace("____", "-").replace("__", ".");
        dds_topic_map_[dds_topic] = deal_src_topic;
    }

    return dds_topic_map_[dds_topic];
}

void CReceiveData::DDSMsgToString(DDSData_Msg *msg)
{
    std::string desc_data;
    desc_data.resize(kTopicNameMaxLen + kTimestampLen + kBatchLen + msg->payload._length);
    char topic_name[kTopicNameMaxLen];
    memset(topic_name, 0, kTopicNameMaxLen);
    QString temp_topic = ConvertDDSTopic(msg->topic);
    memcpy(&topic_name, TOSTR(temp_topic).data(), temp_topic.size());

    memcpy(&desc_data[0], &topic_name, kTopicNameMaxLen);
    memcpy(&desc_data[kTopicNameMaxLen], &msg->timestamp, kTimestampLen);

    size_t batch = 1;
    memcpy(&desc_data[kTopicNameMaxLen + kTimestampLen], &batch, kBatchLen);

    memcpy(&desc_data[kTopicNameMaxLen + kTimestampLen + kBatchLen],
           msg->payload._buffer, msg->payload._length);
    msgs_queue_.Enqueue(desc_data);
    condition_var_.notify_one();
}

/***************************************can data receive*********************/
void CReceiveData::ReceiveCanData(uint zcan_type, int chnnel_num, const std::string &name, char *ip_address)
{
    zcan_type_ = zcan_type;
    h_Dev_ = ZCAN_OpenDevice(zcan_type_, 0, 0);
    if (h_Dev_ == INVALID_DEVICE_HANDLE)
    {
        qDebug() << "Open device failed!";
        return;
    }
    CHANNEL_HANDLE hChannel[chnnel_num];
    for (int i = 0; i < chnnel_num; i++)
    {
        hChannel[i] = ZCAN_InitCAN(h_Dev_, i, nullptr);
        if (hChannel[i] == INVALID_CHANNEL_HANDLE)
        {
            qDebug() << "Init CAN failed!";
            return;
        }
    }
    qDebug() << "as tcp client";
    uint val = 1;
    ZCAN_SetReference(zcan_type_, 0, 0, SETREF_SET_DATA_RECV_MERGE, &val);
    val = 0;
    ZCAN_SetReference(zcan_type_, 0, 0, CMD_TCP_TYPE, &val);
    ZCAN_SetReference(zcan_type_, 0, 0, CMD_DESIP, (void *)ip_address);
    val = CANINTERFACE;
    ZCAN_SetReference(zcan_type_, 0, 0, CMD_DESPORT, &val);

    for (int i = 0; i < chnnel_num; i++)
    {
        if (ZCAN_StartCAN(hChannel[i]) != STATUS_OK)
        {
            qDebug() << "Start CAN error!";
            return;
        }
    }
    can_topic_name_ = "RawDataReceiver-ReinjectionData.CanData-" + name + "_";

    std::thread t([&]
                  {
        std::cout << can_topic_name_ << " recever thread start!" << std::endl;
        ZCANDataObj dataBuf[100];
        memset(dataBuf, 0, sizeof(dataBuf));
        while (receive_flag_)
        {
            if (!switch_flag_)
            {
                continue;
            }
            uint recv = ZCAN_ReceiveData(h_Dev_, dataBuf, sizeof(dataBuf) / sizeof(dataBuf[0]));
            if (recv == 0)
            {
                qDebug() << "receive can data len = 0 ;";
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            else
            {
                for (uint i = 0; i < recv; i++)
                {
                    ZCANDataObj data = dataBuf[i];
                    if (data.dataType == ZCAN_DT_ZCAN_CAN_CANFD_DATA)
                    {
                        ZCANCANFDData &can = data.data.zcanCANFDData;
                        if (can.frame.can_id != 0)
                        {
                            double timestamp = KTime().getTime();
                            std::string topic_name = can_topic_name_ + std::to_string((int)data.chnl);
                            qDebug() << "can topic_name =" << TOQSTR(topic_name) << ", timestamp =" << timestamp << ", Can.frame.can_id =" << can.frame.can_id;
                            std::string msg_data;
                            msg_data.resize(sizeof(data));
                            memcpy(&msg_data[0], &data, sizeof(data));
                            RecordData(topic_name, timestamp, msg_data);
                        }
                        else
                        {
                            qDebug() << "receive id is 0!";
                        }
                    }
                }
            }
        }
        ZCAN_CloseDevice(h_Dev_);
        std::cout << can_topic_name_ << " recever thread over!" << std::endl; });
    t.detach();
}