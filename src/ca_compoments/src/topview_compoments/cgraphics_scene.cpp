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
    const QPointF cur_pos = back_item->PointSceneInItem(event->scenePos());
    double x_axis_min = back_item->x_axis_min_;
    double x_axis_max = back_item->x_axis_max_;
    double y_axis_min = back_item->y_axis_min_;
    double y_axis_max = back_item->y_axis_max_;
    const double factor = 1.05;
    const double x_central = cur_pos.x();
    const double y_central = cur_pos.y();
    double bottom_offset = 0, top_offset = 0, left_offset = 0, right_offset = 0;
    if (event->delta() > 0)
    {
        bottom_offset = 1.0 / factor * (y_central - y_axis_min);
        top_offset = 1.0 / factor * (y_axis_max - y_central);
        left_offset = 1.0 / factor * (x_central - x_axis_min);
        right_offset = 1.0 / factor * (x_axis_max - x_central);
    }
    else
    {
        bottom_offset = 1.0 * factor * (y_central - y_axis_min);
        top_offset = 1.0 * factor * (y_axis_max - y_central);
        left_offset = 1.0 * factor * (x_central - x_axis_min);
        right_offset = 1.0 * factor * (x_axis_max - x_central);
    }
    switch (zoom_flag_)
    {
    case cZoomMode::ZoomX:
        back_item->x_axis_min_ = x_central - left_offset;
        back_item->x_axis_max_ = x_central + right_offset;
        this->update();
        break;
    case cZoomMode::ZoomY:
        back_item->y_axis_min_ = y_central - bottom_offset;
        back_item->y_axis_max_ = y_central + top_offset;
        this->update();
        break;
    case cZoomMode::ZoomXY:
        back_item->x_axis_min_ = x_central - left_offset;
        back_item->x_axis_max_ = x_central + right_offset;
        back_item->y_axis_min_ = y_central - bottom_offset;
        back_item->y_axis_max_ = y_central + top_offset;
        this->update();
        break;
    default:
        break;
    }
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
    CVehicleBackGroundItem *back_item = static_cast<CVehicleBackGroundItem *>(this->items().back());
    const QPointF cur_pos = event->scenePos();
    QPointF cur_val = back_item->PointSceneInItem(cur_pos);
    if (is_press_)
    {
        QPointF last_val = back_item->PointSceneInItem(last_point_);
        last_point_ = cur_pos;
        back_item->x_axis_min_ -= (cur_val - last_val).x() * 0.6;
        back_item->x_axis_max_ -= (cur_val - last_val).x() * 0.6;
        back_item->y_axis_min_ -= (cur_val - last_val).y() * 0.6;
        back_item->y_axis_max_ -= (cur_val - last_val).y() * 0.6;
        this->update();
    }

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