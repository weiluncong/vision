#ifndef CVEHICLE_BACKGROUND_ITEM_H
#define CVEHICLE_BACKGROUND_ITEM_H

#include <QtWidgets>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include "cglobal_param.h"

enum cAxisMode
{
    LeftPositive,  // x轴左正右负
    RightPositive, // x轴左负右正
    UpPositive,    // y轴上正下负
    DownPositve    // y轴上负下正
};

class CVehicleBackGroundItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CVehicleBackGroundItem(QRectF rectf, cAxisMode x_mode = cAxisMode::LeftPositive, cAxisMode y_mode = cAxisMode::UpPositive);
    ~CVehicleBackGroundItem() {}
    QRectF boundingRect() const { return rectf_; }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    QPointF PointSceneInItem(const QPointF &p);
    QPointF PointItemInScene(const QPointF &p);

public:
    QRectF rectf_;
    double x_axis_min_ = -8;
    double x_axis_max_ = 8;
    double y_axis_min_ = -60;
    double y_axis_max_ = 120;

private:
    double x_axis_factor_;
    double y_axis_factor_;
    cAxisMode x_mode_;
    cAxisMode y_mode_;
};

#endif