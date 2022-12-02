#ifndef CSDA_PARSER_H
#define CSDA_PARSER_H

#include "csda_common_parser.h"

/**
 * @brief SDA数据独有解析类，包含规控、预测、决策、图尚地图所需数据源 
 */
class CSdaParser: public CSdaCommmonParser
{
public:
    CSdaParser(){}

    ~CSdaParser(){}

    // 图尚地图解析
    void ParseHDMap(const QString &msg_name, const google::protobuf::Message &msg, double time);
    // 定位信息解析
    void ParseIns(const QString &msg_name, const google::protobuf::Message &msg, double time);
    // 众包地图解析
    void ParseIdmapStatic(const QString &msg_name, const google::protobuf::Message &msg, double time);
    // 环境模型解析
    void ParseRNPEnvOut(const QString &msg_name, const google::protobuf::Message &msg, double time);
    // 解析预测信号，包含目标信息、目标历史轨迹、预测轨迹
    void ParsePredictions(const QString &msg_name, const google::protobuf::Message &msg, double time);
    // 解析预测信号，解析目标debug信息
    void ParsePredictObjectDebug(const QString &msg_name, const google::protobuf::Message &msg, double time);
    
    // 激光freespace解析
    void ParseLidarFreeSpace(const QString &msg_name, const google::protobuf::Message &msg, double time);
    // 激光freespace解析
    void ParseLidarObjects(const QString &msg_name, const google::protobuf::Message &msg, double time);

private:
    // 解析轨迹包
    void ParserTrajectory(const google::protobuf::Message &msg, QVector<cav::CPointData> &line);
    // 解析轨迹集
    void ParserTrajectoryMultimodal(const google::protobuf::Message &msg, cav::CPredictLine &debug_line);

};

#endif  //CSDA_PARSER_H
