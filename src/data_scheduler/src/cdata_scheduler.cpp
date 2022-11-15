#include "cdata_scheduler.h"

CDataScheduler *CDataScheduler::data_scheduler_ = nullptr;

CDataScheduler::CDataScheduler()
{
    CAbstractScheduler *camera_scheduler = new CCameraScheduler();
    widget_schedulers_["camera"] = camera_scheduler;

    data_center_ = CDataCenter::GetCDataCenter();
    signal_manager_ = CSignalManager::GetCSignalManager();

    connect(signal_manager_, &CSignalManager::SigParseFinished,
            this, &CDataScheduler::HandleParserFinished);
    connect(signal_manager_, &CSignalManager::SigSliderMoved,
            this, &CDataScheduler::HandleSliderMoved);
    connect(signal_manager_, &CSignalManager::SigSyncData,
            this, &CDataScheduler::SyncData);
}

CDataScheduler::~CDataScheduler()
{
    qDeleteAll(widget_schedulers_);
}

CDataScheduler *CDataScheduler::GetCDataScheduler()
{
    if (!data_scheduler_)
        data_scheduler_ = new CDataScheduler();
    return data_scheduler_;
}

void CDataScheduler::Destory()
{
    SAFE_DELETE(data_scheduler_);
}

void CDataScheduler::StartPlay()
{
    if (is_first_start_)
    {
        is_first_start_ = false;
        //开启线程，自动播放视频，视频时间轴作为主要时间轴
        std::thread t1(&CDataScheduler::PlayData, this);
        t1.detach();
    }
    else
    {
        read_switch_ = true;
    }
}

void CDataScheduler::StopPlay(bool exit)
{
    read_switch_ = false;
    is_exited_ = exit;
    if (is_exited_)
        is_first_start_ = true;
}

void CDataScheduler::ClearSchedulers()
{
    for (auto scheduler : widget_schedulers_.values())
    {
        scheduler->ClearWidgets();
    }
}

void CDataScheduler::PlayData()
{
    read_switch_ = true;
    auto img_data_ptr = data_center_->GetDataPtr<CImageData>();
    if (!img_data_ptr->IsEmpty())
    {
        QString img_name = img_data_ptr->FirstKey();
        double last_timestamp = 0;
        img_iterator_ = img_data_ptr->Begin(img_name);
        while (!is_exited_)
        {
            int sleep_time = 100; //线程睡眠时间
            if (read_switch_ && img_iterator_ != img_data_ptr->End(img_name))
            {
                std::lock_guard<std::mutex> it_lock(iterator_mutex_);
                last_timestamp = img_iterator_.key();
                ++img_iterator_;
                emit signal_manager_->SigSyncData(last_timestamp);
                sleep_time = (img_iterator_.key() - last_timestamp) * 1000;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        }
    }
}

void CDataScheduler::SyncData(double timestamp)
{
    widget_schedulers_["camera"]->SyncData(timestamp);
}

void CDataScheduler::HandleParserFinished(const QString &type, double timestamp)
{
    if (widget_schedulers_.contains(type))
    {
        widget_schedulers_[type]->SyncData(timestamp);
    }
}

void CDataScheduler::HandleSliderMoved(int timestamp)
{
    std::lock_guard<std::mutex> it_lock(iterator_mutex_);
    auto img_data_ptr = data_center_->GetDataPtr<CImageData>();
    img_iterator_ = img_data_ptr->LowerBound(img_data_ptr->FirstKey(), timestamp / 100.0);
    emit signal_manager_->SigSyncData(img_iterator_.key());
}
