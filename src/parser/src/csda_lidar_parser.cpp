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
