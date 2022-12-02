#ifndef CREFERENCELINE_STRUCT_H
#define CREFERENCELINE_STRUCT_H

#include "cpoint_struct.h"
#include <QVector>

namespace cav
{
    /**
     * @brief decisionPoints use：
     * @param id 决策线序号
     * @param num_points 参考路径对应的位置点数量， 最大500
     * @param points 决策轨迹线
     */
    class CReferenceLine
    {
    public:
        CReferenceLine() {}
        ~CReferenceLine() {}

    public:
        int64_t id_ = 0;
        int32_t num_points_ = 0;          // 参考路径对应的位置点数量， 最大500
        QVector<cav::CPointData> points_; // 位置点数组
    };
}

#endif // CREFERENCELINE_STRUCT_H