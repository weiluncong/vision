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
    auto img_data_ptr = data_center_->GetDataPtr<CImageData>();
    if (camera_widget_vec_.isEmpty())
        return;

    for (auto camera : camera_widget_vec_)
    {
        QString camera_name = camera->Name();
        if (img_data_ptr->Keys().contains(camera_name))
        {
            double time = 0;
            std::vector<unsigned char> img;
            time = timestamp;
            img = data_center_->GetValue<CImageData>(camera_name, time).img;
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
