#ifndef CLINE_ITEM_H
#define CLINE_ITEM_H

#include <QtWidgets>
#include "cvehicle_background_item.h"
#include "cline_struct.h"

using namespace cav;

class CLineItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    CLineItem(CVehicleBackGroundItem *parent_item);
    ~CLineItem() override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void SetData(const CLineData &data);
    void SetColor(const QColor &color) { color_ = color; }

private:
    void DrawSolidLine(QPainter *painter);
    void DrawDashLine(QPainter *painter);

private:
    CVehicleBackGroundItem *parent_item_;
    CLineData line_data_;
    QColor color_;
    QVector<QPointF> line_points_;
};

#endif // CLINE_ITEM_H