#include "cgraphics_scene.h"

CGraphicsScene::CGraphicsScene(QWidget *parent)
    : QGraphicsScene(parent)
{
    this->setSceneRect(-8.0 * x_factor_, -120.0 * y_factor_, 16 * x_factor_, 180 * y_factor_);
    CreateActions();
    CreateMenu();
}

void CGraphicsScene::CreateActions()
{
    act_origin_ = new QAction("&x=8, y=120");
    act_origin_->setObjectName("1");
    connect(act_origin_, &QAction::triggered, this, &CGraphicsScene::HandleActSetAxis);
    act_one2one_ = new QAction("&x=8m y=8");
    act_origin_->setObjectName("2");
    connect(act_one2one_, &QAction::triggered, this, &CGraphicsScene::HandleActSetAxis);
}

void CGraphicsScene::CreateMenu()
{
    menu_ = new QMenu();
    menu_->addAction(act_origin_);
    menu_->addAction(act_one2one_);
}
/** @brief 滑轮事件*/
void CGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    CVehicleBackGroundItem *back_item = static_cast<CVehicleBackGroundItem *>(this->items().back());
}

/** @brief 鼠标释放按键事件*/
void CGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

/** @brief 鼠标点击事件*/
void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        last_point_ = event->scenePos();
        is_press_ = true;
    }
    else if (event->button() == Qt::RightButton)
    {
        menu_->exec(QCursor::pos());
    }
    QGraphicsScene::mousePressEvent(event);
}

/** @brief 鼠标移动事件*/
void CGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    const QPointF p = event->scenePos();

    QGraphicsScene::mouseMoveEvent(event);
}

void CGraphicsScene::HandleActSetAxis()
{
    CVehicleBackGroundItem *back_item = static_cast<CVehicleBackGroundItem *>(this->items().back());
    back_item->x_axis_min_ = -8;
    back_item->x_axis_max_ = 8;
    if (sender()->objectName() == "1")
    {
        back_item->y_axis_min_ = -60;
        back_item->y_axis_max_ = 120;
    }
    else
    {
        back_item->y_axis_min_ = -8;
        back_item->y_axis_max_ = 8;
    }
    this->update();
}