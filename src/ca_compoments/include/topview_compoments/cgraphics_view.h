#ifndef CGRAPHIC_VIEW_H
#define CGRAPHIC_VIEW_H

#include <QtWidgets>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include "cvehicle_background_item.h"

class CGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CGraphicsView(QWidget *parent = nullptr);
    ~CGraphicsView() override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
};

#endif // CGRAPHIC_VIEW_H