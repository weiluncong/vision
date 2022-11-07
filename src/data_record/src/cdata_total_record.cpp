#include "cdata_total_record.h"

CDataTotalRecord::CDataTotalRecord(const QString &path, QObject *parent)
    : file_path_(path), QObject(parent)
{
    data_center_ = CDataCenter::GetCDataCenter();
    proto_pool_ = CProtoPool::GetCProtoPool();
}

void CDataTotalRecord::GetProtoContent()
{
    std::string temp_content = proto_pool_->GetProtoContent();
    if (!temp_content.empty())
        proto_content_ = temp_content;
}

void CDataTotalRecord::OpenFile()
{
    QString name = file_path_ + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".dat";
    ReleaseRecord();
    data_record_ = new CDataRecord(name.toStdString());
    data_record_->SetProtoContent(proto_content_);
    data_record_->Init();
}

void CDataTotalRecord::StartRecord()
{
    GetProtoContent();
    std::thread t(&CDataTotalRecord::DoRecord, this);
    t.detach();
}

void CDataTotalRecord::DoRecord()
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (FLAGS_v_total_record)
        {
            if (data_center_->data_buf_.empty())
            {
                continue;
            }
            if (data_record_ == nullptr || data_record_->GetRecordedSize() >= FLAGS_v_total_record_size)
            {
                OpenFile();
            }
            std::multimap<double, std::string> data_buf;
            {
                std::lock_guard<std::mutex> record_lg(data_center_->record_mutex_);
                data_buf = std::move(data_center_->data_buf_);
            }
            for (auto i : data_buf)
            {
                data_record_->Record(i.second);
            }
        }
    }
}

void CDataTotalRecord::ReleaseRecord()
{
    SAFE_DELETE(data_record_);
    if (!FLAGS_v_total_record)
        SafeClear(data_center_->data_buf_);
}
