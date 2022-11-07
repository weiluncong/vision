#include "cdata_point_record.h"

CDataPointRecord::CDataPointRecord(const QString &file_path, QObject *parent)
    : QObject(parent)
{
    file_path_ = file_path;
    data_center_ = CDataCenter::GetCDataCenter();
    signal_manager_ = CSignalManager::GetCSignalManager();
    proto_pool_ = CProtoPool::GetCProtoPool();
}

CDataPointRecord::~CDataPointRecord()
{
    for(auto i : point_recorders_)
    {
        SAFE_DELETE(i);
    }
}

void CDataPointRecord::GetProtoContent()
{
    std::string temp_content = proto_pool_->GetProtoContent();
    if (!temp_content.empty())
        proto_content_ = temp_content;
}

void CDataPointRecord::StartPointRecord()
{
    if(proto_content_.empty())
    {
        GetProtoContent();
    }
    CPointRecord *point_record = new CPointRecord(this);
    connect(point_record, &CPointRecord::SigPointRecordOver,
            [this]{
                CPointRecord *record = static_cast<CPointRecord *>(sender());
                for(auto it = point_recorders_.begin(); it != point_recorders_.end(); ++it)
                {
                    if(*it == record)
                    {
                        point_recorders_.erase(it);
                        break;
                    }
                }
            });
    point_recorders_.push_back(point_record);
    std::thread t(&CPointRecord::DoRecord, point_record);
    t.detach();
}