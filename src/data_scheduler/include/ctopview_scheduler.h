#ifndef CTOPVIEW_SCHEDULER_H
#define CTOPVIEW_SCHEDULER_H

#include "cabstract_scheduler.h"
#include "cvehicle_topview_widget.h"

class CTopViewScheduler : public CAbstractScheduler
{
public:
    explicit CTopViewScheduler() {}
    ~CTopViewScheduler() override { ClearWidgets(); }

    void AddTopViewWidget(CVehicleTopViewWidget *vehicle_widget);
    void SyncData(double timestamp) override;

    void ClearWidgets() override;
    bool GetVehicleWidgetStatus() { return (vehicle_topview_widget_ != nullptr); }

private:
    template <typename T>
    inline void UpdateItem(double timestamp);

private:
    CVehicleTopViewWidget *vehicle_topview_widget_ = nullptr;
};

template <typename T>
inline void CTopViewScheduler::UpdateItem(double timestamp)
{
    auto datas = data_center_->GetDataPtr<T>();
    if (!datas)
        return;

    auto params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
    for (auto topic_name : datas->Keys())
    {
        if (!params_map.contains(topic_name))
        {
            vehicle_topview_widget_->AddSetterItem(topic_name);
            params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
        }
        if (params_map[topic_name].check_status_)
        {
            double t_temp = timestamp;
            auto item_data = data_center_->GetValue<T>(topic_name, t_temp);
            if (!item_data.isEmpty())
                vehicle_topview_widget_->UpdateItemData(topic_name, abs(t_temp - timestamp), item_data, params_map[topic_name].color_);
        }
    }
}

#endif
