#include "cobject_parser.h"

void CObjectParser::ParseObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (package_msg_name.contains("LidarObjectProto") || !package_msg_name.contains("Objects") || package_msg_name.contains("Bus"))
        return;
    msg_name_ = package_msg_name;
    QVector<CObjectData> obj_vector;
    QVector<cav::CObjBoxCV> obj_box_vector;
//    QVector<const google::protobuf::Message *> objs_msg;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    double timestamp = 0;
    if (msg_name_.contains("VpCameraProto"))
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
        ParseObject(nmsg, timestamp, obj, obj_box);
//        objs_msg.push_back(&nmsg);
        obj_vector.push_back(obj);
        obj_box_vector.push_back(obj_box);
    }
    data_center_->InsertValue<QVector<CObjectData>>(package_msg_name, time, obj_vector);
    data_center_->InsertValue<QVector<cav::CObjBoxCV>>(package_msg_name, time, obj_box_vector);
//    data_center_->InsertValue<QVector<const google::protobuf::Message *>>(package_msg_name, time, objs_msg);
    if (msg_name_.contains("VpCameraProto"))
    {
        ParseFinished("cvision", time);
    }
    ParseFinished("topview", time);
}

void CObjectParser::ParseObject(const google::protobuf::Message &msg, const double &timestamp, CObjectData &obj, cav::CObjBoxCV &obj_box)
{
    obj.timestamp_ = timestamp;
    auto obj_descript = msg.GetDescriptor();
    AssignStruct(msg, obj_descript, obj.track_id_, "track_id");
    AssignStruct(msg, obj_descript, obj_box.track_id_, "track_id");

    AssignStruct(msg, obj_descript, obj.pos_x_, "longitudinal_distance");
    AssignStruct(msg, obj_descript, obj.pos_y_, "lateral_distance");
    AssignStruct(msg, obj_descript, obj.length_, "length");
    AssignStruct(msg, obj_descript, obj.width_, "width");
    AssignStruct(msg, obj_descript, obj.heading_angle_, "heading_angle");
    
    if (!msg_name_.contains("VpCameraProto"))
    {
        AssignStruct(msg, obj_descript, obj.sensor_flag_, "sensor_flag");
        AssignStruct(msg, obj_descript, obj.classification_, "classification");
    }
    else
    {
        auto descriptor = msg.GetDescriptor();
        auto reflection = msg.GetReflection();
        auto left_top = descriptor->FindFieldByName("left_top");
        if (left_top != nullptr)
        {
            const auto &left_top_msg = reflection->GetMessage(msg, left_top);
            ParserPoint(left_top_msg, obj_box.top_left_);
        }
        auto right_down = descriptor->FindFieldByName("right_down");
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
