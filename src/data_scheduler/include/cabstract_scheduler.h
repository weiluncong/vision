#ifndef CABSTRACTSCHEDULER_H
#define CABSTRACTSCHEDULER_H

#include <QObject>
#include "cdata_center.h"

class CAbstractScheduler : public QObject
{
    Q_OBJECT
public:
    CAbstractScheduler()
    {
        data_center_ = CDataCenter::GetCDataCenter();
    }
    virtual ~CAbstractScheduler() {}
    virtual void ClearWidgets() = 0;
    virtual void SyncData(double timestamp) = 0;

public:
    CDataCenter *data_center_;
};

#endif //  CABSTRACTSCHEDULER_H