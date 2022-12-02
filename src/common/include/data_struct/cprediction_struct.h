#ifndef CPREDICTION_STRUCT_H
#define CPREDICTION_STRUCT_H

#include "cobj_struct.h"
#include "cpoint_struct.h"
#include <QVector>

namespace cav
{
    
    /**
     * @brief prediction use：
     * @param 输出算法类型
     */
    enum CPredictionType
    {
        AlgorithmUnknow = 0,
        AlgorithmCar,           //算法输出 - 规则车辆
        AlgorithmPerson,        //算法输出 - 规则行人
        AlgorithmCslstm,        //算法输出 - cslstm
        AlgorithmRaster,        //算法输出 - raster
        AlgorithmGraph,         //算法输出 - graph
    };

    /**
     * @brief prediction use：
     * @param id 目标序号
     * @param type_ 输出算法类型
     * @param prob_ 置信度
     * @param predict_line_ 预测轨迹线
     */
    class CPredictLine
    {
    public:
        CPredictLine() {}
        ~CPredictLine() {}
    public:
        int id_ = 0;
        cav::CPredictionType type_ = CPredictionType::AlgorithmUnknow;
        float prob_ = 0;
        QVector<cav::CPointData> predict_line_;
    };
}

#endif // CPREDICTION_STRUCT_H