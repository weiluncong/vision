#include "cgraphics_view.h"

CGraphicsView::CGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setCursor(Qt::PointingHandCursor);
}

CGraphicsView::~CGraphicsView() {}

/** @brief 鼠标点击事件*/
void CGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
}

/** @brief 视图放大事件*/
void CGraphicsView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    static double w = this->scene()->width();
    static double h = this->scene()->height();
    this->scene()->setSceneRect(-8.0 * 30 * width() / w, -120.0 * 3 * height() / h, width(), height());
    CVehicleBackGroundItem *item = static_cast<CVehicleBackGroundItem *>(this->scene()->items().back());
    if (item)
        item->rectf_ = QRectF(-8.0 * 30 * width() / w + 50, -120.0 * 3 * height() / h + 20, width() - 80, height() - 50);
    this->scene()->update();
}