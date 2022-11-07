#include "cdata_producer.h"

CDataProducer::CDataProducer(QObject *parent)
    : QObject(parent)
{
    receive_data_ = new CReceiveData();
    data_center_ = CDataCenter::GetCDataCenter();
}

CDataProducer::~CDataProducer()
{
    SAFE_DELETE(receive_data_);
    SAFE_DELETE(load_data_);
}

void CDataProducer::Switch()
{
    if (FLAGS_v_online)
    {
        data_center_->ClearAllData();
        if (receive_data_ == nullptr)
            receive_data_ = new CReceiveData();
        SAFE_DELETE(load_data_);
    }
    else
    {
        if (receive_data_ != nullptr)
            SAFE_DELETE(receive_data_);
    }
}

void CDataProducer::OpenFile(const QString &file_path)
{
    data_center_->ClearAllData();
    if (load_data_ == nullptr)
        load_data_ = new CLoadData(file_path);

    std::thread t(&CLoadData::Dat2Replay, load_data_);
    t.detach();
}
