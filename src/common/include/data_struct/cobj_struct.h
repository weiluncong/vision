#ifndef COBJ_STRUCT_H
#define COBJ_STRUCT_H

#include <QtWidgets>
#include <cpoint_struct.h>

namespace cav
{
    class CObjectData
    {
    public:
        CObjectData() {}
        ~CObjectData() {}

    public:
        int track_id_ = 0; //目标ID
        int sensor_flag_ = 0;
        int classification_ = 0;   // 目标类别
        double pos_x_ = 0;         // 纵向距离
        double pos_y_ = 0;         // 横向距离
        double width_ = 0;         // 目标宽度
        double length_ = 0;        // 目标长度
        double heading_angle_ = 0; // 航向角
        double timestamp_ = 0;

        //lidar use
        double abs_velocity_x_ = 0;     //纵向绝对速度
        double abs_velocity_y_ = 0;     //横向绝对速度
        double rel_velocity_x_ = 0;     //纵向相对速度
        double rel_velocity_y_ = 0;     //横向相对速度
        double rel_acceleration_x_ = 0; //纵向相对加速度
        double rel_acceleration_y_ = 0; //横向相对加速度

        //prediction use
        int move_status_ = 0;           //目标运动状态 0
        int algorithm_type_ = 0;        //预测算法类型
        // float prob_ = 0;                //目标预测轨迹对应的概率

        //vision use
        int static_type_ = 0;       //视觉静态目标类型
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