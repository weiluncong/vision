#ifndef CCHART_H
#define CCHART_H

#include <QObject>
#include <QWidget>
#include <QtCharts>
#include <QChart>

class CChart : public QChart
{
    Q_OBJECT
public:
    CChart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());
    virtual ~CChart(){}
private:
    void initChart();
};

#endif // CCHART_H
