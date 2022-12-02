#ifndef CLANE_STRUCT_H
#define CLANE_sTRUCT_H

namespace cav
{
    // 车道线结构体
    class CLaneData
    {
    public:
        CLaneData() {}
        ~CLaneData() {}

    public:
        double a0_ = 0;    // 三阶曲线参数常数项
        double a1_ = 0;    // 三阶曲线参数常数项
        double a2_ = 0;    // 三阶曲线参数常数项
        double a3_ = 0;    // 三阶曲线参数常数项
        double range_ = 0; // 车道线范围
        double start_ = 0; // x方向车道线起始点距离
        double end_ = 0;   // x方向车道线终点距离
        int type_ = 0;     // 车道线类型
    };
} // namespace cav

#endif // CLANE_STRUCT_H