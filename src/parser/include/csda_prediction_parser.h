#ifndef CSDA_PREDICTION_PARSER_H
#define CSDA_PREDICTION_PARSER_H

#include "csda_common_parser.h"

/**
 * @brief SDA 预测信号解析 包含预测目标、预测轨迹、历史轨迹、多算法预测轨迹
 */
class CSDAPredictionParser : public CSdaCommmonParser
{
public:
    CSDAPredictionParser() {}
    ~CSDAPredictionParser() {}
    // 解析预测信号，包含目标信息、目标历史轨迹、预测轨迹
    void ParsePredictions(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 解析预测信号，解析目标debug信息
    void ParsePredictObjectDebug(const QString &package_msg_name, const google::protobuf::Message &msg, double time);

private:
    // 解析轨迹包
    void ParserTrajectory(const google::protobuf::Message &msg, QVector<cav::CPointData> &line);
    // 解析轨迹集
    void ParserTrajectoryMultimodal(const google::protobuf::Message &msg, cav::CPredictLine &debug_line);
};

#endif // CSDA_PREDICTION_PARSER_H