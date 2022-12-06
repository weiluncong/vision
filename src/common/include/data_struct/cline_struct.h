#ifndef CLINE_STRUCT_H
#define CLINE_STRUCT_H

#include "cpoint_struct.h"
#include <QVector>

namespace cav
{
    // 车道线结构体
    class CLineData
    {
//    public:
//        enum cLineMode
//        {
//            Unknow,
//            LaneLine,       //车道线
//            PredictLine,    //预测线
//            EnvModeLane,    //环境模型
//        };

//        // 预测算法类型
//        enum cPredictionType
//        {
//            AlgorithmUnknow,
//            AlgorithmCar,           //预测算法输出 - 规则车辆
//            AlgorithmPerson,        //预测算法输出 - 规则行人
//            AlgorithmCslstm,        //预测算法输出 - cslstm
//            AlgorithmRaster,        //预测算法输出 - raster
//            AlgorithmGraph,         //预测算法输出 - graph
//        };

    public:
        CLineData() {}
        ~CLineData() {}

    public:
        double a0_ = 0;                  // 三阶曲线参数常数项
        double a1_ = 0;                  // 三阶曲线参数常数项
        double a2_ = 0;                  // 三阶曲线参数常数项
        double a3_ = 0;                  // 三阶曲线参数常数项
        double range_ = 0;               // 车道线范围
        double start_ = 0;               // x方向车道线起始点距离
        double end_ = 0;                 // x方向车道线终点距离
        int id_ = 0;                     // 车道线的id值
        int type_ = 0;                   // 车道线 1.虚线 2.点实线
        int lane_type_ = 0;              // 车道线类型，比如边界线、中心线、导向线
        int relation_to_ego_ = 0;            // 车道线与自车道相对位置 向左为负 向右为正  反向全部-1000 0表示在导航
        float confidence_ = 0;                 // 线的置信度
        bool navigation_status_ = false; // 是否在导航的线上


        QVector<CPointData> points_; // 点集，适用于地图和点集车道线
    };
} // namespace cav

#endif // CLANE_STRUCT_H
