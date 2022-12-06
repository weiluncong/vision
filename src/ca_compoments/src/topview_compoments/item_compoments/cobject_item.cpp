#include "cobject_item.h"

CObjectItem::CObjectItem(CVehicleBackGroundItem *parent_item)
    : parent_item_(parent_item)
{
    setParentItem(parent_item);
}

CObjectItem::~CObjectItem() {}

QRectF CObjectItem::boundingRect() const
{
    return parent_item_->boundingRect();
}

void CObjectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(color_, 3));
    UpdateObjectRect();

    //1,2,3,7 car;  4 person; 5 bicycle; 11 cone;
    if (obj_data_.pos_x_ != 0 || obj_data_.pos_y_ != 0)
    {
        int type = obj_data_.classification_;
        if (obj_data_.width_ == 0 && obj_data_.length_ == 0 &&
            obj_data_.track_id_ != 0 && obj_data_.track_id_ != 255)
        {
            obj_data_.heading_angle_ = 0;
            PainterDrawCircle(painter);
        }
        else if (type == 4)
        {
            PainterDrawCircle(painter);
        }
        else if (type == 5)
        {
            PainterDrawTriangle(painter);
        }
        else if (type == 11)
        {
            painter->setBrush(color_);
            PainterDrawCircle(painter);
        }
        else
        {
            PainterDrawQuadrangle(painter);
        }
    }
}

void CObjectItem::PainterDrawQuadrangle(QPainter *painter)
{
    QPainterPath path(top_left_);
    path.quadTo((top_left_ + top_right_) / 2, top_right_);
    path.quadTo((top_right_ + bottom_right_) / 2, bottom_right_);
    path.quadTo((bottom_right_ + bottom_left_) / 2, bottom_left_);
    path.quadTo((bottom_left_ + top_left_) / 2, top_left_);
    painter->drawPath(path);
    PainterDrawText(painter);
}

void CObjectItem::PainterDrawCircle(QPainter *painter)
{
    painter->drawEllipse(origin_point_.x(), origin_point_.y(), 6, 6);
    PainterDrawText(painter);
}

void CObjectItem::PainterDrawTriangle(QPainter *painter)
{
    QPolygon bicycle;
    bicycle.setPoints(3, (top_left_.x() + top_right_.x()) / 2, (top_left_.y() + top_right_.y()) / 2,
                      bottom_left_.x(), bottom_left_.y(), bottom_right_.x(), bottom_right_.y());
    painter->drawPolygon(bicycle);
    painter->drawPoint(origin_point_);
    PainterDrawText(painter);
}

void CObjectItem::PainterDrawText(QPainter *painter)
{
    painter->setPen(QPen(Qt::black, 4));
    painter->drawText(origin_point_.x() - 6, origin_point_.y() + 6, GetObjInfoText());
}

void CObjectItem::UpdateObjectRect()
{
    top_left_ = QPointF(obj_data_.pos_y_ + obj_data_.width_ / 2, obj_data_.pos_x_ + obj_data_.length_ / 2);
    top_right_ = QPointF(obj_data_.pos_y_ - obj_data_.width_ / 2, obj_data_.pos_x_ + obj_data_.length_ / 2);
    bottom_left_ = QPointF(obj_data_.pos_y_ + obj_data_.width_ / 2, obj_data_.pos_x_ - obj_data_.length_ / 2);
    bottom_right_ = QPointF(obj_data_.pos_y_ - obj_data_.width_ / 2, obj_data_.pos_x_ - obj_data_.length_ / 2);
    origin_point_ = QPointF(obj_data_.pos_y_, obj_data_.pos_x_);
    Rotation(obj_data_.heading_angle_);
    top_left_ = parent_item_->PointItemInScene(top_left_);
    top_right_ = parent_item_->PointItemInScene(top_right_);
    bottom_left_ = parent_item_->PointItemInScene(bottom_left_);
    bottom_right_ = parent_item_->PointItemInScene(bottom_right_);
    origin_point_ = parent_item_->PointItemInScene(origin_point_);
}

void CObjectItem::Rotation(double angle)
{
    top_left_ = PointRotation(top_left_, origin_point_, angle);
    top_right_ = PointRotation(top_right_, origin_point_, angle);
    bottom_left_ = PointRotation(bottom_left_, origin_point_, angle);
    bottom_right_ = PointRotation(bottom_right_, origin_point_, angle);
}

QPointF CObjectItem::PointRotation(const QPointF &point, const QPointF &origin, double angle)
{
    QPointF p;
    p.setX((point.x() - origin.x()) * cos(-angle) - (point.y() - origin.y()) * sin(-angle) + origin.x());
    p.setY((point.x() - origin.x()) * sin(-angle) + (point.y() - origin.y()) * cos(-angle) + origin.y());
    return p;
}

QString CObjectItem::GetObjInfoText() const
{
    QString obj_info = QString::number(obj_data_.track_id_);
    if (obj_data_.move_status_ != 0)
    {
        obj_info += "_";
        obj_info += QString::number(obj_data_.move_status_) ;
    }
    if (obj_data_.algorithm_type_ != 0)
    {
        if (obj_data_.move_status_ != 0)
            obj_info += "_";
        else
            obj_info += "_0_";
        obj_info += QString::number(obj_data_.algorithm_type_) ;
    }

    return obj_info;
}
