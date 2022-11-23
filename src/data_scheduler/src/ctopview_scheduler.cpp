#include "ctopview_scheduler.h"

void CTopViewScheduler::SyncData(double timestamp)
{
    if (!vehicle_topview_widget_)
        return;

    UpdateObjectItem(timestamp);
}

void CTopViewScheduler::ClearWidgets()
{
    SAFE_DELETE(vehicle_topview_widget_);
}

void CTopViewScheduler::AddTopViewWidget(CVehicleTopViewWidget *vehicle_widget)
{
    vehicle_topview_widget_ = vehicle_widget;
    connect(vehicle_widget, &CVehicleTopViewWidget::SigVehicleTopViewClosed,
            [this]() { this->vehicle_topview_widget_ = nullptr; });
}

void CTopViewScheduler::UpdateObjectItem(double timestamp)
{
    auto object_datas = data_center_->GetDataPtr<QVector<CObjectData>>();
    if (!object_datas)
        return;

    auto params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
    for (auto topic_name : object_datas->Keys())
    {
        if (!params_map.contains(topic_name))
        {
            vehicle_topview_widget_->AddSetterItem(topic_name);
            params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
        }
        if (params_map[topic_name].check_status_)
        {
            double t_temp = timestamp;
            auto objs = data_center_->GetValue<QVector<CObjectData>>(topic_name, t_temp);
            if (!objs.isEmpty())
                vehicle_topview_widget_->UpdateObjectItemData(topic_name, abs(t_temp - timestamp), objs, params_map[topic_name].color_);
        }
    }
}