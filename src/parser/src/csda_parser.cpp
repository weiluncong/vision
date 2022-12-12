#include "csda_parser.h"

void CSdaParser::ParseHDMap(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("StaticHDMapInfo"))
    {
        return;
    }

    QMap<int, QVector<CLineData>> data;
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
                int link_id = 0;
                QVector<CLineData> line_vec;
                const auto &link_msg = reflection->GetRepeatedMessage(msg, field, j);
                auto link_reflector = link_msg.GetReflection();
                auto link_descript = link_msg.GetDescriptor();
                AssignStruct(link_msg, link_descript, link_id, "link_id");
                int link_count = link_descript->field_count();
                for (int k = 0; k < link_count; k++)
                {
                    auto field = link_descript->field(k);
                    if (field->lowercase_name() == "lines")
                    {
                        int val_size = link_reflector->FieldSize(link_msg, field);
                        for (int l = 0; l < val_size; l++)
                        {
                            CLineData line;
                            const auto &line_msg = link_reflector->GetRepeatedMessage(link_msg, field, l);
                            auto line_reflector = line_msg.GetReflection();
                            auto line_descript = line_msg.GetDescriptor();
                            AssignStruct(line_msg, line_descript, line.id_, "index");
                            AssignStruct(line_msg, line_descript, line.lane_type_, "line_type");
                            int line_count = line_descript->field_count();
                            for (int m = 0; m < line_count; m++)
                            {
                                auto field = line_descript->field(m);
                                if (field->lowercase_name() == "line_points")
                                {
                                    int val_size = line_reflector->FieldSize(line_msg, field);
                                    for (int n = 0; n < val_size; n++)
                                    {
                                        CPointData point;
                                        const auto &point_msg = line_reflector->GetRepeatedMessage(line_msg, field, n);
                                        point = ParserGnss(point_msg);
                                        line.points_.push_back(point);
                                    }
                                    continue;
                                }
                            }
                            line_vec.push_back(line);
                        }
                    }
                }
                data[link_id].append(line_vec);
            }
        }
    }

    data_center_->InsertValue(package_msg_name, time, data);
    ParseFinished("topview", time);
}

void CSdaParser::ParseIns(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("InsData") || package_msg_name.contains("fix"))
        return;

    CPointData point;
    auto descriptor = msg.GetDescriptor();
    point = ParserGnss(msg);
    AssignStruct(msg, descriptor, point.heading_, "heading");

    data_center_->InsertValue("InsDataFix", time, point);
    ParseFinished("topview", time);
}

void CSdaParser::ParseIdmapStatic(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("idmap.StaticIDMapInfo"))
    {
        return;
    }

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    QVector<CLineData> navigation_line_vec;
    QMap<QString, QVector<CLineData>> map_info;
    for (int i = 0; i < field_count; i++)
    {
        auto field = descriptor->field(i);

        /// Message IDMapStatus
        if (field->name() == "idmap_status")
        {
            const auto &map_status_msg = reflection->GetMessage(msg, field);
            auto map_status_desc = map_status_msg.GetDescriptor();

            int map_available_value = 0;
            AssignStruct(map_status_msg, map_status_desc, map_available_value, "map_available");
            if (map_available_value == 0)
                return;
        }

        /// repeated Lane
        if (field->name() == "lanes")
        {
            int lane_size = reflection->FieldSize(msg, field);
            for (int j = 0; j < lane_size; j++)
            {
                CLineData line;
                auto &lane_msg = reflection->GetRepeatedMessage(msg, field, j);
                auto lane_reflector = lane_msg.GetReflection();
                auto lane_descript = lane_msg.GetDescriptor();
                // 车道id
                AssignStruct(lane_msg, lane_descript, line.id_, "lane_id");
                ///<是否匹配导航路线：[/],(0,0,1),[/],(1,0),/，输出
                ///<说明：默认不匹配，输出0；如果重复的情况在原有的基础上增加1bit
                AssignStruct(lane_msg, lane_descript, line.navigation_status_, "in_navigation_route");

                /// lane msg content parser
                int lane_count = lane_descript->field_count();
                for (int k = 0; k < lane_count; k++)
                {
                    auto field = lane_descript->field(k);
                    /// 车道中心线形状点:[/],(/,/,/),[50],(/,/),按归一化顺序播发，输出
                    if (field->name() == "lane_points")
                    {
                        int point_size = lane_reflector->FieldSize(lane_msg, field);
                        for (int n = 0; n < point_size; n++)
                        {
                            CPointData point;
                            const auto &point_msg = lane_reflector->GetRepeatedMessage(lane_msg, field, n);
                            point = ParserGnss(point_msg);
                            line.points_.push_back(point);
                        }
                    }
                }
                if (line.navigation_status_)
                {
                    line.type_ = CLineData::SolidLine;
                    navigation_line_vec.append(line);
                }
                else
                {
                    line.type_ = CLineData::QtDashLine;
                    map_info["Lane"].append(line);
                }
            }
        }

        /// repeated Line
        if (field->name() == "lines")
        {
            int line_size = reflection->FieldSize(msg, field);
            for (int l = 0; l < line_size; l++)
            {
                CLineData line;
                const auto &line_msg = reflection->GetRepeatedMessage(msg, field, l);
                auto line_reflector = line_msg.GetReflection();
                auto line_descript = line_msg.GetDescriptor();

                ///< 车道线id:[/],(0,0,/),[/],(1,0),无索引顺序，输出
                AssignStruct(line_msg, line_descript, line.id_, "line_id");
                // AssignStruct(line_msg, line_descript, line.type_, "linemarking_type");
                line.type_ = CLineData::SolidLine;
                int line_count = line_descript->field_count();
                for (int m = 0; m < line_count; m++)
                {
                    auto field = line_descript->field(m);

                    ///< 车道线类型
                    if (field->name() == "line_type")
                    {
                        line.lane_type_ = GetRepeatedMsg(line_msg, field, 0).toInt();
                        continue;
                    }
                    ///< 车道线轨迹点集合
                    if (field->name() == "line_points")
                    {
                        int val_size = line_reflector->FieldSize(line_msg, field);
                        for (int n = 0; n < val_size; n++)
                        {
                            CPointData point;
                            const auto &point_msg = line_reflector->GetRepeatedMessage(line_msg, field, n);
                            point = ParserGnss(point_msg);
                            line.points_.push_back(point);
                        }
                        continue;
                    }
                }
                map_info["Line"].append(line);
            }
        }
    }

    data_center_->InsertValue(package_msg_name + "[Navigation]", time, navigation_line_vec);
    for (auto it = map_info.begin(); it != map_info.end(); it++)
    {
        data_center_->InsertValue(package_msg_name + "[" + it.key() + "]", time, it.value());
    }
    ParseFinished("topview", time);
}

void CSdaParser::ParseRNPEnvOut(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("RNPEnvOut"))
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
                    QMap<int, QVector<CLineData>> map;
                    int lane_size = lanes_reflec->FieldSize(lanes_msg, field);
                    for (int l = 0; l < lane_size; ++l)
                    {
                        CLineData line;
                        line.type_ = CLineData::SolidLine;
                        const auto &lane_msg = lanes_reflec->GetRepeatedMessage(lanes_msg, field, l);
                        auto lane_reflector = lane_msg.GetReflection();
                        auto lane_descript = lane_msg.GetDescriptor();
                        AssignStruct(lane_msg, lane_descript, line.relation_to_ego_, "relation2ego");

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
                                    CPointData point;
                                    const auto &point_msg = lane_reflector->GetRepeatedMessage(lane_msg, field, n);
                                    point = ParserPoint(point_msg);
                                    line.points_.push_back(point);
                                }
                                continue;
                            }
                        }
                        map[line.relation_to_ego_].push_back(line);
                    }

                    for (auto it = map.begin(); it != map.end(); it++)
                    {
                        data_center_->InsertValue(package_msg_name + "[" + QString::number(it.key()) + "]", time, it.value());
                    }
                    ParseFinished("topview", time);
                    break;
                }
            }
            break;
        }
    }
}

void CSdaParser::ParseCurrentLane(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("CurrentLaneId"))
    {
        return;
    }

    /// auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);
        if (field->name() != "current_lane_id")
            continue;

        uint64_t current_lane_id;
        AssignStruct(msg, descriptor, current_lane_id, "current_lane_id");
        data_center_->InsertValue("CurrentLaneId", time, current_lane_id);
    }
}
