#include "cpoint_record.h"
#include "cdata_point_record.h"

CPointRecord::CPointRecord(CDataPointRecord *point_record)
{
    point_record_ = point_record;
}

CPointRecord::~CPointRecord()
{
    SafeClear(point_record_->data_center_->data_buf_);
}

void CPointRecord::DoRecord()
{
    QString name = point_record_->file_path_ + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".dat";

    connect(point_record_->signal_manager_, &CSignalManager::SigUpdateDataBufSize,
            this, &CPointRecord::WaitDataBufUpdate);
    CDataRecord *data_record = new CDataRecord(name.toStdString());
    data_record->SetProtoContent(point_record_->proto_content_);
    data_record->Init();

    /** @brief 第一次读取缓存区数据*/
    std::multimap<double, std::string> data_buf;
    {
        std::lock_guard<std::mutex> record_lg(point_record_->data_center_->record_mutex_);
        data_buf = point_record_->data_center_->data_buf_;
    }
    for (auto i : data_buf)
    {
        data_record->Record(i.second);
    }

    /** @brief 等待缓存区填满*/
    std::unique_lock<std::mutex> record_lg(point_record_mutex_);
    condition_var_.wait(record_lg);
    {
        std::lock_guard<std::mutex> record_lg(point_record_->data_center_->record_mutex_);
        data_buf = point_record_->data_center_->data_buf_;
    }
    for(auto i : data_buf)
    {
        data_record->Record(i.second);
    }
    SAFE_DELETE(data_record);
    emit SigPointRecordOver();
}

void CPointRecord::WaitDataBufUpdate()
{
    data_buf_size_++;
    if (data_buf_size_ == FLAGS_v_point_record_size)
    {
        condition_var_.notify_one();
        disconnect(point_record_->signal_manager_, &CSignalManager::SigUpdateDataBufSize,
                this, &CPointRecord::WaitDataBufUpdate);
    }
}