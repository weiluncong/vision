#ifndef CPATHPLANNING_STRUCT_H
#define CPATHPLANNING_STRUCT_H

#include "cpoint_struct.h"
#include <QVector>

namespace cav
{
    /**
     * 路径规划
     */
    class CPathPlanningData
    {
    public:
        CPathPlanningData() {}
        ~CPathPlanningData() {}

        int id_ = 0;
        QVector<cav::CPointData> points_;
    };
}

#endif // CPATHPLANNING_STRUCT_H