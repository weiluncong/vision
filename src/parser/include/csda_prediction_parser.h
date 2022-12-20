#ifndef CSDA_PREDICTION_PARSER_H
#define CSDA_PREDICTION_PARSER_H

#include "csda_common_parser.h"
#include "cline_struct.h"
#include "ccamline_parser.h"
#include "cobject_parser.h"

/**
 * @brief SDA 预测信号解析 包含预测目标、预测轨迹、历史轨迹、多算法预测轨迹
 */
class CSDAPredictionParser : public CSdaCommmonParser
{
public:
    CSDAPredictionParser();
    ~CSDAPredictionParser();
    // 解析预测信号，包含目标信息、目标历史轨迹、预测轨迹
    void ParsePredictions(const QString &package_msg_name, const google::protobuf::Message &msg, double time);

private:
    CCamLineParser *line_parser_;
    CObjectParser *object_parser_;
};

#endif // CSDA_PREDICTION_PARSER_H
