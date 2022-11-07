#ifndef CSLIDER_H
#define CSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QKeyEvent>
#include "csignal_manager.h"

class CSlider : public QSlider
{
    Q_OBJECT
public:
    explicit CSlider(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    CSignalManager *signal_manager_;
};

#endif // KSLIDER_H
