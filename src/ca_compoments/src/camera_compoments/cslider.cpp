#include "cslider.h"

CSlider::CSlider(QWidget *parent)
    : QSlider(parent)
{
    signal_manager_ = CSignalManager::GetCSignalManager();
    setOrientation(Qt::Horizontal);
    setFocusPolicy(Qt::StrongFocus);
    setSingleStep(2);
}

void CSlider::mousePressEvent(QMouseEvent *event)
{
    QSlider::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        int duration = maximum() - minimum();
        double pos = minimum() + duration * (event->pos().x() * 1.0 / width());
        if (pos != value())
        {
            setValue((int)pos);
            emit signal_manager_->SigSliderMoved(pos);
        }
    }
}

void CSlider::keyPressEvent(QKeyEvent *event)
{
    QSlider::keyPressEvent(event);
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
    {
        emit signal_manager_->SigSliderMoved(value());
    }
}