#include "cpoint_set_parser.h"

void CPointSetParser::ParseFreeSpace(const QString &package_msg_name, const google::protobuf::Message &msg, const double time)
{
    if (!package_msg_name.contains("FreeSpace") || package_msg_name.contains("Bus"))
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
        auto fs_reflection = nmsg.GetReflection();
        auto fs_descriptor = nmsg.GetDescriptor();
        AssignStruct(nmsg, fs_descriptor, point.z_, "motion_state");
        auto position_field = fs_descriptor->FindFieldByName("position");
        const auto &pmsg = fs_reflection->GetMessage(nmsg, position_field);
        auto position_descriptor = pmsg.GetDescriptor();
        AssignStruct(pmsg, position_descriptor, point.x_, "longitudinal_distance");
        AssignStruct(pmsg, position_descriptor, point.y_, "lateral_distance");
        point_vector.push_back(point);
    }
    data_center_->InsertValue<QVector<CPointData>>(package_msg_name, time, point_vector);
    ParseFinished("topview", time);
}