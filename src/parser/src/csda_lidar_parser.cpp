#include "csda_lidar_parser.h"

void CSDALidarParser::ParseLidarFreeSpace(const QString &msg_name, const google::protobuf::Message &msg, double time)
{
    if (!msg_name.contains("LidarFreeSpaceProto.FreeSpaceData"))
        return;
    QVector<cav::CPointData> data;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto field = descriptor->FindFieldByName("freespace");
    int field_count = descriptor->field_count();
    int size = reflection->FieldSize(msg, field);
    for (int i = 0; i < size; ++i)
    {
        const auto &center_msg = reflection->GetRepeatedMessage(msg, field, i);
        cav::CPointData point = ParserPoint(center_msg);
        data.push_back(point);
    }
    data_center_->InsertValue<QVector<cav::CPointData>>(msg_name, time, data);
    ParseFinished("topview", time);
}

void CSDALidarParser::ParseLidarObjects(const QString &msg_name, const google::protobuf::Message &msg, double time)
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
        auto core_field = obj_descript->FindFieldByName("coreinfo");
        const auto &core_msg = obj_reflection->GetMessage(obj_msg, core_field);
        auto core_reflection = core_msg.GetReflection();
        auto core_descript = core_msg.GetDescriptor();
        AssignStruct(core_msg, core_descript, obj.track_id_, "track_id");
        {
            auto field = core_descript->FindFieldByName("center");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point = ParserPoint(msg);
            obj.pos_x_ = point.x_;
            obj.pos_y_ = point.y_;
        }
        {
            auto field = core_descript->FindFieldByName("size");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point = ParserPoint(msg);
            obj.width_ = point.y_;
            obj.length_ = point.x_;
        }
        {
            auto field = core_descript->FindFieldByName("direction");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point;
            point = ParserPoint(msg);
            obj.heading_angle_ = point.z_ * 3.14 / 180;
        }
        {
            auto field = core_descript->FindFieldByName("velocity");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point = ParserPoint(msg);
            obj.abs_velocity_x_ = point.x_;
            obj.abs_velocity_y_ = point.y_;
        }
        {
            auto field = core_descript->FindFieldByName("relative_velocity");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point = ParserPoint(msg);
            obj.rel_velocity_x_ = point.x_;
            obj.rel_velocity_y_ = point.y_;
        }
        {
            auto field = core_descript->FindFieldByName("acceleration");
            const auto &msg = core_reflection->GetMessage(core_msg, field);
            cav::CPointData point = ParserPoint(msg);
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