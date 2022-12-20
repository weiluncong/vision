#ifndef CCHART_VIEW_H
#define CCHART_VIEW_H

#include <QObject>
#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QMouseEvent>

class CChartView : public QChartView
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override {if(event->key() == Qt::Key_Control){ctrl_press_ = false;}}

public:
    explicit CChartView(QChart *chart, QWidget *parent = nullptr);
    virtual ~CChartView(){}

protected:
    void SaveAxisRange();
    
    bool is_press_ = false;
    bool already_save_range_ = false;
    QPointF last_point_;

private:
    bool ctrl_press_ = false;
    double x_min_ = 0;
    double x_max_ = 0;

};

#endif // CCHART_VIEW_H
