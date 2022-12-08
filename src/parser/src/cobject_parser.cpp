#include "cobject_parser.h"

void CObjectParser::ParseObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (package_msg_name.contains("LidarObjectProto") || !package_msg_name.contains("Objects") || package_msg_name.contains("Bus"))
        return;

    QVector<CObjectData> obj_vector;
    QVector<cav::CObjBoxCV> obj_box_vector;
    //    QVector<const google::protobuf::Message *> objs_msg;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    double timestamp = 0;
    if (package_msg_name.contains("VpCameraProto"))
        AssignStruct(msg, descriptor, timestamp, "time_stamp");
    else
        AssignStruct(msg, descriptor, timestamp, "timestamp");
    auto objs_field = descriptor->FindFieldByName("objs");

    int size = reflection->FieldSize(msg, objs_field);
    for (int n = 0; n < size; ++n)
    {
        CObjectData obj;
        cav::CObjBoxCV obj_box;
        const auto &nmsg = reflection->GetRepeatedMessage(msg, objs_field, n);
        ParseObject(package_msg_name, nmsg, timestamp, obj, obj_box);
        //        objs_msg.push_back(&nmsg);
        obj_vector.push_back(obj);
        obj_box_vector.push_back(obj_box);
    }
    data_center_->InsertValue<QVector<CObjectData>>(package_msg_name, time, obj_vector);
    data_center_->InsertValue<QVector<cav::CObjBoxCV>>(package_msg_name, time, obj_box_vector);
    //    data_center_->InsertValue<QVector<const google::protobuf::Message *>>(package_msg_name, time, objs_msg);
    if (package_msg_name.contains("VpCameraProto"))
    {
        ParseFinished("cvision", time);
    }
    ParseFinished("topview", time);
}

void CObjectParser::ParseObject(const QString &package_msg_name, const google::protobuf::Message &msg, double time, CObjectData &obj, cav::CObjBoxCV &obj_box)
{
    obj.timestamp_ = time;
    auto obj_descript = msg.GetDescriptor();
    AssignStruct(msg, obj_descript, obj.track_id_, "track_id");
    AssignStruct(msg, obj_descript, obj_box.track_id_, "track_id");
    AssignStruct(msg, obj_descript, obj.pos_x_, "longitudinal_distance");
    AssignStruct(msg, obj_descript, obj.pos_y_, "lateral_distance");
    AssignStruct(msg, obj_descript, obj.length_, "length");
    AssignStruct(msg, obj_descript, obj.width_, "width");
    AssignStruct(msg, obj_descript, obj.heading_angle_, "heading_angle");

    if (!package_msg_name.contains("VpCameraProto"))
    {
        obj.heading_angle_ = obj.heading_angle_ * 3.14 / 180.0;
        AssignStruct(msg, obj_descript, obj.sensor_flag_, "sensor_flag");
        AssignStruct(msg, obj_descript, obj.classification_, "classification");
    }
    else
    {
        auto reflection = msg.GetReflection();
        auto left_top = obj_descript->FindFieldByName("left_top");
        if (left_top != nullptr)
        {
            const auto &left_top_msg = reflection->GetMessage(msg, left_top);
            ParserPoint(left_top_msg, obj_box.top_left_);
        }
        auto right_down = obj_descript->FindFieldByName("right_down");
        if (right_down != nullptr)
        {
            const auto &right_down_msg = reflection->GetMessage(msg, right_down);
            ParserPoint(right_down_msg, obj_box.bottom_right_);
        }
    }
}

void CObjectParser::ParserPoint(const google::protobuf::Message &msg, cav::CPointData &point)
{
    auto point_descript = msg.GetDescriptor();
    AssignStruct(msg, point_descript, point.x_, "u");
    AssignStruct(msg, point_descript, point.y_, "v");
}

void CObjectParser::ParsePredictionObject(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
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
