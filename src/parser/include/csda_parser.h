#ifndef CSDA_PARSER_H
#define CSDA_PARSER_H

#include "csda_common_parser.h"

/**
 * @brief SDA数据独有解析类，包含规控、预测、决策、图尚地图所需数据源
 */
class CSdaParser : public CSdaCommmonParser
{
public:
    CSdaParser() {}
    ~CSdaParser() {}

    // 图尚地图解析
    void ParseHDMap(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 定位信息解析
    void ParseIns(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 众包地图解析
    void ParseIdmapStatic(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    // 环境模型解析
    void ParseRNPEnvOut(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
};

#endif // CSDA_PARSER_H
