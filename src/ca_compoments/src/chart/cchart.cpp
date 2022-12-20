#include "cchart.h"

CChart::CChart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QChart(parent, wFlags)
{
    initChart();
}

/** @brief 初始化定义chart的轴，包括绘制轴的颜色、字体及主题*/
void CChart::initChart()
{
    this->setTheme(QChart::ChartThemeDark);
    this->legend()->setVisible(false);
    this->setMargins(QMargins(10, 0, 0, 0));

    QValueAxis *axisX = new QValueAxis();
    axisX->setLabelFormat("%.2f");
    axisX->setLabelsFont(QFont("微软雅黑", 10));
    axisX->setGridLinePen(QPen(Qt::darkGreen, 0.5, Qt::DashLine));
    axisX->setTickCount(5);
    axisX->setRange(0, 3);
    this->addAxis(axisX, Qt::AlignBottom);
    // Y轴同理，区别：Qt::AlignLeft
}
