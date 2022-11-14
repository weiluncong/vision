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
    for(auto i : data_ptr_map_.values())
    {
        SAFE_DELETE(i);
    }
    SafeClear(data_ptr_map_);
    data_start_time_ = 0;
    data_end_time_ = 0;
}
