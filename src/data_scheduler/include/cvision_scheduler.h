#ifndef CVISION_SHEDULER_H
#define CVISION_SHEDULER_H

#include <QObject>
#include "cabstract_scheduler.h"
#include "cvision_widget.h"
#include "cdata_center.h"

class CVisionScheduler : public CAbstractScheduler
{
public:
    explicit CVisionScheduler(){}
    ~CVisionScheduler() {}
    CVisionScheduler(const CVisionScheduler &) = delete;
    CVisionScheduler &operator=(const CVisionScheduler &) = delete;

    void AddCameraWidget(CVisionCameraWidget *camera);
    void SyncData(double timestamp) override;
    void ClearWidgets() override;

public slots:
    void HandleActCameraClose();

private:
    QVector<CVisionCameraWidget *> camera_widget_vec_;
};

#endif // CVISION_SHEDULER_H
