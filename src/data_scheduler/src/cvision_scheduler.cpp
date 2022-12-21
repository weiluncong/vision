#include "cvision_scheduler.h"

void CVisionScheduler::AddCameraWidget(CVisionCameraWidget *camera)
{
    connect(camera, &CVisionCameraWidget::SigCameraClose, this, &CVisionScheduler::HandleActCameraClose);
    camera_widget_vec_.push_back(camera);
}

void CVisionScheduler::SyncData(double timestamp)
{
    if (camera_widget_vec_.isEmpty())
        return;

    double time = timestamp;
    auto img_data_ptr = data_center_->GetDataPtr<cav::CImageData>();
    auto obj_data_ptr = data_center_->GetDataPtr<QVector<cav::CObjBoxCV>>();
    for (auto camera : camera_widget_vec_)
    {
        if (obj_data_ptr) // update obj
        {
            for (auto name : obj_data_ptr->Keys())
            {
                if (name.contains("VpCameraProto.CamObjects-fc"))
                {
                    QVector<cav::CObjBoxCV> vision_obj_data = data_center_->GetValue<QVector<cav::CObjBoxCV>>(name, time);
                    if (!vision_obj_data.isEmpty())
                        camera->SetObjectView(vision_obj_data);
                    break;
                }
            }
        }

        if (img_data_ptr)
        {
            // update semantic
            for (auto name : img_data_ptr->Keys())
            {
                if (!name.contains(QString::number(camera->id_)))
                    continue;
                cav::CImageData img_data = data_center_->GetValue<cav::CImageData>(name, time);
                if (img_data.img_.empty())
                    continue;
                if (name.contains("Struct.ParsingImage-fc_panorama_semantic"))
                    camera->SetSematicData(img_data);
                else if (name.contains("Struct.ParsingImage-fc_lane_semantic"))
                    camera->SetLaneSematicData(img_data);
                else if (name.contains("Struct.RawImage-fc"))
                    camera->SetRawData(img_data);
            }

            // update img
            if (img_data_ptr->Keys().contains(camera->Name()))
            {
                QString time_str = QDateTime::fromMSecsSinceEpoch((time + data_center_->data_start_time_) * 1000).toString("yyyy-MM-dd hh:mm:ss.zzz");
                camera->UpdateView(time_str, time);
            }
        }
    }
}

void CVisionScheduler::ClearWidgets()
{
    for (auto camera : camera_widget_vec_)
    {
        SAFE_DELETE(camera);
    }
    SafeClear(camera_widget_vec_);
}

void CVisionScheduler::HandleActCameraClose()
{
    CVisionCameraWidget *w = qobject_cast<CVisionCameraWidget *>(sender());
    for (int i = 0; i < camera_widget_vec_.size(); ++i)
    {
        if (camera_widget_vec_[i] == w)
        {
            camera_widget_vec_.remove(i);
            break;
        }
    }
}
