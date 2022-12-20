#include "cpoint_set_item.h"

CPointSetItem::CPointSetItem(CVehicleBackGroundItem *parent_item)
    : parent_item_(parent_item)
{
    setParentItem(parent_item_);
}

QRectF CPointSetItem::boundingRect() const
{
    return parent_item_->boundingRect();
}

void CPointSetItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(color_, 5));
    for (auto point : point_datas_)
    {
        QPointF p = parent_item_->PointItemInScene(QPointF(point.y_, point.x_));
        painter->drawPoint(p);
    }
}

void CPointSetItem::QuickSort(int left, int right, QVector<CPointData> &arr)
{
    if (left >= right)
        return;
    CPointData base, temp;
    int i = left, j = right;
    base = arr[left];
    while (i < j)
    {
        while (arr[j].y_ >= base.y_ && i < j)
            j--;
        while (arr[i].y_ <= base.y_ && i < j)
            i++;
        if (i < j)
        {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    arr[left] = arr[i];
    arr[i] = base;
    QuickSort(left, i - 1, arr);
    QuickSort(i + 1, right, arr);
}