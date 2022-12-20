#ifndef CPOINT_SET_ITEM_H
#define CPOINT_SET_ITEM_H

#include <QtWidgets>
#include "cvehicle_background_item.h"
#include "cpoint_struct.h"

using namespace cav;

class CPointSetItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit CPointSetItem(CVehicleBackGroundItem *parent_item);
    ~CPointSetItem() {}
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void SetData(const QVector<CPointData> &data) { point_datas_ = data; }
    void SetName(const QString &name) { name_ = name; }
    void SetColor(const QColor &color) { color_ = color; }

private:
    void QuickSort(int left, int right, QVector<CPointData> &arr);

private:
    CVehicleBackGroundItem *parent_item_;
    QColor color_;
    QString name_;
    QVector<CPointData> point_datas_;
};

#endif // CPOINT_SET_ITEM_H