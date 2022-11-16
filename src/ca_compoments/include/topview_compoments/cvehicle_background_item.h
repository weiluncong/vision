#ifndef CVEHICLE_BACKGROUND_ITEM_H
#define CVEHICLE_BACKGROUND_ITEM_H

#include <QtWidgets>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include "cglobal_param.h"

class CVehicleBackGroundItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CVehicleBackGroundItem(QRectF rectf);
    ~CVehicleBackGroundItem() {}
    QRectF boundingRect() const { return rectf_; }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

public:
    QPointF PointSceneInItem(const QPointF &p) {}
    QPointF PointItemInScene(const QPointF &p) {}

public:
    QRectF rectf_;
    double x_axis_min_ = -8;
    double x_axis_max_ = 8;
    double y_axis_min_ = -60;
    double y_axis_max_ = 120;

private:
    double x_axis_factor_;
    double y_axis_factor_;
};

#endif