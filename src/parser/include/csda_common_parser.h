#ifndef CSDA_COMMON_PARSER_H
#define CSDA_COMMON_PARSER_H

#include "cdata_center.h"
#include "abstract_parser.h"
#include "cmap_struct.h"
#include "cprediction_struct.h"
#include "cpathplanning_struct.h"
#include "creferenceline_struct.h"
#include "cvision_rle_struct.h"

/**
 * @brief SDA数据解析基类，用于构建数据结构联系、抽取通用功能函数。
 */
class CSdaCommmonParser : public AbstractParser
{
public:

    CSdaCommmonParser(){}
    ~CSdaCommmonParser(){}

    /**
     * @brief 数据有效值检查
     */
    template<typename T>
    bool ValueCheck(T & value)
    {
        if (!std::is_same<T,int>::value && 
            !std::is_same<T,float>::value && 
            !std::is_same<T,double>::value)
        {
            qDebug() << "value Type error!";
            return false;
        }
        if (std::isnan(value) || std::isinf(value))
            return false;
        else 
            return true;
    }

    /**
     * @brief 点云解析
     */
    cav::CPointData ParserPoint(const google::protobuf::Message &msg);

    /**
     * @brief 经纬度坐标解析
     */
    cav::CMapPoint ParserGnss(const google::protobuf::Message &msg);
    
    QString  FieldToQStr(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field);
};



#endif //CSDA_COMMON_PARSER_H
