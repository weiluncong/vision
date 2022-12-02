#include "csda_parser.h"

void CSdaParser::ParseHDMap(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("StaticHDMapInfo"))
    {
        return;
    }

    QMap<int, QVector<CMapLine>> data;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);
        if (field->lowercase_name() == "links")
        {
            int val_size = reflection->FieldSize(msg, field);
            for (int j = 0; j < val_size; j++)
            {
                int link_id;
                QVector<CMapLine> lines;
                const auto &link_msg = reflection->GetRepeatedMessage(msg, field, j);
                auto link_reflector = link_msg.GetReflection();
                auto link_descript = link_msg.GetDescriptor();
                int link_count = link_descript->field_count();
                bool is_routing_path = false;
                for (int k = 0; k < link_count; k++)
                {
                    auto field = link_descript->field(k);
                    if (field->lowercase_name() == "lines")
                    {
                        is_routing_path = FieldToQStr(msg, field).toUInt();
                        continue;
                    }

                    if (field->lowercase_name() == "lines")
                    {
                        int val_size = link_reflector->FieldSize(link_msg, field);
                        for (int l = 0; l < val_size; l++)
                        {
                            CMapLine line;
                            line.in_navigation_route_ = is_routing_path;
                            const auto &line_msg = link_reflector->GetRepeatedMessage(link_msg, field, l);
                            auto line_reflector = line_msg.GetReflection();
                            auto line_descript = line_msg.GetDescriptor();
                            int line_count = line_descript->field_count();
                            for (int m = 0; m < line_count; m++)
                            {
                                auto field = line_descript->field(m);
                                if (field->lowercase_name() == "index")
                                {
                                    line.index_ = FieldToQStr(line_msg, field).toInt();
                                }
                                else if (field->lowercase_name() == "line_type")
                                {
                                    line.type_ = FieldToQStr(line_msg, field).toInt();
                                }
                                else if (field->lowercase_name() == "line_points")
                                {
                                    int val_size = line_reflector->FieldSize(line_msg, field);
                                    for (int n = 0; n < val_size; n++)
                                    {
                                        CMapPoint gnss;
                                        const auto &point_msg = line_reflector->GetRepeatedMessage(line_msg, field, n);
                                        gnss = ParserGnss(point_msg);
                                        line.points_.push_back(gnss);
                                    }
                                }
                            }
                            if (!line.points_.isEmpty())
                                lines.push_back(line);
                        }
                    }
                    else if (field->lowercase_name() == "link_id")
                    {
                        link_id = FieldToQStr(link_msg, field).toInt();
                    }
                }
                if (!lines.isEmpty())
                    data[link_id].append(lines);
            }
        }
    }

    data_center_->InsertValue(msg_name, time, data);
    ParseFinished("topview", time);
}

void CSdaParser::ParseIns(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("InsData") || msg_name.toLower().contains("topic") || msg_name.contains("fix"))
        return;

    CMapInsData data;
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    data.gnss_ = ParserGnss(msg);
    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);
        if (field->lowercase_name() == "heading")
        {
            data.heading_ = FieldToQStr(msg, field).toDouble();
        }
    }

    data_center_->InsertValue("InsDataFix", time, data);
    ParseFinished("topview", time);
}

void CSdaParser::ParseIdmapStatic(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("idmap.StaticIDMapInfo"))
    {
        return;
    }

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    QMap<int, QVector<CMapLine>> map_info;
    for (int i = 0; i < field_count; i++)
    {
        auto field = descriptor->field(i);

        /// Message IDMapStatus
        if (field->name() == "idmap_status")
        {
            const auto &map_status_msg = reflection->GetMessage(msg, field);
            auto map_status_reflec = map_status_msg.GetReflection();
            auto map_status_desc = map_status_msg.GetDescriptor();
            int field_count = map_status_desc->field_count();
            for (int j = 0; j < field_count; j++)
            {
                auto field = map_status_desc->field(j);
                if (field->name() == "map_available" && FieldToQStr(map_status_msg, field).toInt() == 0)
                {
                    return;
                }
            }
        }

        /// repeated Lane
        if (field->name() == "lanes")
        {
            int lane_size = reflection->FieldSize(msg, field);
            QVector<CMapLine> lanes;
            for (int j = 0; j < lane_size; j++)
            {
                CMapLine lane;
                lane.type_ = CLineType::LaneLine;
                auto &lane_msg = reflection->GetRepeatedMessage(msg, field, j);
                auto lane_reflector = lane_msg.GetReflection();
                auto lane_descript = lane_msg.GetDescriptor();

                /// lane msg content parser
                int lane_count = lane_descript->field_count();
                for (int k = 0; k < lane_count; k++)
                {
                    auto field = lane_descript->field(k);
                    /// qDebug() << "name:" << QString::fromStdString(field->name());

                    /// 车道id
                    if (field->name() == "lane_id")
                    {
                        lane.index_ = FieldToQStr(lane_msg, field).toULong();
                        continue;
                    }

                    ///<是否匹配导航路线：[/],(0,0,1),[/],(1,0),/，输出
                    ///<说明：默认不匹配，输出0；如果重复的情况在原有的基础上增加1bit
                    if (field->name() == "in_navigation_route")
                    {
                        lane.in_navigation_route_ = FieldToQStr(lane_msg, field).toUInt();
                        continue;
                    }

                    /// 车道中心线形状点:[/],(/,/,/),[50],(/,/),按归一化顺序播发，输出
                    if (field->name() == "lane_points")
                    {
                        int point_size = lane_reflector->FieldSize(lane_msg, field);
                        for (int n = 0; n < point_size; n++)
                        {
                            CMapPoint gnss;
                            const auto &point_msg = lane_reflector->GetRepeatedMessage(lane_msg, field, n);
                            gnss = ParserGnss(point_msg);
                            lane.points_.push_back(gnss);
                        }
                    }
                }

                if (!lane.points_.isEmpty())
                    lanes.push_back(lane);
                else
                    qDebug() << "lane id:" << lane.index_ << " points is Empty!";
            }

            map_info[0].append(lanes);
        }

        /// repeated Line
        if (field->name() == "lines")
        {
            QVector<CMapLine> lines;
            int line_size = reflection->FieldSize(msg, field);
            for (int l = 0; l < line_size; l++)
            {
                CMapLine line;
                const auto &line_msg = reflection->GetRepeatedMessage(msg, field, l);
                auto line_reflector = line_msg.GetReflection();
                auto line_descript = line_msg.GetDescriptor();
                int line_count = line_descript->field_count();
                for (int m = 0; m < line_count; m++)
                {
                    auto field = line_descript->field(m);

                    ///< 车道线id:[/],(0,0,/),[/],(1,0),无索引顺序，输出
                    if (field->name() == "line_id")
                    {
                        line.index_ = FieldToQStr(line_msg, field).toULong();
                        continue;
                    }

                    ///< 车道线类型
                    if (field->name() == "line_type")
                    {
                        line.type_ = GetRepeatedMsg(line_msg, field, 0).toInt();
                        /// FieldToQStr(line_msg, field).toInt();
                        continue;
                    }

                    if (field->name() == "linemarking_type")
                    {
                        line.marking_type_ = FieldToQStr(line_msg, field).toInt();
                        /// FieldToQStr(line_msg, field).toInt();
                        continue;
                    }

                    ///< 车道线轨迹点集合
                    if (field->name() == "line_points")
                    {
                        int val_size = line_reflector->FieldSize(line_msg, field);
                        for (int n = 0; n < val_size; n++)
                        {
                            CMapPoint gnss;
                            const auto &point_msg = line_reflector->GetRepeatedMessage(line_msg, field, n);
                            gnss = ParserGnss(point_msg);
                            line.points_.push_back(gnss);
                        }

                        continue;
                    }
                }

                if (!line.points_.isEmpty())
                    lines.push_back(line);
                else
                    qDebug() << "lane id:" << line.index_ << " points is Empty!";
            }

            map_info[1].append(lines);
        }
    }

    data_center_->InsertValue(msg_name, time, map_info);
    ParseFinished("topview", time);
}

void CSdaParser::ParseRNPEnvOut(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("RNPEnvOut"))
    {
        return;
    }

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);

        ///<重构后的车道线-下游决策规划的ACC模式使用
        /// ReconstructedLanes
        if (field->name() == "reconstructed_lanes_out")
        {
            const auto &lanes_msg = reflection->GetMessage(msg, field);
            auto lanes_reflec = lanes_msg.GetReflection();
            auto lanes_desc = lanes_msg.GetDescriptor();
            int field_count = lanes_desc->field_count();
            for (int j = 0; j < field_count; ++j)
            {
                auto field = lanes_desc->field(j);

                /// repeated Lane
                if (field->name() == "lanes")
                {
                    QVector<CSDAModeLane> lanes;
                    int lane_size = lanes_reflec->FieldSize(lanes_msg, field);
                    for (int l = 0; l < lane_size; ++l)
                    {
                        CSDAModeLane lane;
                        const auto &lane_msg = lanes_reflec->GetRepeatedMessage(lanes_msg, field, l);
                        auto lane_reflector = lane_msg.GetReflection();
                        auto lane_descript = lane_msg.GetDescriptor();
                        int lane_count = lane_descript->field_count();
                        for (int m = 0; m < lane_count; ++m)
                        {
                            auto field = lane_descript->field(m);

                            ///< 车道线轨迹点集合
                            if (field->name() == "centerline_points")
                            {
                                int val_size = lane_reflector->FieldSize(lane_msg, field);
                                for (int n = 0; n < val_size; ++n)
                                {
                                    cav::CPointData point;
                                    const auto &point_msg = lane_reflector->GetRepeatedMessage(lane_msg, field, n);
                                    point = ParserPoint(point_msg);
                                    lane.points_.push_back(point);
                                }

                                continue;
                            }

                            if (field->name() == "relation2ego")
                            {
                                lane.relation2ego_ = FieldToQStr(lane_msg, field).toInt();
                            }
                        }

                        if (!lane.points_.isEmpty())
                            lanes.push_back(lane);
                    }

                    data_center_->InsertValue(msg_name, time, lanes);
                    ParseFinished("topview", time);
                }
            }

            return;
        }
    }
}