#include "csda_vision_parser.h"

CSdaVisionParser::CSdaVisionParser()
{
    rle_decode_ = new cav::CRLECompressedImage();
    vision_decode_ = new cav::CapilotParsingFrame();
    full_num_ = 0;
}

CSdaVisionParser::~CSdaVisionParser()
{
    SAFE_DELETE(rle_decode_);
    SAFE_DELETE(vision_decode_);
}

void CSdaVisionParser::ParseSemantic(const QString &package_msg_name, const std::string &data, double time)
{
    if (!package_msg_name.contains("ParsingImage-fc"))
        return;

    online_parsing_img_flag = !online_parsing_img_flag;
    if (FLAGS_v_online && !online_parsing_img_flag)
    {
        return;
    }
    if (!vision_decode_->decode_frame((uint8_t *)data.data(), data.size()))
    {
        qDebug() << "pasingImage raw_data pasing error!";
        return;
    }

    std::vector<uchar> sematic_data, lane_semantic_data;
    int length = IMAGE_SIZE;
    sematic_data.resize(length);
    lane_semantic_data.resize(length);
    full_num_++;

    std::thread *t1 = nullptr;
    if (vision_decode_->parsing_size() > 0)
    {
        if (!rle_decode_->Decompress((const uint8_t *)vision_decode_->parsing_data(),
                                     vision_decode_->parsing_size(),
                                     sematic_data.data(), length))
        {
            QString name = package_msg_name + "_panorama_semantic";
            t1 = new std::thread(&CSdaVisionParser::RLEDecode, this, std::ref(sematic_data), name, time, full_num_);
        }
        else
            qDebug() << "pasingImage sematic_data pasing error!";
    }

    if (vision_decode_->lane_parsing_size() > 0)
    {
        if (!rle_decode_->Decompress((const uint8_t *)vision_decode_->lane_parsing_data(),
                                     vision_decode_->lane_parsing_size(),
                                     lane_semantic_data.data(), length))
        {
            QString name = package_msg_name + "_lane_semantic";
            RLEDecode(lane_semantic_data, name, time, full_num_);
        }
        else
            qDebug() << "pasingImage lane_semantic_image pasing error!";
    }
    if (t1 != nullptr && t1->joinable())
    {
        t1->join();
        delete (t1);
    }
}

void CSdaVisionParser::RLEDecode(const std::vector<uchar> &data, const QString &name, double time, int id)
{
    if (data.size() < IMAGE_SIZE)
        return;
    cav::CImageData semantic_image;
    semantic_image.frame_id_ = id;
    std::vector<unsigned char> out_put;
    cav::CPointData color;
    out_put.resize(IMAGE_SIZE * 3);
    for (int i = 0; i < IMAGE_SIZE; i++)
    {
        color = GetCVColor(data[i]);
        out_put[i * 3] = color.x_;
        out_put[i * 3 + 1] = color.y_;
        out_put[i * 3 + 2] = color.z_;
    }
    cv::Mat src = cv::Mat(256, 480, CV_8UC3, out_put.data(), 0);
    cv::Mat rgb;
    cv::cvtColor(src, rgb, CV_BGR2RGB);
    std::vector<unsigned char> data_encode;
    cv::imencode(".jpg", rgb, data_encode, {cv::IMWRITE_JPEG_QUALITY, 30});
    semantic_image.img_ = data_encode;

    data_center_->InsertValue(name, time, semantic_image);
    ParseFinished("cvision", time);
}

cav::CPointData CSdaVisionParser::GetCVColor(int type)
{
    cav::CPointData color;
    if (type == cav::CRLEType::Road)
        color = cav::CPointData(128, 64, 128);
    else if (type == cav::CRLEType::Sidewalk)
        color = cav::CPointData(70, 70, 70);
    else if (type == cav::CRLEType::Person)
        color = cav::CPointData(220, 20, 60);
    else if (type == cav::CRLEType::Car)
        color = cav::CPointData(0, 0, 142);
    else if (type == cav::CRLEType::Motareycle)
        color = cav::CPointData(0, 0, 70);
    else if (type == cav::CRLEType::Trafficlane)
        color = cav::CPointData(200, 200, 200);
    else if (type == cav::CRLEType::Guideline)
        color = cav::CPointData(0, 192, 192);
    else if (type == cav::CRLEType::Stopline)
        color = cav::CPointData(255, 0, 0);
    else if (type == cav::CRLEType::Fencerle)
        color = cav::CPointData(190, 153, 153);
    else if (type == cav::CRLEType::Crosswalkline)
        color = cav::CPointData(64, 192, 0);
    else if (type == cav::CRLEType::Signline)
        color = cav::CPointData(200, 200, 128);
    else if (type == cav::CRLEType::Trafficcone)
        color = cav::CPointData(0, 64, 64);
    else if (type == cav::CRLEType::Pole)
        color = cav::CPointData(153, 153, 153);
    else if (type == cav::CRLEType::Trafficlight)
        color = cav::CPointData(250, 170, 30);
    else if (type == cav::CRLEType::Traffic_arrow)
        color = cav::CPointData(128, 0, 192);
    else if (type == cav::CRLEType::Speed_bump)
        color = cav::CPointData(220, 220, 0);
    return color;
}

void CSdaVisionParser::ParserVisionFreespace(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("VpCameraProto.CamFreeSpace"))
    {
        return;
    }
    QVector<CPointData> point_vector;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto points_field = descriptor->FindFieldByName("points");
    if (!points_field)
        return;
    int size = reflection->FieldSize(msg, points_field);
    for (int n = 0; n < size; ++n)
    {
        CPointData point;
        const auto &nmsg = reflection->GetRepeatedMessage(msg, points_field, n);
        auto fs_reflect = nmsg.GetReflection();
        auto fs_descript = nmsg.GetDescriptor();
        auto position_field = fs_descript->FindFieldByName("position");
        const auto &pmsg = fs_reflect->GetMessage(nmsg, position_field);
        auto position_descript = pmsg.GetDescriptor();
        AssignStruct(pmsg, position_descript, point.x_, "longitudinal_distance");
        AssignStruct(pmsg, position_descript, point.y_, "lateral_distance");
        AssignStruct(pmsg, position_descript, point.z_, "vertical_distance");
        point_vector.push_back(point);
    }
    data_center_->InsertValue<QVector<CPointData>>(package_msg_name, time, point_vector);
    ParseFinished("topview", time);
}

void CSdaVisionParser::ParserVisionLane(const google::protobuf::Message &msg, CSDALineCVBlocks &lines)
{
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; i++)
    {
        auto field = descriptor->field(i);
        auto &field_name = field->lowercase_name();
        if (field_name == "lines")
        {
            int val_size = reflection->FieldSize(msg, field);
            for (int j = 0; j < val_size; j++)
            {
                auto &line_msg = reflection->GetRepeatedMessage(msg, field, j);
                auto reflection = line_msg.GetReflection();
                auto descriptor = line_msg.GetDescriptor();
                int field_count = descriptor->field_count();
                CSDALineCV line;
                for (int k = 0; k < field_count; k++)
                {
                    auto field = descriptor->field(k);
                    auto &field_name = field->lowercase_name();

                    if (field_name == "contours")
                    {
                        int val_size = reflection->FieldSize(line_msg, field);
                        for (int l = 0; l < val_size; l++)
                        {
                            if (l > 0)
                                continue; // 第二个为VLC坐标，不做取用
                            auto &contour_msg = reflection->GetRepeatedMessage(line_msg, field, l);
                            line.points_ = ParserContour(contour_msg);
                        }
                    }
                    else if (field_name == "type")
                    {
                        int type = FieldToQStr(line_msg, field).toInt();
                        if (type & 65536) // 65536 边界线判断
                            line.type_ = CLineType::CurbLine;
                        else
                            line.type_ = CLineType::LaneLine;
                    }
                }
                if (!line.points_.isEmpty())
                    lines.push_back(line);
            }
        }
    }
}

QVector<cv::Point> CSdaVisionParser::ParserContour(const google::protobuf::Message &msg)
{
    QVector<cv::Point> points;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; i++)
    {
        auto field = descriptor->field(i);
        auto &field_name = field->lowercase_name();
        if (field_name == "pts")
        {
            int val_size = reflection->FieldSize(msg, field);
            for (int j = 0; j < val_size; j++)
            {
                cav::CPointData point;
                auto &point_msg = reflection->GetRepeatedMessage(msg, field, j);
                point = ParserPoint(point_msg);
                cv::Point cvPoint(point.x_, point.y_);
                points.push_back(cvPoint);
            }
        }
    }
    return points;
}

void CSdaVisionParser::ParseVisionLine(const google::protobuf::Message &msg, CSDALineRleBlocks &blocks)
{
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; i++)
    {
        auto field = descriptor->field(i);
        const auto &field_name = field->lowercase_name();
        if (field_name == "lines")
        {
            int val_size = reflection->FieldSize(msg, field);
            for (int j = 0; j < val_size; j++)
            {
                auto &line_msg = reflection->GetRepeatedMessage(msg, field, j);
                auto reflection = line_msg.GetReflection();
                auto descriptor = line_msg.GetDescriptor();
                int field_count = descriptor->field_count();
                CSDALineRle block;
                for (int k = 0; k < field_count; k++)
                {
                    auto field = descriptor->field(k);
                    auto &field_name = field->lowercase_name();
                    if (field_name == "parsing_rles")
                    {
                        QVector<CSDARLE> rles;
                        int val_size = reflection->FieldSize(line_msg, field);
                        for (int l = 0; l < val_size; l++)
                        {
                            auto &rle_msg = reflection->GetRepeatedMessage(line_msg, field, l);
                            auto rle = ParserRLE(rle_msg);
                            if (ValueCheck(rle.row_) && ValueCheck(rle.start_col_) &&
                                ValueCheck(rle.end_col_))
                                rles.push_back(rle);
                        }
                        if (!rles.isEmpty())
                        {
                            block.line_ = rles;
                        }
                    }
                    else if (field_name == "type")
                    {
                        int type = FieldToQStr(line_msg, field).toInt();
                        if (type == 0)
                            block.type_ = CLineType::Unknown;
                        else if (type == 2)
                            block.type_ = CLineType::LaneLine;
                        else if (type == 4)
                            block.type_ = CLineType::CurbLine;
                        else if (type == 8)
                            block.type_ = CLineType::CenterLine;
                        else if (type == 16)
                            block.type_ = CLineType::GuardBar;
                        else if (type == 32)
                            block.type_ = CLineType::ConcreteBarrier;
                        else if (type == 64)
                            block.type_ = CLineType::Fence;
                        else if (type == 128)
                            block.type_ = CLineType::Wall;
                    }
                }
                if (!block.line_.isEmpty())
                    blocks.push_back(block);
            }
        }
    }
}

CSDARLE CSdaVisionParser::ParserRLE(const google::protobuf::Message &msg)
{
    CSDARLE rle;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int k = 0; k < field_count; k++)
    {
        auto field = descriptor->field(k);
        auto &field_name = field->lowercase_name();
        if (field_name == "y")
        {
            rle.row_ = FieldToQStr(msg, field).toInt();
        }
        else if (field_name == "xstart")
        {
            rle.start_col_ = FieldToQStr(msg, field).toInt();
        }
        else if (field_name == "xend")
        {
            rle.end_col_ = FieldToQStr(msg, field).toInt();
        }
    }
    return rle;
}
