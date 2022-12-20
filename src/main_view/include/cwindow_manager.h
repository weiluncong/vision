#ifndef CWINDOW_MANAGER_H
#define CWINDOW_MANAGER_H

#include <QtWidgets>
#include "cglobal_param.h"
#include "cdata_producer.h"
#include "cdata_scheduler.h"
#include "cdata_total_record.h"
#include "cdata_point_record.h"

const int kMaxRecentFiles = 5;

class CMainWindow;
class CWindowManager : public QObject
{
    Q_OBJECT
public:
    CWindowManager(CMainWindow *main_window, QObject *parent = nullptr);
    ~CWindowManager();

public slots:
    void HandleActModel();
    void HandleActOpenFile();
    void HandleActSwitch();
    void HandleDataLoadFinish();
    void HandleActDataTotalRecord();
    void HandleActDataPointRecord();
    void HandleActAddCursor();
    // void HandleActAddDoubleCursors();
    void HandleActOnceDataPointRecord();
    void HandleActHideExplorerBox();

private:
    CMainWindow *main_window_ = nullptr;
    CDataProducer *data_producer_ = nullptr;
    CDataScheduler *data_scheduler_ = nullptr;
    CSignalManager *signal_manager_ = nullptr;
    CDataTotalRecord *data_total_record_ = nullptr;
    CDataPointRecord *data_point_record_ = nullptr;

    QString file_path_;
    QStringList recent_file_paths_;
    QString total_record_address_ = "./";
    QString point_record_address_ = "./";
};

#endif // CWINDOW_MANAGER_H
