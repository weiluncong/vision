#ifndef COBJ_STRUCT_H
#define COBJ_STRUCT_H

#include <QtWidgets>

namespace cav
{
    class CObjectData
    {
    public:
        CObjectData() {}
        ~CObjectData() {}

    public:
        int track_id_ = 0;
        int sensor_flag_ = 0;
        int classification_ = 0;
        double pos_x_ = 0;
        double pos_y_ = 0;
        double width_ = 0;
        double length_ = 0;
        double heading_angle_ = 0;
        double timestamp_ = 0;
    };

    class CExObjectData
    {
    public:
        CExObjectData() {}
        ~CExObjectData() {}

    public:
        CObjectData parent_obj_;
        QVector<CObjectData> child_objs_;
    };
} // namespace cav

#endif // COBJ_STRUCT_H