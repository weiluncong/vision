#include "cvehicle_background_item.h"

CVehicleBackGroundItem::CVehicleBackGroundItem(QRectF rectf, cAxisMode x_mode, cAxisMode y_mode)
    : rectf_(rectf), x_mode_(x_mode), y_mode_(y_mode) {}

/** @brief 绘制自车坐标系的轴*/
void CVehicleBackGroundItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    double x_axis_val = 0, y_axis_val = 0;
    x_axis_val = (x_mode_ == cAxisMode::LeftPositive) ? x_axis_max_ : x_axis_min_;
    y_axis_val = (y_mode_ == cAxisMode::UpPositive) ? y_axis_min_ : y_axis_max_;

    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::darkGray, 2));
    painter->drawLine(PointItemInScene(QPointF(x_axis_min_, 0)), PointItemInScene(QPointF(x_axis_max_, 0)));
    painter->drawLine(PointItemInScene(QPointF(0, y_axis_min_)), PointItemInScene(QPointF(0, y_axis_max_)));
    painter->setPen(QPen(Qt::black, 2));
    painter->drawRect(boundingRect());
    /** @brief 绘制x轴*/
    int x_interval = ((x_axis_max_ - x_axis_min_) / 8.0) > 1 ? (x_axis_max_ - x_axis_min_) / 8.0 : 1;
    for (double i = x_axis_max_; i >= x_axis_min_; i -= x_interval)
    {
        i = int(i);
        painter->setPen(QPen(Qt::black, 2));
        QPointF p = PointItemInScene(QPointF(i, y_axis_val));
        painter->drawLine(p.x(), p.y(), p.x(), p.y() - 10);
        painter->drawText(p.x() - 6, p.y() + 15, QString::number(i));
        if (i < x_axis_max_)
        {
            for (int j = -3; j < 5; ++j)
            {
                if ((i + x_interval / 5.0 * j) < x_axis_max_ && (i + x_interval / 5.0 * j) > x_axis_min_)
                {
                    QPointF pm = PointItemInScene(QPointF(i + x_interval / 5.0 * j, y_axis_val));
                    painter->drawLine(pm.x(), pm.y(), pm.x(), pm.y() - 5);
                }
            }
        }
        painter->setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
        painter->drawLine(PointItemInScene(QPointF(i, y_axis_min_)), PointItemInScene(QPointF(i, y_axis_max_)));
    }

    /** @brief 绘制y轴*/
    int y_interval = ((y_axis_max_ - y_axis_min_) / 18.0) > 5 ? (y_axis_max_ - y_axis_min_) / 18.0 : 5;
    for (double i = y_axis_max_; i >= y_axis_min_; i -= y_interval)
    {
        i = int(i);
        painter->setPen(QPen(Qt::black, 2));
        QPointF p = PointItemInScene(QPointF(x_axis_val, i));
        painter->drawLine(p.x(), p.y(), p.x() + 10, p.y());
        painter->drawText(p.x() - 30, p.y() + 5, QString::number(i));
        if (i < y_axis_max_)
        {
            for (int j = -3; j < 5; ++j)
            {
                if ((i + y_interval / 5.0 * j) < y_axis_max_ && (i + y_interval / 5.0 * j) > y_axis_min_)
                {
                    QPointF pm = PointItemInScene(QPointF(x_axis_val, i + y_interval / 5.0 * j));
                    painter->drawLine(pm.x(), pm.y(), pm.x() + 5, pm.y());
                }
            }
        }
        painter->setPen(QPen(Qt::darkGray, 1, Qt::DotLine));
        painter->drawLine(PointItemInScene(QPointF(x_axis_max_, i)), PointItemInScene(QPointF(x_axis_min_, i)));
    }

    /** @brief 绘制车模*/
    painter->setPen(QPen(Qt::red, 2));
    painter->drawPixmap(QRect(PointItemInScene(QPointF(0.94, 3.78)).toPoint(), PointItemInScene(QPointF(-0.94, -1.04)).toPoint()),
                        QPixmap(":/icon/car1.png"));
}

/** @brief 将scene中的点映射到item上*/
QPointF CVehicleBackGroundItem::PointSceneInItem(const QPointF &p)
{
    double x_val = 0, y_val = 0;
    x_axis_factor_ = boundingRect().width() / (x_axis_max_ - x_axis_min_);
    y_axis_factor_ = boundingRect().height() / (y_axis_max_ - y_axis_min_);
    QPointF bottom_left_point = boundingRect().bottomLeft();

    x_val = (x_mode_ == cAxisMode::LeftPositive) ? (x_axis_max_ - (p.x() - bottom_left_point.x()) / x_axis_factor_) : (x_axis_min_ + (p.x() - bottom_left_point.x()) / x_axis_factor_);
    y_val = (y_mode_ == cAxisMode::UpPositive) ? (y_axis_min_ + (bottom_left_point.y() - p.y()) / y_axis_factor_) : (y_axis_max_ - (bottom_left_point.y() - p.y()) / y_axis_factor_);

    return QPointF(x_val, y_val);
}

/** @brief 将item上的点映射到scene上*/
QPointF CVehicleBackGroundItem::PointItemInScene(const QPointF &p)
{
    double x_val = 0, y_val = 0;
    x_axis_factor_ = boundingRect().width() / (x_axis_max_ - x_axis_min_);
    y_axis_factor_ = boundingRect().height() / (y_axis_max_ - y_axis_min_);
    QPointF bottom_left_point = boundingRect().bottomLeft();

    x_val = (x_mode_ == cAxisMode::LeftPositive) ? (bottom_left_point.x() + (x_axis_max_ - p.x()) * x_axis_factor_) : (bottom_left_point.x() + (p.x() - x_axis_min_) * x_axis_factor_);
    y_val = (y_mode_ == cAxisMode::UpPositive) ? (bottom_left_point.y() - (-y_axis_min_ + p.y()) * y_axis_factor_) : (bottom_left_point.y() - (y_axis_max_ - p.y()) * y_axis_factor_);

    return QPointF(x_val, y_val);
}