#ifndef CMAP_STRUCT_H
#define CMAP_STRUCT_H

#include <QString>
#include <QColor>
#include <QVector>

#include "cpoint_struct.h"
#include "cobj_struct.h"

namespace cav
{
    /**
     * @brief hdMap use： to descript gnss location
     * @param longitude longtitude 经度
     * @param latitude latitude 纬度
     */
    class CMapPoint
    {
    public:
        CMapPoint() {}
        ~CMapPoint() {}

    public:
        double longitude_ = 0;
        double latitude_ = 0;
    };

    /**
     * @brief hdMap use： to descript laneline
     * @param lineType 车道线类型（单实线，双实线，取markingtype里的第一个）
     * @param linePoints 单线点集 gnss坐标
     */
    class CMapLine
    {
    public:
        CMapLine() {}
        ~CMapLine() {}

    public:
        unsigned long index_ = 0;
        int type_ = 0;
        bool in_navigation_route_ = false;
        int marking_type_;
        QVector<CMapPoint> points_;
    };

    class CStaticHdMapData
    {
    public:
        CStaticHdMapData() {}
        ~CStaticHdMapData() {}

    public:
        int link_id_ = 0;
        QVector<CMapLine> lines_;
    };

    /**
     * @brief insdata use： 车身位置
     * 纬度,正为N半球:[deg],(0,-90,+90),[/],(/,/),标识维度信息
     * 经度,正为E半球:[deg],(0,-180,+180),[/],(/,/),标识经度信息
     * @param gns  当前定位坐标点
     * @param heading 航向角:[deg],(0,0,360),[/],(/,/),标识航向角,正东方向逆时针为正
     */
    class CMapInsData
    {
    public:
        CMapInsData() {}
        ~CMapInsData() {}

    public:
        double heading_ = 0;
        CMapPoint gnss_;
    };
}

#endif // CMAP_STRUCT_H
