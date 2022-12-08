#ifndef CTOPVIEW_SCHEDULER_H
#define CTOPVIEW_SCHEDULER_H

#include "cabstract_scheduler.h"
#include "cvehicle_topview_widget.h"


class CTopViewScheduler : public CAbstractScheduler
{
public:
    explicit CTopViewScheduler();
    ~CTopViewScheduler() override { ClearWidgets(); }

    void AddTopViewWidget(CVehicleTopViewWidget *vehicle_widget);
    void SyncData(double timestamp) override;

    void ClearWidgets() override;
    bool GetVehicleWidgetStatus() { return (vehicle_topview_widget_ != nullptr); }


private:
    template <typename T>
    inline void UpdateItem(double timestamp);


    template<typename T, typename Fn>
    void InsertCallBack(const QString &topic_name, Fn func_ptr)
    {
//        typedef QMap<QString, T> (CTopViewScheduler::*CALL_BACK_FUCN) (const QString &, T);
//        CALL_BACK_FUCN func = func_ptr;
        void *p = malloc(sizeof (Fn));
        new  (p)Fn(func_ptr);
        temple_callback_[topic_name] = p;
    }


    QMap<QString, QVector<CLineData>> UpdateMapLine(const QString &,QVector<CLineData>);

    void UpdateInsData(const CPointData &ins_data);

private:
    CVehicleTopViewWidget *vehicle_topview_widget_ = nullptr;

    uint64_t current_lane_;

    CPointData ins_data_;

    CTransfer transfer_;

    QMap<QString, void *>  temple_callback_;
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
        /// 不需要进行特别处理的函数
        if(!temple_callback_.contains(topic_name))
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

            continue;
        }

        /// 需要特被处理的函数
        typedef QMap<QString, T> (CTopViewScheduler::*CALL_BACK_FUCN) (const QString &, T);
        CALL_BACK_FUCN func =  *(CALL_BACK_FUCN*)temple_callback_[topic_name];
        if(nullptr == func)
            continue;

        double t_temp = timestamp;
        auto item_data = data_center_->GetValue<T>(topic_name, t_temp);
        if (item_data.isEmpty())
            continue;

        QMap<QString, T> res = (this->*func)(topic_name, item_data);
        for(QString del_topic : res.keys())
        {
            if (!params_map.contains(del_topic))
            {
                vehicle_topview_widget_->AddSetterItem(del_topic);
                params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
            }

            if (params_map[del_topic].check_status_)
            {
                double t_temp = timestamp;
                auto item_data = res.value(del_topic);//data_center_->GetValue<T>(del_topic, t_temp);
                if (!item_data.isEmpty())
                    vehicle_topview_widget_->UpdateItemData(del_topic, abs(t_temp - timestamp), item_data, params_map[topic_name].color_);
            }
        }
    }
}

#endif
