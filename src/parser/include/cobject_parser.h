#ifndef COBJECT_PARSER_H
#define COBJECT_PARSER_H

#include "abstract_parser.h"
#include "cobj_struct.h"
#include "cvision_obj_struct.h"

class CObjectParser : public AbstractParser
{
public:
    CObjectParser() {}
    ~CObjectParser() {}

    void ParseObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 解析预测目标
    void ParsePredictionObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 解析视觉动态目标
    void ParseVisionDynamicObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 解析视觉静态目标
    void ParsesVisionStaticObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 激光目标解析 激光目标宽度为y，长度为x，heading_angle为z值
    void ParseLidarObjects(const QString &msg_name, const google::protobuf::Message &msg, double time);

private:
    void ParseObject(const QString &package_msg_name, const google::protobuf::Message &msg, double time, CObjectData &obj);
    void ParseVisionStaticObject(const google::protobuf::Message &msg, CObjectData &obj);
    void ParserPoint(const google::protobuf::Message &msg, cav::CPointData &point);
    void ParseLidarPoint(const google::protobuf::Message &msg, cav::CPointData &point);
};

#endif // COBJECT_PARSER_H
