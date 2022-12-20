#ifndef CGRAPHICS_SCENE_H
#define CGRAPHICS_SCENE_H

#include <QtWidgets>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include "cvehicle_background_item.h"

enum cZoomMode
{
    ZoomX,
    ZoomY,
    ZoomXY
};

class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CGraphicsScene(QWidget *parent = nullptr);
    ~CGraphicsScene() { SAFE_DELETE(menu_); }
    void SetZoomMode(cZoomMode mode) { zoom_flag_ = mode; }
    void HandleActSetAxis();

protected:
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void CreateMenu();
    void CreateActions();

private:
    QMenu *menu_;
    QAction *act_origin_;
    QAction *act_one2one_;
    cZoomMode zoom_flag_ = cZoomMode::ZoomY;
    const double x_factor_ = 30;
    const double y_factor_ = 3;
    bool is_press_ = false;
    QPointF last_point_;
};

#endif // CGRAPHICS_SCENE_H