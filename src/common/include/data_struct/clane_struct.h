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
        double a0_ = 0;
        double a1_ = 0;
        double a2_ = 0;
        double a3_ = 0;
        double range_ = 0;
        double start_ = 0;
        double end_ = 0;
        int type_ = 0;
    };
} // namespace cav

#endif // CLANE_STRUCT_H