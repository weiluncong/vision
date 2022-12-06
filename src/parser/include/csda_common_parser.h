#ifndef CSDA_COMMON_PARSER_H
#define CSDA_COMMON_PARSER_H

#include "cdata_center.h"
#include "abstract_parser.h"
#include "cpathplanning_struct.h"
#include "creferenceline_struct.h"
#include "cvision_rle_struct.h"

/**
 * @brief SDA数据解析基类，用于构建数据结构联系、抽取通用功能函数。
 */
class CSdaCommmonParser : public AbstractParser
{
public:
    CSdaCommmonParser() {}
    ~CSdaCommmonParser() {}

    /**
     * @brief 数据有效值检查
     *
     * @tparam T int/float/double
     * @param value 数值
     * @return true 数据有效
     * @return false 数据无效
     */
    template <typename T>
    bool ValueCheck(T &value);

    /**
     * @brief 单点解析
     * @param CPointData 点:包含x，y，z
     * @return cav::CPointData 根据当前数据中是否有xyz依次填值，无则为cPointData默认值 0
     */
    CPointData ParserPoint(const google::protobuf::Message &msg);

    /**
     * @brief 经纬度坐标解析
     */
    CPointData ParserGnss(const google::protobuf::Message &msg);
};

template <typename T>
bool CSdaCommmonParser::ValueCheck(T &value)
{
    if (!std::is_same<T, int>::value &&
        !std::is_same<T, float>::value &&
        !std::is_same<T, double>::value)
    {
        qDebug() << "value Type error!";
        return false;
    }
    if (std::isnan(value) || std::isinf(value))
        return false;
    else
        return true;
}

#endif // CSDA_COMMON_PARSER_H
