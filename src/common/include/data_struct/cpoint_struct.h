#ifndef CPOINT_STRUCT_H
#define CPOINT_STRUCT_H

namespace cav
{
    class CPointData
    {
    public:
        CPointData() {}
        ~CPointData() {}

        CPointData(double x, double y, double z)
            :x_(x), y_(y), z_(z)
        {}

    public:
        double x_ = 0;
        double y_ = 0;
        double z_ = 0;

        double longitude_ = 0; // longtitude 经度 正为E半球:[deg],(0,-180,+180),[/],(/,/),标识经度信息
        double latitude_ = 0;  // latitude 纬度 正为N半球:[deg],(0,-90,+90),[/],(/,/),标识维度信息
        double heading_ = 0;   // heading 航向角:[deg],(0,0,360),[/],(/,/),标识航向角,正东方向逆时针为正
    };
} // namespace cav

#endif // CPOINT_STRUCT_H