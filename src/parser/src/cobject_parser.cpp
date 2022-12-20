#include "cobject_parser.h"

void CObjectParser::ParseObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (package_msg_name.contains("LidarObjectProto") || !package_msg_name.contains("Objects") || package_msg_name.contains("Bus"))
        return;

    QVector<CObjectData> obj_vector;
    QVector<const google::protobuf::Message *> objs_msg;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    double timestamp = 0;
    AssignStruct(msg, descriptor, timestamp, "timestamp");
    auto objs_field = descriptor->FindFieldByName("objs");
    int size = reflection->FieldSize(msg, objs_field);
    for (int n = 0; n < size; ++n)
    {
        CObjectData obj;
        const auto &nmsg = reflection->GetRepeatedMessage(msg, objs_field, n);
        ParseObject(package_msg_name, nmsg, timestamp, obj);
        objs_msg.push_back(&nmsg);
        obj_vector.push_back(obj);
    }
    data_center_->InsertValue<QVector<CObjectData>>(package_msg_name, time, obj_vector);
    data_center_->InsertValue<QVector<const google::protobuf::Message *>>(package_msg_name, time, objs_msg);
    ParseFinished("topview", time);
}

void CObjectParser::ParseObject(const QString &package_msg_name, const google::protobuf::Message &msg, double time, CObjectData &obj)
{
    obj.timestamp_ = time;
    auto obj_descript = msg.GetDescriptor();
    AssignStruct(msg, obj_descript, obj.track_id_, "track_id");
    AssignStruct(msg, obj_descript, obj.pos_x_, "longitudinal_distance");
    AssignStruct(msg, obj_descript, obj.pos_y_, "lateral_distance");
    AssignStruct(msg, obj_descript, obj.length_, "length");
    AssignStruct(msg, obj_descript, obj.width_, "width");
    AssignStruct(msg, obj_descript, obj.heading_angle_, "heading_angle");
    AssignStruct(msg, obj_descript, obj.sensor_flag_, "sensor_flag");
    AssignStruct(msg, obj_descript, obj.classification_, "classification");
}

void CObjectParser::ParserPoint(const google::protobuf::Message &msg, cav::CPointData &point)
{
    auto point_descript = msg.GetDescriptor();
    AssignStruct(msg, point_descript, point.x_, "u");
    AssignStruct(msg, point_descript, point.y_, "v");
}

void CObjectParser::ParseLidarPoint(const google::protobuf::Message &msg, cav::CPointData &point)
{
    auto descriptor = msg.GetDescriptor();
    AssignStruct(msg, descriptor, point.x_, "x");
    AssignStruct(msg, descriptor, point.y_, "y");
    AssignStruct(msg, descriptor, point.z_, "z");
}

void CObjectParser::ParsePredictionObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    QVector<CObjectData> obj_vector;
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
    }
    data_center_->InsertValue<QVector<cav::CObjectData>>(package_msg_name, time, obj_vector);
}

void CObjectParser::ParseVisionDynamicObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("VpCameraProto.CamObjects"))
        return;

    QVector<CObjectData> obj_vector;
    QVector<cav::CObjBoxCV> obj_box_vector;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    double timestamp = 0;
    AssignStruct(msg, descriptor, timestamp, "time_stamp");
    auto objs_field = descriptor->FindFieldByName("objs");
    if (objs_field == nullptr)
        return;
    int size = reflection->FieldSize(msg, objs_field);
    for (int n = 0; n < size; ++n)
    {
        CObjectData obj;
        cav::CObjBoxCV obj_box;
        const auto &nmsg = reflection->GetRepeatedMessage(msg, objs_field, n);
        {
            obj.timestamp_ = time;
            auto obj_descript = nmsg.GetDescriptor();
            AssignStruct(nmsg, obj_descript, obj.track_id_, "track_id");
            AssignStruct(nmsg, obj_descript, obj_box.track_id_, "track_id");
            AssignStruct(nmsg, obj_descript, obj.pos_x_, "longitudinal_distance");
            AssignStruct(nmsg, obj_descript, obj.pos_y_, "lateral_distance");
            AssignStruct(nmsg, obj_descript, obj.length_, "length");
            AssignStruct(nmsg, obj_descript, obj.width_, "width");
            AssignStruct(nmsg, obj_descript, obj.heading_angle_, "heading_angle");
            obj.heading_angle_ = obj.heading_angle_ * 3.14 / 180.0;
            auto reflection = nmsg.GetReflection();
            auto left_top = obj_descript->FindFieldByName("left_top");
            if (left_top != nullptr)
            {
                const auto &left_top_msg = reflection->GetMessage(nmsg, left_top);
                ParserPoint(left_top_msg, obj_box.top_left_);
            }
            auto right_down = obj_descript->FindFieldByName("right_down");
            if (right_down != nullptr)
            {
                const auto &right_down_msg = reflection->GetMessage(nmsg, right_down);
                ParserPoint(right_down_msg, obj_box.bottom_right_);
            }
        }
        obj_vector.push_back(obj);
        obj_box_vector.push_back(obj_box);
    }
    data_center_->InsertValue<QVector<CObjectData>>(package_msg_name, time, obj_vector);
    ParseFinished("topview", time);
    data_center_->InsertValue<QVector<cav::CObjBoxCV>>(package_msg_name, time, obj_box_vector);
    ParseFinished("cvision", time);
}

void CObjectParser::ParsesVisionStaticObjects(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("VpCameraProto.CamTsr"))
        return;
    QVector<cav::CObjectData> ts_vec;
    QVector<cav::CObjectData> gm_vec;
    QVector<cav::CObjectData> gg_vec;
    QVector<cav::CObjectData> tl_vec;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto field = descriptor->FindFieldByName("ts");
    int size = reflection->FieldSize(msg, field);
    for (int n = 0; n < size; n++)
    {
        cav::CObjectData obj;
        obj.track_id_ = n + 1;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, field, n);
        ParseVisionStaticObject(obj_msg, obj);
        ts_vec.push_back(obj);
    }
    field = descriptor->FindFieldByName("gm");
    size = reflection->FieldSize(msg, field);
    for (int n = 0; n < size; n++)
    {
        cav::CObjectData obj;
        obj.track_id_ = n + 1;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, field, n);
        ParseVisionStaticObject(obj_msg, obj);
        gm_vec.push_back(obj);
    }
    field = descriptor->FindFieldByName("gg");
    size = reflection->FieldSize(msg, field);
    for (int n = 0; n < size; n++)
    {
        cav::CObjectData obj;
        obj.track_id_ = n + 1;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, field, n);
        ParseVisionStaticObject(obj_msg, obj);
        gg_vec.push_back(obj);
    }
    field = descriptor->FindFieldByName("tl");
    size = reflection->FieldSize(msg, field);
    for (int n = 0; n < size; n++)
    {
        cav::CObjectData obj;
        obj.track_id_ = n + 1;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, field, n);
        ParseVisionStaticObject(obj_msg, obj);
        tl_vec.push_back(obj);
    }
    // qDebug() << "size" << ts_vec.size() << gm_vec.size() << gg_vec.size() << tl_vec.size();
    data_center_->InsertValue<QVector<cav::CObjectData>>(msg_name + "-ts", time, ts_vec);
    data_center_->InsertValue<QVector<cav::CObjectData>>(msg_name + "-gm", time, gm_vec);
    data_center_->InsertValue<QVector<cav::CObjectData>>(msg_name + "-gg", time, gg_vec);
    data_center_->InsertValue<QVector<cav::CObjectData>>(msg_name + "-tl", time, tl_vec);
    ParseFinished("topview", time);
}

void CObjectParser::ParseVisionStaticObject(const google::protobuf::Message &msg, CObjectData &obj)
{
    auto obj_reflection = msg.GetReflection();
    auto obj_descript = msg.GetDescriptor();
    AssignStruct(msg, obj_descript, obj.pos_x_, "longitudinal_distance");
    AssignStruct(msg, obj_descript, obj.pos_y_, "lateral_distance");
    AssignStruct(msg, obj_descript, obj.length_, "length");
    AssignStruct(msg, obj_descript, obj.width_, "width");
    AssignStruct(msg, obj_descript, obj.static_type_, "type");
}


void CObjectParser::ParseLidarObjects(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("LidarObjectProto.Objects"))
        return;
    QVector<cav::CObjectData> obj_vec;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto field = descriptor->FindFieldByName("obj");
    int size = reflection->FieldSize(msg, field);
    for (int n = 0; n < size; n++)
    {
        cav::CObjectData obj;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, field, n);
        auto obj_reflection = obj_msg.GetReflection();
        auto obj_descript = obj_msg.GetDescriptor();
        auto core_field = obj_descript->FindFieldByName("coreInfo");
        const auto &core_msg = obj_reflection->GetMessage(obj_msg, core_field);
        auto core_reflection = core_msg.GetReflection();
        auto core_descript = core_msg.GetDescriptor();
        AssignStruct(core_msg, core_descript, obj.track_id_, "track_id");
        {
            auto field = core_descript->FindFieldByName("center");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point;
            ParseLidarPoint(msg, point);
            obj.pos_x_ = point.x_;
            obj.pos_y_ = point.y_;
        }
        {
            auto field = core_descript->FindFieldByName("size");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point;
            ParseLidarPoint(msg, point);
            obj.width_ = point.y_;
            obj.length_ = point.x_;
        }
        {
            auto field = core_descript->FindFieldByName("direction");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point;
            ParseLidarPoint(msg, point);
            obj.heading_angle_ = point.z_ * 3.14 / 180;
        }
        {
            auto field = core_descript->FindFieldByName("velocity");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point;
            ParseLidarPoint(msg, point);
            obj.abs_velocity_x_ = point.x_;
            obj.abs_velocity_y_ = point.y_;
        }
        {
            auto field = core_descript->FindFieldByName("relative_velocity");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point;
            ParseLidarPoint(msg, point);
            obj.rel_velocity_x_ = point.x_;
            obj.rel_velocity_y_ = point.y_;
        }
        {
            auto field = core_descript->FindFieldByName("acceleration");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point;
            ParseLidarPoint(msg, point);
            obj.rel_acceleration_x_ = point.x_;
            obj.rel_acceleration_y_ = point.y_;
        }
        obj_vec.push_back(obj);
    }
    if (obj_vec.size() <= 0)
        return;
    data_center_->InsertValue<QVector<cav::CObjectData>>(msg_name, time, obj_vec);
    ParseFinished("topview", time);
}