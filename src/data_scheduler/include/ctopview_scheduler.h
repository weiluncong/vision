#ifndef CTOPVIEW_SCHEDULER_H
#define CTOPVIEW_SCHEDULER_H

#include "cabstract_scheduler.h"
#include "cvehicle_topview_widget.h"

class CTopViewScheduler : public CAbstractScheduler
{
public:
    explicit CTopViewScheduler() {}
    ~CTopViewScheduler() override { ClearWidgets(); }

    void AddTopViewWidget(CVehicleTopViewWidget *vehicle_widget);
    void SyncData(double timestamp) override;

    void ClearWidgets() override;
    bool GetVehicleWidgetStatus() { return (vehicle_topview_widget_ != nullptr); }

private:
    void UpdateObjectItem(double timestamp);
    void UpdatePointItem(double timestamp);
    void UpadteLineItem(double timestamp);

    void UpdateMapItem(double timestamp);
    void UpdateMapLine(const QString &topic, QVector<CMapLine> map_line);

    CVehicleTopViewWidget *vehicle_topview_widget_ = nullptr;

    QMap<QString, QMap<int, QVector<CMapLine>>> current_maps_;
};

#endif
