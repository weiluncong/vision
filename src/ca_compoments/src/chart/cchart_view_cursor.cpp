#include "cchart_view_cursor.h"

CChartViewCursor::CChartViewCursor(QChart *chart, QWidget *parent)
    :CChartView(chart, parent)
{
    signal_manager_ = CSignalManager::GetCSignalManager();
}

/** @brief 重载鼠标点击事件，增加游标线*/
void CChartViewCursor::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        is_press_ = true;
        if(cursor_status_)
        {
            if(cursor_line_ != nullptr)
                cursor_line_->clear();
            else
            {
                cursor_line_ = new QLineSeries();
                chart()->addSeries(cursor_line_);
            }
            QPointF cur_pos = chart()->mapToValue(event->pos());
            AddCursorLine(cur_pos);
            if (cur_pos != last_point_)
            {
                emit signal_manager_->SigSliderMoved(cur_pos.x() * 100);
            }
        }
        last_point_ = event->pos();
    }
}

/** @brief 重载鼠标移动事件，移动游标线*/
void CChartViewCursor::mouseMoveEvent(QMouseEvent *event)
{
    QPointF cur_pos = chart()->mapToValue(event->pos());
    if(is_press_)
    {
        if(cursor_status_ && cursor_line_ != nullptr)
        {
            setCursor(Qt::SizeHorCursor);
            cursor_line_->clear();
            AddCursorLine(cur_pos);
            emit signal_manager_->SigSliderMoved(cur_pos.x() * 100);
        }
        else if (!cursor_status_)
        {
            QPointF off_set = chart()->mapToValue(event->pos()) - chart()->mapToValue(last_point_);
            last_point_ = event->pos();

            QValueAxis *axis_x = static_cast<QValueAxis *>(chart()->axes(Qt::Horizontal)[0]);
            double x_min = axis_x->min();
            double x_max = axis_x->max();

            chart()->axes(Qt::Horizontal)[0]->setRange(x_min - off_set.x(), x_max - off_set.x());
            //因为maptovalue只能转换第一个纵坐标信号的点，其他信号的对应点，通过比例关系换算出鼠标的移动距离
            QValueAxis *factor_axisY = static_cast<QValueAxis *>(chart()->axes(Qt::Vertical)[0]);
            double offset_y_factor = off_set.y() / (factor_axisY->max() - factor_axisY->min());
            //
            for (auto i : chart()->axes(Qt::Vertical))
            {
                QValueAxis *axisY = static_cast<QValueAxis *>(i);

                double currentYMin = axisY->min();
                double currentYMax = axisY->max();
                double vertical_offset = (currentYMax - currentYMin) * offset_y_factor;
                i->setRange(currentYMin - vertical_offset, currentYMax - vertical_offset);
            }
        }
        else
        {
            QPointF off_set = chart()->mapToValue(event->pos()) - chart()->mapToValue(last_point_);
            last_point_ = event->pos();
            if(!already_save_range_)
            {
                SaveAxisRange();
                already_save_range_ = true;
            }
            chart()->scroll(-off_set.x()*10, -off_set.y()*10);
        }
    }
}

/** @brief 添加cursorLine(包含创建线以及移动线)*/
void CChartViewCursor::AddCursorLine(const QPointF &curPos)
{
    QValueAxis *axisY = dynamic_cast<QValueAxis*>(chart()->axes(Qt::Vertical)[0]);
    const double yMin = axisY->min();
    const double yMax = axisY->max();
    cursor_line_->append(curPos.x(), yMin);
    cursor_line_->append(curPos.x(), yMax);
    if (!already_attached_)
    {
        cursor_line_->setPen(QPen(Qt::cyan, Qt::DashLine));
        cursor_line_->attachAxis(chart()->axes(Qt::Vertical)[0]);
        cursor_line_->attachAxis(chart()->axes(Qt::Horizontal)[0]);
        already_attached_ = true;
    }
}

/** @brief 当正常播放视频时，如果有游标线，则自动进行游标线的移动*/
void CChartViewCursor::PlayCursorLine(const double &time)
{
    if(cursor_status_)
    {
        if(cursor_line_ == nullptr)
        {
            cursor_line_ = new QLineSeries();
            chart()->addSeries(cursor_line_);
        }
        else
        {
            cursor_line_->clear();
        }
        AddCursorLine(QPointF(time, 0));
    }
}
