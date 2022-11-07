#include "ccamera_scheduler.h"

CCameraScheduler::CCameraScheduler()
    : CAbstractScheduler()
{
}

void CCameraScheduler::AddCameraWidget(CCameraWidget *camera)
{
    connect(camera, &CCameraWidget::SigCameraClose, this, &CCameraScheduler::HandleActCameraClose);
    camera_widget_vec_.push_back(camera);
}

void CCameraScheduler::SyncData(double timestamp)
{
    if (camera_widget_vec_.isEmpty())
        return;

    for (auto camera : camera_widget_vec_)
    {
        QString camera_name = camera->Name();
        if (data_center_->img_datas_->Keys().contains(camera_name))
        {
            double time = 0;
            std::vector<unsigned char> img;
            if (FLAGS_v_online)
            {
                time = timestamp;
                img = data_center_->img_datas_->Pop(camera_name, timestamp).img;
            }
            else
            {
                auto img_iterator = data_center_->img_datas_->LowerBound(camera_name, timestamp);
                if (img_iterator != data_center_->img_datas_->End(camera_name))
                {
                    time = img_iterator.key();
                    img = img_iterator.value().img;
                }
            }
            if (img.size() > 0)
            {
                QString time_str = QDateTime::fromMSecsSinceEpoch(time * 1000).toString("yyyy-MM-dd hh:mm:ss.zzz");
                camera->UpdateView(img, time_str, time);
            }
        }
    }
}

void CCameraScheduler::ClearWidgets()
{
    for (auto camera : camera_widget_vec_)
    {
        SAFE_DELETE(camera);
    }
    SafeClear(camera_widget_vec_);
}

void CCameraScheduler::HandleActCameraClose()
{
    CCameraWidget *w = qobject_cast<CCameraWidget*>(sender());
    for (int i = 0; i < camera_widget_vec_.size(); ++i)
    {
        if (camera_widget_vec_[i] == w)
        {
            camera_widget_vec_.remove(i);
            break;
        }
    }
}
