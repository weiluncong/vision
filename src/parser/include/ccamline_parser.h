#ifndef CCAMLINE_PARSER_H
#define CCAMLINE_PARSER_H

#include "abstract_parser.h"
#include "cline_struct.h"

class CCamLineParser : public AbstractParser
{
public:
    CCamLineParser() {}
    ~CCamLineParser() {}

    void ParseCamLines(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    void ParseVpCamLines(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    void ParseLine(const google::protobuf::Message &msg, CLineData &line);
    // 预测轨迹线解析
    void ParsePredictionLines(const QString &package_msg_name, QVector<const google::protobuf::Message *> msgs, double time);
    void ParsePredictObjectDebug(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 规划线解析
    void ParsePlanningPath(const QString &package_msg_name, const google::protobuf::Message &msg, double time);

private:
    // 解析预测信号，解析目标debug信息
    void ParserTrajectoryMultimodal(const google::protobuf::Message &msg, CLineData &line);
    // 解析单条轨迹线
    void ParserTrajectory(const google::protobuf::Message &msg, QVector<cav::CPointData> &line);

    CPointData ParserPoint(const google::protobuf::Message &msg);
};
#endif // CCAMLINE_PARSER_H
