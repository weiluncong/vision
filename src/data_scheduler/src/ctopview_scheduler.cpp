#include "ctopview_scheduler.h"

void CTopViewScheduler::SyncData(double timestamp)
{
    if (!vehicle_topview_widget_)
        return;

    double tmp_time = timestamp;
    UpdateInsData(data_center_->GetValue<CPointData>("InsDataFix", tmp_time));

    tmp_time = timestamp;
    current_lane_  = data_center_->GetValue<uint64_t>("CurrentLaneId", tmp_time);

    UpdateItem<QVector<CObjectData>>(timestamp);
    UpdateItem<QVector<CLineData>>(timestamp);
    UpdateItem<QVector<CPointData>>(timestamp);
}

void CTopViewScheduler::ClearWidgets()
{
    SAFE_DELETE(vehicle_topview_widget_);
}

QMap<QString, QVector<CLineData>> CTopViewScheduler::UpdateMapLine(const QString &topic_name, QVector<CLineData> lines)
{
    QMap<QString, QVector<CLineData>>  res;
    QVector<CLineData> line_vec;
    CPointData point;
    double heading = (360.0 - ins_data_.heading_);

    for(CLineData line : lines)
    {
        QMap<unsigned long, QVector<CPointData>> tPoints;
        CLineData l = line;
        l.points_.clear();
        for (auto &point : line.points_)
        {
            transfer_.PointTransForm(point.longitude_, point.latitude_,
                                   ins_data_.longitude_, ins_data_.latitude_, heading,
                                   point.x_, point.y_);
            l.points_.push_back(point);
        }
        line_vec.push_back(l);
    }

    res[topic_name] = line_vec;
    if(topic_name.contains("Lane"))
    {
        QString current_topic = topic_name;
        current_topic = current_topic.replace("[Lane]", "[CurrentLane]");
        for(CLineData &line : line_vec)
        {
            if(line.id_ == current_lane_)
            {
                res[current_topic].push_back(line);
            }
        }
    }

    return  res;
}



void CTopViewScheduler::UpdateInsData(const CPointData &ins_data)
{
    if ((ins_data.latitude_ != 0.00000 || ins_data.longitude_ != 0.0000)
       || ins_data.heading_ != 0.000)
    {
        ins_data_ = ins_data;
    }
}


CTopViewScheduler::CTopViewScheduler()
{
    InsertCallBack<QVector<CLineData>>("map_static_info-idmap.StaticIDMapInfo[Lane]",
                   &CTopViewScheduler::UpdateMapLine);
    InsertCallBack<QVector<CLineData>>("map_static_info-idmap.StaticIDMapInfo[Line]",
                   &CTopViewScheduler::UpdateMapLine);
    InsertCallBack<QVector<CLineData>>("map_static_info-idmap.StaticIDMapInfo[Navigation]",
                   &CTopViewScheduler::UpdateMapLine);
}

void CTopViewScheduler::AddTopViewWidget(CVehicleTopViewWidget *vehicle_widget)
{
    vehicle_topview_widget_ = vehicle_widget;
    connect(vehicle_widget, &CVehicleTopViewWidget::SigVehicleTopViewClosed,
            [this]() { this->vehicle_topview_widget_ = nullptr; });
}
