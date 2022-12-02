#include "csda_parser.h"

void CSdaParser::ParsePredictions(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("prediction.RNPObjectOut"))
        return;
    QVector<cav::CObjectData> obj_vector;
    QVector<cav::CPredictLine> history_lines_vec;
    QVector<cav::CPredictLine> predict_lines_vec;

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto objs_field = descriptor->FindFieldByName("objects");
    int size = reflection->FieldSize(msg, objs_field);
    for (int i = 0; i < size; i++)
    {
        CObjectData obj;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, objs_field, i);
        auto obj_descript = obj_msg.GetDescriptor();
        auto obj_reflection = obj_msg.GetReflection();
        AssignStruct(obj_msg, obj_descript, obj.timestamp_, "utc");
        AssignStruct(obj_msg, obj_descript, obj.classification_, "class_type");
        AssignStruct(obj_msg, obj_descript, obj.track_id_, "trackid");
        AssignStruct(obj_msg, obj_descript, obj.algorithm_type_, "alg_type");
        {
            auto object_info_field = obj_descript->FindFieldByName("object_info");
            const auto &object_info_msg = obj_reflection->GetMessage(obj_msg, object_info_field);
            auto object_info_descript = object_info_msg.GetDescriptor();
            AssignStruct(object_info_msg, object_info_descript, obj.length_, "length");
            AssignStruct(object_info_msg, object_info_descript, obj.width_, "width");
            AssignStruct(object_info_msg, object_info_descript, obj.sensor_flag_, "detect_sensor");
        }
        {
            auto object_status_field = obj_descript->FindFieldByName("object_status");
            const auto &object_status_msg = obj_reflection->GetMessage(obj_msg, object_status_field);
            auto object_status_descript = object_status_msg.GetDescriptor();
            bool state = false;
            AssignStruct(object_status_msg, object_status_descript, state, "obj_is_moving");
            if (state)
                obj.move_status_ = 1;
            AssignStruct(object_status_msg, object_status_descript, state, "obj_is_oncoming");
            if (state)
                obj.move_status_ = 2;
            AssignStruct(object_status_msg, object_status_descript, state, "obj_is_cross");
            if (state)
                obj.move_status_ = 3;
        }
        {
            auto object_position_field = obj_descript->FindFieldByName("object_position_legacy");
            const auto &object_position_msg = obj_reflection->GetMessage(obj_msg, object_position_field);
            auto obj_position_descript = object_position_msg.GetDescriptor();
            AssignStruct(object_position_msg, obj_position_descript, obj.pos_x_, "posx");
            AssignStruct(object_position_msg, obj_position_descript, obj.pos_y_, "posy");
            AssignStruct(object_position_msg, obj_position_descript, obj.abs_velocity_x_, "velx");
            AssignStruct(object_position_msg, obj_position_descript, obj.abs_velocity_y_, "vely");
            AssignStruct(object_position_msg, obj_position_descript, obj.heading_angle_, "heading_angle_local");
        }
        obj_vector.push_back(obj);

        //历史轨迹解析
        CPredictLine history_line;
        auto history_line_field = obj_descript->FindFieldByName("trajectory_history");
        const auto &history_line_msg = obj_reflection->GetMessage(obj_msg, history_line_field);
        ParserTrajectory(history_line_msg, history_line.predict_line_);
        history_lines_vec.push_back(history_line);

        //预测轨迹解析
        auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodal_prediction");
        const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
        auto predict_lines_multi_descriptor = predict_lines_multi_msg.GetDescriptor();
        auto predict_lines_multi_reflection = predict_lines_multi_msg.GetReflection();
        //轨迹线仅有一条，不做repeate处理
        auto trajector_field = predict_lines_multi_descriptor->FindFieldByName("trajectories");
        const auto &trajector_msg = predict_lines_multi_reflection->GetRepeatedMessage(predict_lines_multi_msg, trajector_field, 0);
        CPredictLine predict_line_multi;
        ParserTrajectory(trajector_msg, predict_line_multi.predict_line_);

        auto prob_field = predict_lines_multi_descriptor->FindFieldByName("probs");
        int size = predict_lines_multi_reflection->FieldSize(predict_lines_multi_msg, prob_field);
        if (size != 0)
            predict_line_multi.prob_ = predict_lines_multi_reflection->GetRepeatedFloat(predict_lines_multi_msg, prob_field, 0);
        predict_lines_vec.push_back(predict_line_multi);
    }
    QString obj_name = msg_name + "-obj";
    data_center_->InsertValue<QVector<cav::CObjectData>>(obj_name, time, obj_vector);
    QString history_name = msg_name + "-history_trajectory";
    data_center_->InsertValue<QVector<cav::CPredictLine>>(history_name, time, history_lines_vec);
    QString predict_name = msg_name + "-predict_trajectory";
    data_center_->InsertValue<QVector<cav::CPredictLine>>(predict_name, time, predict_lines_vec);
    ParseFinished("topview", time);
}

void CSdaParser::ParserTrajectory(const google::protobuf::Message &msg, QVector<cav::CPointData> &line)
{
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto points_field = descriptor->FindFieldByName("state");
    int size = reflection->FieldSize(msg, points_field);
    for (int i = 0; i < size; i++)
    {
        cav::CPointData point;
        const auto &state_msg = reflection->GetRepeatedMessage(msg, points_field, i);
        auto state_descript = state_msg.GetDescriptor();
        auto state_reflection = state_msg.GetReflection();
        auto point_field = state_descript->FindFieldByName("point");
        auto &point_msg = state_reflection->GetMessage(state_msg, point_field);
        point = ParserPoint(point_msg);
        if (ValueCheck(point.x_) && ValueCheck(point.y_))
            line.push_back(point);
    }
}

void CSdaParser::ParsePredictObjectDebug(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("prediction.RNPObjectDebugOut"))
        return;
    QVector<cav::CPredictLine> predictions;

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto objs_field = descriptor->FindFieldByName("objects_debug");
    int size = reflection->FieldSize(msg, objs_field);
    for (int i = 0; i < size; i++)
    {
        int id = 0;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, objs_field, i);
        auto obj_descript = obj_msg.GetDescriptor();
        auto obj_reflection = obj_msg.GetReflection();
        AssignStruct(obj_msg, obj_descript, id, "trackid");
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_rule_veh");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CPredictLine prediction;
            prediction.id_ = id;
            prediction.type_ = cav::CPredictionType::AlgorithmCar;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, prediction);
            if (!prediction.predict_line_.isEmpty())
                predictions.push_back(prediction);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_rule_ped");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CPredictLine prediction;
            prediction.id_ = id;
            prediction.type_ = cav::CPredictionType::AlgorithmPerson;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, prediction);
            if (!prediction.predict_line_.isEmpty())
                predictions.push_back(prediction);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_cslstm");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CPredictLine prediction;
            prediction.id_ = id;
            prediction.type_ = cav::CPredictionType::AlgorithmCslstm;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, prediction);
            if (!prediction.predict_line_.isEmpty())
                predictions.push_back(prediction);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_raster");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CPredictLine prediction;
            prediction.id_ = id;
            prediction.type_ = cav::CPredictionType::AlgorithmRaster;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, prediction);
            if (!prediction.predict_line_.isEmpty())
                predictions.push_back(prediction);
        }
    }
    data_center_->InsertValue<QVector<cav::CPredictLine>>(msg_name, time, predictions);
    ParseFinished("topview", time);
}

void CSdaParser::ParserTrajectoryMultimodal(const google::protobuf::Message &msg, cav::CPredictLine &debug_line)
{
    auto descriptor = msg.GetDescriptor();
    auto reflection = msg.GetReflection();
    //轨迹线仅有一条，不做repeate处理
    auto trajector_field = descriptor->FindFieldByName("trajectories");
    int size = reflection->FieldSize(msg, trajector_field);
    if (size == 0)
        return;
    const auto &trajector_msg = reflection->GetRepeatedMessage(msg, trajector_field, 0);
    QVector<cav::CPointData> predict_line_multi;
    ParserTrajectory(trajector_msg, predict_line_multi);
    debug_line.predict_line_ = predict_line_multi;

    auto prob_field = descriptor->FindFieldByName("probs");
    debug_line.prob_  =  reflection->GetRepeatedFloat(msg, prob_field, 0);
}


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

void CSdaParser::ParseLidarFreeSpace(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("LidarFreeSpaceProto.FreeSpaceData"))
        return;
    QVector<cav::CPointData> data;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);
        if (field->lowercase_name() == "freespace")
        {
            int size = reflection->FieldSize(msg, field);
            for (int i = 0; i < size; ++i)
            {
                const auto &center_msg = reflection->GetRepeatedMessage(msg, field, i);
                cav::CPointData point = ParserPoint(center_msg);
                if (ValueCheck(point.x_) && ValueCheck(point.y_) && ValueCheck(point.z_))
                    data.push_back(point);
            }
        }
    }
    data_center_->InsertValue<QVector<cav::CPointData>>(msg_name, time, data);
    ParseFinished("topview", time);
}

void CSdaParser::ParseLidarObjects(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("LidarObjectProto.Objects"))
        return;
    QVector<cav::CObjectData> obj_vec;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int k = 0; k < field_count; ++k)
    {
        auto field = descriptor->field(k);
        if (field->lowercase_name() == "obj")
        {
            int obj_count = reflection->FieldSize(msg, field);
            for (int n = 0; n < obj_count; n++)
            {
                const auto &obj_msg = reflection->GetRepeatedMessage(msg, field, n);
                auto obj_reflection = obj_msg.GetReflection();
                auto obj_descript = obj_msg.GetDescriptor();
                int obj_count = obj_descript->field_count();
                for (int i = 0; i < obj_count; ++i)
                {
                    auto core_field = obj_descript->field(i);
                    if (core_field->lowercase_name() == "coreinfo")
                    {
                        const auto &core_msg =
                            obj_reflection->GetMessage(obj_msg, core_field);
                        auto core_reflection = core_msg.GetReflection();
                        auto core_descript = core_msg.GetDescriptor();
                        cav::CObjectData obj;
                        AssignStruct(core_msg, core_descript, obj.track_id_, "track_id");
                        int core_count = core_descript->field_count();

                        for (int j = 0; j < core_count; j++)
                        {
                            auto ob_field = core_descript->field(j);
                            if (ob_field->lowercase_name() == "center")
                            {
                                const auto &center_msg = core_reflection->GetMessage(core_msg, ob_field);
                                cav::CPointData point;
                                point = ParserPoint(center_msg);
                                obj.pos_x_ = point.x_;
                                obj.pos_y_ = point.y_;
                            }
                            else if (ob_field->lowercase_name() == "size")
                            {
                                const auto &size_msg = core_reflection->GetMessage(core_msg, ob_field);
                                cav::CPointData point;
                                point = ParserPoint(size_msg);
                                obj.width_ = point.y_;
                                obj.length_ = point.x_;
                            }
                            else if (ob_field->lowercase_name() == "direction")
                            {
                                const auto &direction_msg = core_reflection->GetMessage(core_msg, ob_field);
                                cav::CPointData point;
                                point = ParserPoint(direction_msg);
                                obj.heading_angle_ = point.z_ * 3.14 / 180;
                            }
                            else if (ob_field->lowercase_name() == "velocity")
                            {
                                const auto &velocity_msg = core_reflection->GetMessage(core_msg, ob_field);
                                cav::CPointData point;
                                point = ParserPoint(velocity_msg);
                                obj.abs_velocity_x_ = point.x_;
                                obj.abs_velocity_y_ = point.y_;
                            }
                            else if (ob_field->lowercase_name() == "relative_velocity")
                            {
                                const auto &relative_velocity_msg = core_reflection->GetMessage(core_msg, ob_field);
                                cav::CPointData point;
                                point = ParserPoint(relative_velocity_msg);
                                obj.rel_velocity_x_ = point.x_;
                                obj.rel_velocity_y_ = point.y_;
                            }
                            else if (ob_field->lowercase_name() == "acceleration")
                            {
                                const auto &acceleration_msg = core_reflection->GetMessage(core_msg, ob_field);
                                cav::CPointData point;
                                point = ParserPoint(acceleration_msg);
                                obj.rel_acceleration_x_ = point.x_;
                                obj.rel_acceleration_y_ = point.y_;
                            }
                        }
                        obj_vec.push_back(obj);
                    }
                }
            }
        }
    }
    if (obj_vec.size() <= 0)
        return;
    data_center_->InsertValue<QVector<cav::CObjectData>>(msg_name, time, obj_vec);
    ParseFinished("topview", time);
}

