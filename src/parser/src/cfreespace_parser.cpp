#include "cfreespace_parser.h"

CFreeSpaceParser::CFreeSpaceParser()
{
}

void CFreeSpaceParser::ParseFreeSpace(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    bool filter_switch = false;
    filter_switch |= package_msg_name.contains("FreeSpace");
    filter_switch &= !package_msg_name.contains("Bus");
    if (!filter_switch)
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
        AssignStruct(nmsg, fs_descript, point.z_, "motion_state");
        auto position_field = fs_descript->FindFieldByName("position");
        const auto &pmsg = fs_reflect->GetMessage(nmsg, position_field);
        auto position_descript = pmsg.GetDescriptor();
        AssignStruct(pmsg, position_descript, point.x_, "longitudinal_distance");
        AssignStruct(pmsg, position_descript, point.y_, "lateral_distance");
        point_vector.append(point);
    }
    data_center_->InsertValue(package_msg_name, time, point_vector);
    ParseFinished("topview", time);
}
