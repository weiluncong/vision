#include "cline_item.h"

CLineItem::CLineItem(CVehicleBackGroundItem *parent_item)
    : parent_item_(parent_item)
{
    setParentItem(parent_item_);
}

CLineItem::~CLineItem()
{
    SafeClear(line_points_);
}

QRectF CLineItem::boundingRect() const
{
    return parent_item_->boundingRect();
}

void CLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (line_points_.isEmpty())
        return;

    painter->setPen(QPen(color_, 3));
    painter->setRenderHint(QPainter::Antialiasing);
    if (line_data_.type_ == CLineData::SolidLine)
    {
        for (int i = 0; i < line_points_.size() - 1; i++)
        {
            QPointF start_point = parent_item_->PointItemInScene(line_points_[i]);
            QPointF end_point = parent_item_->PointItemInScene(line_points_[i + 1]);
            painter->drawLine(start_point, end_point);
        }
    }
    else if (line_data_.type_ == CLineData::DashedLine)
    {
        QPen pen = painter->pen();
        pen.setStyle(Qt::DashLine);
        painter->setPen(pen);
        for (int i = 0; i < line_points_.size() - 1; i++)
        {
            QPointF start_point = parent_item_->PointItemInScene(line_points_[i]);
            QPointF end_point = parent_item_->PointItemInScene(line_points_[i + 1]);
            painter->drawLine(start_point, end_point);
        }
    }
    else if (line_data_.type_ == CLineData::SolidDotLine)
    {
        QPainterPath path(parent_item_->PointItemInScene(line_points_[0]));
        painter->setPen(QPen(color_, 6));
        for (int i = 0; i < line_points_.size() - 1; ++i)
        {
            QPointF start_point = parent_item_->PointItemInScene(line_points_[i]);
            QPointF end_point = parent_item_->PointItemInScene(line_points_[i + 1]);
            QPointF control_point = (start_point + end_point) / 2.0;
            path.quadTo(control_point, end_point);
            painter->drawPoint(start_point);
        }
        painter->setPen(QPen(color_, 3));
        painter->drawPath(path);
    }
    else if (line_data_.type_ == CLineData::RedSolidLine)
    {
        painter->setPen(QPen(Qt::red, 2));
        for (int i = 0; i < line_points_.size() - 1; i++)
        {
            QPointF start_point = parent_item_->PointItemInScene(line_points_[i]);
            QPointF end_point = parent_item_->PointItemInScene(line_points_[i + 1]);
            painter->drawLine(start_point, end_point);
        }
    }
    else if (line_data_.type_ == CLineData::GreenSolidLine)
    {
        painter->setPen(QPen(Qt::green, 2));
        for (int i = 0; i < line_points_.size() - 1; i++)
        {
            QPointF start_point = parent_item_->PointItemInScene(line_points_[i]);
            QPointF end_point = parent_item_->PointItemInScene(line_points_[i + 1]);
            painter->drawLine(start_point, end_point);
        }
    }
    else if (line_data_.type_ == CLineData::BlueSolidLine)
    {
        painter->setPen(QPen(Qt::blue, 2));
        for (int i = 0; i < line_points_.size() - 1; i++)
        {
            QPointF start_point = parent_item_->PointItemInScene(line_points_[i]);
            QPointF end_point = parent_item_->PointItemInScene(line_points_[i + 1]);
            painter->drawLine(start_point, end_point);
        }
    }
    else
    {
        QPainterPath path(parent_item_->PointItemInScene(line_points_[0]));
        painter->setPen(QPen(color_, 6));
        for (int i = 0; i < line_points_.size() - 1; ++i)
        {
            QPointF start_point = parent_item_->PointItemInScene(line_points_[i]);
            QPointF end_point = parent_item_->PointItemInScene(line_points_[i + 1]);
            QPointF control_point = (start_point + end_point) / 2.0;
            path.quadTo(control_point, end_point);
            painter->drawPoint(start_point);
        }
        painter->setPen(QPen(color_, 3));
        painter->drawPath(path);
    }
}

void CLineItem::SetData(const CLineData &data)
{
    SafeClear(line_points_);
    line_data_ = data;
    if (line_data_.points_.isEmpty())
    {
        if (line_data_.range_ <= 10000 && abs(line_data_.start_) < 10000 && line_data_.range_ != 0 && line_data_.a0_ != 0)
        {
            for (int i = line_data_.start_; i < line_data_.end_; i += 2)
            {
                double posx = i;
                double posy = line_data_.a0_ + line_data_.a1_ * posx + line_data_.a2_ * pow(posx, 2) + line_data_.a3_ * pow(posx, 3);
                line_points_.append(QPointF(posy, posx));
            }
            double posx = line_data_.end_;
            double posy = line_data_.a0_ + line_data_.a1_ * posx + line_data_.a2_ * pow(posx, 2) + line_data_.a3_ * pow(posx, 3);
            line_points_.append(QPointF(posy, posx));
        }
    }
    else
    {
        for (auto i : line_data_.points_)
        {
            line_points_.append(QPointF(i.y_, i.x_));
        }
    }
}
