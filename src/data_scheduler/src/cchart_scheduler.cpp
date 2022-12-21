#include "cchart_scheduler.h"
#include "cgraphic_widget.h"
#include "cexplorer_box.h"

CChartScheduler::CChartScheduler()
    : CAbstractScheduler()
{
    connect(CExplorerBox::GetCExplorerBox(), &CExplorerBox::CurveSignal, this, &CChartScheduler::AddParserSignals);
}

void CChartScheduler::SyncData(double timestamp)
{
    if (FLAGS_v_online)
    {
        CDataMap<double> *char_data_map = data_center_->GetDataPtr<double>();
        if (!char_data_map)
            return;

        for (auto topic_name : char_data_map->Keys())
        {
            QMap<double, double> time_value = data_center_->GetMapValue<double>(topic_name);
            if (!time_value.isEmpty())
            {
                QString name_value = topic_name + "#" + time_value[timestamp];
                emit sendChartNameData(topic_name, time_value[timestamp], timestamp);
            }
        }
    }
    else
    {
        for (auto graphic : graphic_widget_vec_)
        {
            graphic->chart_view_cursor_->PlayCursorLine(timestamp);
            for (auto id : graphic->row_time_val_map_.keys())
            {
                auto value = graphic->row_time_val_map_[id].lowerBound(timestamp).value();
                graphic->UpdataListData(id, value);
            }
        }
    }
}

void CChartScheduler::ClearWidgets()
{
    for (auto graphic : graphic_widget_vec_)
    {
        SAFE_DELETE(graphic);
    }
    SafeClear(graphic_widget_vec_);
}

void CChartScheduler::AddParserSignals(const QString &add_signal)
{
    emit addChartItem(add_signal);
    if (!FLAGS_v_online)
    {
        if (off_parser_result_map_.contains(add_signal))
        {
            for (auto time : off_parser_result_map_[add_signal].keys())
            {
                // 组装发送数据
                emit sendChartNameData(add_signal, off_parser_result_map_[add_signal][time], time);
            }
        }
        else
        {
            QMap<double, google::protobuf::Message *> time_msg_map = data_center_->GetMapValue<google::protobuf::Message *>(add_signal.section("~", 0, 0));
            for (auto time : time_msg_map.keys())
            {
                // 解析离线新增未解析的信号
                QStringList topic_names = add_signal.split("~");
                topic_names.removeFirst();
                swc_parser_.ParseOfflineSwcData(*time_msg_map[time], topic_names, time, off_parser_result_map_[add_signal]);
                // 组装发送数据
                emit sendChartNameData(add_signal, off_parser_result_map_[add_signal][time], time);
            }
        }
    }
}

void CChartScheduler::AddGraphicWidget(CGraphicWidget *graphic_widget)
{
    connect(graphic_widget, &CGraphicWidget::SigGraphicClose, this, &CChartScheduler::HandleActGraphicClose);
    connect(this, &CChartScheduler::sendChartNameData, graphic_widget, &CGraphicWidget::AppendCurrentPoint);

    graphic_widget_vec_.push_back(graphic_widget);
}

void CChartScheduler::HandleActGraphicClose()
{
    CGraphicWidget *w = qobject_cast<CGraphicWidget *>(sender());
    for (int i = 0; i < graphic_widget_vec_.size(); ++i)
    {
        if (graphic_widget_vec_[i] == w)
        {
            graphic_widget_vec_.remove(i);
            break;
        }
    }
}
