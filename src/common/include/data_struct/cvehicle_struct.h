#ifndef CVEHICLE_STRUCT_H
#define CVEHICLE_STRUCT_H

namespace cav
{
    enum cVehicleDataType
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
        cVehicleDataType type_ = cVehicleDataType::NONE;
    };
} // namespace cav

#endif // CVEHICLE_STRUCT_H