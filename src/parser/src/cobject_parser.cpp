#include "cobject_parser.h"

void CObjectParser::ParseObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("Objects") || package_msg_name.contains("Bus"))
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
        ParseObject(nmsg, timestamp, obj);
        objs_msg.push_back(&nmsg);
        obj_vector.push_back(obj);
    }
    data_center_->InsertValue<QVector<CObjectData>>(package_msg_name, time, obj_vector);
    data_center_->InsertValue<QVector<const google::protobuf::Message *>>(package_msg_name, time, objs_msg);
    ParseFinished("topview", time);
}

void CObjectParser::ParseObject(const google::protobuf::Message &msg, const double &timestamp, CObjectData &obj)
{
    obj.timestamp_ = timestamp;
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