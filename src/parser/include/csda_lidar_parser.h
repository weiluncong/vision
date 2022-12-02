#ifndef CSDA_LIDAR_PARSER_H
#define CSDA_LIDAR_PARSER_H

#include "csda_common_parser.h"

/**
 * @brief SDA 激光雷达数据解析 包含激光目标、激光freespace
 */
class CSDALidarParser : public CSdaCommmonParser
{
public:
    CSDALidarParser() {}
    ~CSDALidarParser() {}
    
    // 激光目标解析 激光目标宽度为y，长度为x，heading_angle为z值
    void ParseLidarObjects(const QString &msg_name, const google::protobuf::Message &msg, double time);
    // 激光freespace解析
    void ParseLidarFreeSpace(const QString &msg_name, const google::protobuf::Message &msg, double time);
    

private:
    // 解析轨迹包
    void ParserTrajectory(const google::protobuf::Message &msg, QVector<cav::CPointData> &line);
    // 解析轨迹集
    void ParserTrajectoryMultimodal(const google::protobuf::Message &msg, cav::CPredictLine &debug_line);
};

#endif // CSDA_LIDAR_PARSER_H