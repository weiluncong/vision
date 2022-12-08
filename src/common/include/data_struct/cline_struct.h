#ifndef CLINE_STRUCT_H
#define CLINE_STRUCT_H

#include "cpoint_struct.h"
#include <QVector>

namespace cav
{
    // 车道线结构体
    class CLineData
    {
    public:
        CLineData() {}
        ~CLineData() {}

        enum LineType{
            SolidLine = 1,
            DashedLine,
            RedSolidLine,
            GreenSolidLine,
            BlueSolidLine
        };

    public:
        double a0_ = 0;                  // 三阶曲线参数常数项
        double a1_ = 0;                  // 三阶曲线参数常数项
        double a2_ = 0;                  // 三阶曲线参数常数项
        double a3_ = 0;                  // 三阶曲线参数常数项
        double range_ = 0;               // 车道线范围
        double start_ = 0;               // x方向车道线起始点距离
        double end_ = 0;                 // x方向车道线终点距离
        unsigned int id_ = 0;                     // 车道线的id值
        int type_ = 0;               //  1.虚线 2.点实线
        int lane_type_ = 0;              // 车道线类型，比如边界线、中心线、导向线
        int relation_to_ego_ = 0;            // 车道线与自车道相对位置 向左为负 向右为正  反向全部-1000 0表示在导航
        float confidence_ = 0;                 // 线的置信度
        bool navigation_status_ = false; // 是否在导航的线上A

        QVector<CPointData> points_; // 点集，适用于地图和点集车道线
    };
} // namespace cav

#endif // CLANE_STRUCT_H
