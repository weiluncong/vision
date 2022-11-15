#ifndef CVEHICLE_STRUCT_H
#define CVEHICLE_STRUCT_H

namespace cav
{
    enum kVehicleDataType
    {
        NONE,      // NONE
        SPEED,     // 速度
        LONGITUDE, // 经度
        LATITUDE   // 纬度
    };

    class CVehicleData
    {
    public:
        CVehicleData() {}
        ~CVehicleData() {}

    public:
        double value_ = 0;
        kVehicleDataType type_ = kVehicleDataType::NONE;
    };
} // namespace cav

#endif // CVEHICLE_STRUCT_H