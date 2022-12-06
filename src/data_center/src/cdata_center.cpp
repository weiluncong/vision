#include "cdata_center.h"

CDataCenter *CDataCenter::data_center_ = nullptr;

CDataCenter *CDataCenter::GetCDataCenter()
{
    if (!data_center_)
    {
        data_center_ = new CDataCenter();
    }
    return data_center_;
}

void CDataCenter::Destory()
{
    SAFE_DELETE(data_center_);
}

void CDataCenter::ClearAllData()
{
    auto img_data_ptr = GetDataPtr<CImageData>();
    SAFE_DELETE(img_data_ptr);
    auto obj_data_ptr = GetDataPtr<QVector<CObjectData>>();
    SAFE_DELETE(obj_data_ptr);
     auto line_data_ptr = GetDataPtr<QVector<CLineData>>();
     SAFE_DELETE(line_data_ptr);
    auto point_data_ptr = GetDataPtr<QVector<CPointData>>();
    SAFE_DELETE(point_data_ptr);
    auto obj_box_data_ptr = GetDataPtr<QVector<CObjBoxCV>>();
    SAFE_DELETE(obj_box_data_ptr);
    auto map_line_data_ptr = GetDataPtr<QMap<int, QVector<CMapLine>>>();
    SAFE_DELETE(map_line_data_ptr);
    auto map_ins_data_ptr = GetDataPtr<CMapInsData>();
    SAFE_DELETE(map_ins_data_ptr);

    SafeClear(data_ptr_map_);
    data_start_time_ = 0;
    data_end_time_ = 0;
}
