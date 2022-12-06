#include "ctopview_scheduler.h"
#include "ctransfer.h"


void CTopViewScheduler::SyncData(double timestamp)
{
    if (!vehicle_topview_widget_)
        return;

    UpdateObjectItem(timestamp);
    UpadteLineItem(timestamp);
    UpdatePointItem(timestamp);
    UpdateMapItem(timestamp);
    vehicle_topview_widget_->UpdateInsData(
                data_center_->GetValue<CMapInsData>("InsDataFix", timestamp));

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

void CTopViewScheduler::UpdatePointItem(double timestamp)
{
    CDataMap<QVector<CPointData>> *point_datas = data_center_->GetDataPtr<QVector<CPointData>>();
    if (!point_datas)
        return;

    auto params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
    for (auto topic_name : point_datas->Keys())
    {
        if (!params_map.contains(topic_name))
        {
            vehicle_topview_widget_->AddSetterItem(topic_name);
            params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
        }
        if (params_map[topic_name].check_status_)
        {
            double t_temp = timestamp;
            auto points = data_center_->GetValue<QVector<CPointData>>(topic_name, t_temp);
            if (!points.isEmpty())
                vehicle_topview_widget_->UpdatePointItemData(topic_name, abs(t_temp - timestamp), points, params_map[topic_name].color_);
        }
    }
}

void CTopViewScheduler::UpadteLineItem(double timestamp)
{
    auto line_datas = data_center_->GetDataPtr<QVector<CLineData>>();
    if (!line_datas)
        return;

    auto params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
    for (auto topic_name : line_datas->Keys())
    {
        if (!params_map.contains(topic_name))
        {
            vehicle_topview_widget_->AddSetterItem(topic_name);
            params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
        }
        if (params_map[topic_name].check_status_)
        {
            double t_temp = timestamp;
            auto lines = data_center_->GetValue<QVector<CLineData>>(topic_name, t_temp);
            if (!lines.isEmpty())
                vehicle_topview_widget_->UpdateLineItemData(topic_name, abs(t_temp - timestamp), lines, params_map[topic_name].color_);
        }
    }
}

void CTopViewScheduler::UpdateMapItem(double timestamp)
{
    auto map_datas = data_center_->GetDataPtr<QMap<int, QVector<CMapLine>>>();
    if (!map_datas || map_datas->IsEmpty())
        return;

    auto params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
    for (auto topic_name : map_datas->Keys())
    {
        double t_temp = timestamp;
        QMap<int, QVector<CMapLine>> map = data_center_->GetValue<QMap<int, QVector<CMapLine>>>(topic_name, t_temp);
        if(std::abs(t_temp - timestamp) > 2.5)
            continue;

        if(map.size() > 0)
            current_maps_[topic_name] = map;

        QMap<int, QVector<CMapLine>> deal_line;
        QVector<CMapLine>  navigation_path;
        for(QVector<CMapLine> lines : current_maps_[topic_name].values())
        {
            for(CMapLine &line : lines)
            {
                if(line.in_navigation_route_)
                {
                    navigation_path.push_back(line);
                    continue;
                }

                deal_line[line.type_].push_back(line);
            }
        }

        for(int key : deal_line.keys())
        {
            UpdateMapLine(topic_name + "[" + QString::number(key) + "]", deal_line[key]);
        }

        UpdateMapLine(topic_name + "[Navigation]", navigation_path);
    }
}

void CTopViewScheduler::UpdateMapLine(const QString &topic_name, QVector<CMapLine> map_line)
{
    auto params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();

    if (!params_map.contains(topic_name))
    {
        vehicle_topview_widget_->AddSetterItem(topic_name);
        params_map = vehicle_topview_widget_->setter_tab_widget_->GetAllItemParam();
    }

    if (params_map[topic_name].check_status_)
    {
        if (!map_line.isEmpty())
            vehicle_topview_widget_->UpdateMapLine(topic_name, map_line, params_map[topic_name].color_);
    }
}


