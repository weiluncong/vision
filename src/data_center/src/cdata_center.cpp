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

void CDataCenter::InitDataMap()
{
    img_datas_ = new CDataMap<CImageData>();
    middle_datas_ = new CDataMap<google::protobuf::Message *>();
}

void CDataCenter::ClearAllData()
{
    SAFE_DELETE(img_datas_);
    SAFE_DELETE(middle_datas_);
    data_start_time_ = 0;
    data_end_time_ = 0;
    InitDataMap();
}
