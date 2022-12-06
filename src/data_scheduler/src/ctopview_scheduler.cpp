#include "ctopview_scheduler.h"

void CTopViewScheduler::SyncData(double timestamp)
{
    if (!vehicle_topview_widget_)
        return;

    vehicle_topview_widget_->UpdateInsData(data_center_->GetValue<CPointData>("InsDataFix", timestamp));
    UpdateItem<QVector<CObjectData>>(timestamp);
    UpdateItem<QVector<CLineData>>(timestamp);
    UpdateItem<QVector<CPointData>>(timestamp);
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
