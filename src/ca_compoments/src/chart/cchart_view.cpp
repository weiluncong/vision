#include "cchart_view.h"

CChartView::CChartView(QChart *chart, QWidget *parent)
    : QChartView(chart, parent)
{
    /*设置鼠标指针为手指形状*/
    setCursor(Qt::PointingHandCursor);
    setRenderHint(QPainter::Antialiasing);
}

/** @brief 鼠标点击事件，判断鼠标左键是否一直按住*/
void CChartView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        last_point_ = event->pos();
        is_press_ = true;
    }
}

/** @brief 鼠标移动事件，随着鼠标的移动，图移动坐标轴*/
void CChartView::mouseMoveEvent(QMouseEvent *event)
{
    /*鼠标点在chartview图层的坐标点*/
    const QPointF curPos = event->pos();
    /*获取坐标点转换到chart坐标轴上的位置*/
    QPointF curVal = chart()->mapToValue(curPos);
    qDebug()  << "???!?!?!?!!?";
    if(is_press_)
    {
        QPointF offset = curVal - chart()->mapToValue(last_point_);
        last_point_ = curPos;
        if(!already_save_range_)
        {
            SaveAxisRange();
            already_save_range_ = true;
        }
        qDebug() << "??????" << curVal << last_point_ << offset.x()*10 << offset.y()*10;
        chart()->scroll(-offset.x()*10, -offset.y()*10);
    }
}

/** @brief 鼠标释放事件，判断是否为鼠标右键释放，如果是则还原坐标轴*/
void CChartView::mouseReleaseEvent(QMouseEvent *event)
{
    is_press_ = false;
    setCursor(Qt::PointingHandCursor);
    if(event->button() == Qt::RightButton && already_save_range_)
    {
        chart()->axes(Qt::Horizontal)[0]->setRange(x_min_, x_max_);
    }
}

/**
* @brief 鼠标滑轮滚动事件，放大和缩小图
* @param factor ： 缩放比例
*/
void CChartView::wheelEvent(QWheelEvent *event)
{
    const QPointF curPos = event->pos();
    QPointF curVal = chart()->mapToValue(QPointF(curPos));
    if(!already_save_range_)
    {
        SaveAxisRange();
        already_save_range_ = true;
    }
    const double factor = 1.25;

    if(ctrl_press_) /// zoom axisY
    {
        for(auto i : chart()->axes(Qt::Vertical))
        {
            QValueAxis *axisY = static_cast<QValueAxis *>(i);
            const double currentYMin = axisY->min();
            const double currentYMax = axisY->max();
            const double yCentral = curVal.y();

            double bottomOffset;
            double topOffset;
            if(event->delta() > 0)
            {
                bottomOffset = 1.0 / factor * (yCentral - currentYMin);
                topOffset = 1.0 / factor * (currentYMax - yCentral);
            }
            else
            {
                bottomOffset = 1.0 * factor * (yCentral - currentYMin);
                topOffset = 1.0 * factor * (currentYMax - yCentral);
            }
            i->setRange(yCentral - bottomOffset, yCentral + topOffset);
        }
    }
    else /// zoom axisX
    {
        QValueAxis *axisX = static_cast<QValueAxis *>(chart()->axes(Qt::Horizontal)[0]);
        const double currentXMin = axisX->min();
        const double currentXMax = axisX->max();
        const double xCentral = curVal.x();

        double leftOffset;
        double rightOffset;
        if(event->delta() > 0)
        {
            leftOffset = 1.0 / factor * (xCentral - currentXMin);
            rightOffset = 1.0 / factor * (currentXMax - xCentral);
        }
        else
        {
            leftOffset = 1.0 * factor * (xCentral - currentXMin);
            rightOffset = 1.0 * factor * (currentXMax - xCentral);
        }
        chart()->axes(Qt::Horizontal)[0]->setRange(xCentral - leftOffset, xCentral + rightOffset);
    }
}

/** @brief 键盘点击事件，当按键分别为contrl,key_left和key_right时，进入不同的处理*/
void CChartView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
        ctrl_press_ = true;
    }
    else if(event->key() == Qt::Key_Left)
    {
        QValueAxis *axisX = static_cast<QValueAxis*>(chart()->axes(Qt::Horizontal)[0]);
        chart()->axes(Qt::Horizontal)[0]->setRange(axisX->min() - 5, axisX->max() - 5);
    }
    else if(event->key() == Qt::Key_Right)
    {
        QValueAxis *axisX = static_cast<QValueAxis*>(chart()->axes(Qt::Horizontal)[0]);
        chart()->axes(Qt::Horizontal)[0]->setRange(axisX->min() + 5, axisX->max() + 5);
    }
}

/** @brief 保存当前坐标轴坐标，右键事件可以恢复初始坐标*/
void CChartView::SaveAxisRange()
{
    QValueAxis *axisX = static_cast<QValueAxis *>(chart()->axes(Qt::Horizontal)[0]);
    x_min_ = axisX->min();
    x_max_ = axisX->max();
}
