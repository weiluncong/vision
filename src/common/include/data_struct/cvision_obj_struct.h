#ifndef CVISION_OBJ_STRUCT_H
#define CVISION_OBJ_STRUCT_H

#include "cpoint_struct.h"

namespace cav
{
    /**
     * @brief vision use： 视频目标点集
     * 目前图像数据存储为640*480,点集需在原图 3840 * 2160绘制
     * @param id_  目标编号
     * @param top_left_  左上点
     * @param bottom_right_ 右下点
     */
    class CObjBoxCV
    {
    public:
        CObjBoxCV() {}
        ~CObjBoxCV() {}

    public:
        int track_id_ = 0;
        cav::CPointData top_left_;     //目标视频左上角坐标点
        cav::CPointData bottom_right_; //目标视频右下角坐标点
    };
}

#endif // CVISION_OBJ_STRUCT_H
