#ifndef CCAMERASCHEDULER_H
#define CCAMERASCHEDULER_H

#include <QObject>
#include "cabstract_scheduler.h"
#include "ccamera_widget.h"
#include "cdata_center.h"
#include "cglobal_param.h"

class CCameraScheduler : public CAbstractScheduler
{
public:
    explicit CCameraScheduler();
    ~CCameraScheduler() {}
    CCameraScheduler(const CCameraScheduler &) = delete;
    CCameraScheduler &operator=(const CCameraScheduler &) = delete;

    void AddCameraWidget(CCameraWidget *camera);
    void SyncData(double timestamp) override;
    void ClearWidgets() override;

public slots:
    void HandleActCameraClose();

private:
    QStringList camera_list_;
    QVector<CCameraWidget *> camera_widget_vec_;
};

#endif // CCAMERASCHEDULER_H
