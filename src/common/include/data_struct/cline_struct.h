#ifndef CLINE_STRUCT_H
#define CLINE_STRUCT_H

#include "cpoint_struct.h"
namespace cav
{
    // 车道线结构体
    class CLineData
    {
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
        int type_ = 0;                   // 车道线类型
        int lane_type_ = 0;              // 车道类型，比如边界线、中心线、导向线
        bool navigation_status_ = false; // 是否在导航的线上
        unsigned int id_ = 0;            // 车道线的id值

        QVector<CPointData> points_; // 点集，适用于地图和点集车道线
    };
} // namespace cav

#endif // CLANE_STRUCT_H