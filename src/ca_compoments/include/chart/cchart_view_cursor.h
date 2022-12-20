#ifndef CCHART_VIEW_CURSOR_H
#define CCHART_VIEW_CURSOR_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include "cchart_view.h"
#include "csignal_manager.h"

class CChartViewCursor : public CChartView
{
    Q_OBJECT
public:
    explicit CChartViewCursor(QChart *chart, QWidget *parent = nullptr);
    ~CChartViewCursor(){}
    void PlayCursorLine(const double &time);

    bool cursor_status_ = false;
    QLineSeries *cursor_line_ = nullptr;
    bool already_attached_ = false;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

   private:
    void AddCursorLine(const QPointF &curPos);
    CSignalManager *signal_manager_;

signals:
    void CursorMoved(const double &);
    void UpdateSignal(const double &);
};

#endif // CCHART_VIEW_CURSOR_H
