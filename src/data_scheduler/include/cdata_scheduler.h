#ifndef CDATASCHEDULER_H
#define CDATASCHEDULER_H

#include <QObject>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include "cdata_center.h"
#include "csignal_manager.h"
#include "cabstract_scheduler.h"
#include "ccamera_scheduler.h"
#include "ctopview_scheduler.h"

class CDataScheduler : public QObject
{
    Q_OBJECT
private:
    explicit CDataScheduler();
    ~CDataScheduler();
    CDataScheduler(const CDataScheduler &) = delete;
    CDataScheduler &operator=(const CDataScheduler &) = delete;

public:
    static CDataScheduler *GetCDataScheduler();
    static void Destory();
    void StartPlay();
    void StopPlay(bool exit = false);
    void ClearSchedulers();

public:
    QMap<QString, CAbstractScheduler *> widget_schedulers_;

public slots:
    void HandleParserFinished(const QString &type, double timestamp);
    void HandleSliderMoved(int timestamp);

private:
    void PlayData();
    void SyncData(double timestamp);

private:
    bool is_first_start_ = true;
    bool read_switch_ = false;
    bool is_exited_ = false;

    static CDataScheduler *data_scheduler_;
    QMap<double, CImageData>::iterator img_iterator_;
    std::mutex iterator_mutex_;
    std::condition_variable iterator_condition_var_;
    CDataCenter *data_center_;
    CSignalManager *signal_manager_;
};

#endif // CDATASCHEDULER_H
