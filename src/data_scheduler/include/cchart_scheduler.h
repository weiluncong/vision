#ifndef CCHART_SCHEDULER_H
#define CCHART_SCHEDULER_H

#include <QObject>
#include "cabstract_scheduler.h"
#include "cswc_parser.h"

class CGraphicWidget;


class CChartScheduler : public CAbstractScheduler
{
    Q_OBJECT
public:
    explicit CChartScheduler();
    ~CChartScheduler() {}
    CChartScheduler(const CChartScheduler &) = delete;
    CChartScheduler &operator=(const CChartScheduler &) = delete;

    void SyncData(double timestamp) override;
    void ClearWidgets() override;

    void AddGraphicWidget(CGraphicWidget *graphic_widget);
    void HandleActGraphicClose();

    void AddParserSignals(const QString &add_signal);

signals:
    void sendChartNameData(QString name, double value, double time);
    void addChartItem(const QString &item_name);


private:
    QMap<QString, QMap<double, double>> off_parser_result_map_;

    QVector<CGraphicWidget *> graphic_widget_vec_;
    CSwcParser swc_parser_;



};

#endif // CCHART_SCHEDULER_H
