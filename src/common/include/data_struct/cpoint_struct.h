#ifndef CPOINT_STRUCT_H
#define CPOINT_STRUCT_H

namespace cav
{
    class CPointData
    {
    public:
        CPointData() {}
        CPointData(double x, double y, double z) 
        {
            x_ = x;
            y_ = y;
            z_ = z;
        }
        ~CPointData() {}

    public:
        int ped_result_ = 0; // 是否行人遮挡
        int ped_azi_ = 0;    // 1：行人在车左边；2：行人在车右边
        double x_ = 0;
        double y_ = 0;
        double z_ = 0;
    };
} // namespace cav

#endif // CPOINT_STRUCT_H