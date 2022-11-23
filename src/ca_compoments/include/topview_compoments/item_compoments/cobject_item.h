#ifndef COBJECT_ITEM_H
#define COBJECT_ITEM_H

#include <QtWidgets>
#include <cmath>
#include "cvehicle_background_item.h"
#include "cobj_struct.h"

using namespace cav;

class CObjectItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem);

public:
    explicit CObjectItem(CVehicleBackGroundItem *parent_item);
    ~CObjectItem() override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void SetData(const CObjectData &data) { obj_data_ = data; }
    void SetColor(const QColor &color) { color_ = color; }
    void SetName(const QString &name) { name_ = name; }
    int GetTrackId() { return obj_data_.track_id_; }

private:
    void UpdateObjectRect();
    void PainterDrawQuadrangle(QPainter *painter);
    void Rotation(double angle);
    QPointF PointRotation(const QPointF &point, const QPointF &origin, double angle);

private:
    CVehicleBackGroundItem *parent_item_;
    CObjectData obj_data_;
    QColor color_;
    QString name_;

    QPointF origin_point_;
    QPointF top_left_;
    QPointF top_right_;
    QPointF bottom_left_;
    QPointF bottom_right_;
};

#endif // COBJECT_ITEM_H