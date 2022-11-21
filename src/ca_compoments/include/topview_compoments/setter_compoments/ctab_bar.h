#ifndef CTAB_BAR_H
#define CTAB_BAR_H

#include <QtWidgets>
#include <QPainter>

class CTabBar : public QTabBar
{
public:
    QSize tabSizeHint(int index) const
    {
        QSize s = QTabBar::tabSizeHint(index);
        s.transpose();
        return s;
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QStylePainter painter(this);
        QStyleOptionTab opt;

        for (int i = 0; i < count(); ++i)
        {
            initStyleOption(&opt, i);
            painter.drawControl(QStyle::CE_TabBarTabShape, opt);
            painter.save();

            QSize s = opt.rect.size();
            s.transpose();
            QRect r(QPoint(), s);
            r.moveCenter(opt.rect.center());
            opt.rect = r;

            QPoint c = tabRect(i).center();
            painter.translate(c);
            painter.rotate(90);
            painter.translate(-c);
            painter.drawControl(QStyle::CE_TabBarTabLabel, opt);
            painter.restore();
        }
    }
};

#endif // CTAB_BAR_H