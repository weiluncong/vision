#ifndef CPOINTRECORD_H
#define CPOINTRECORD_H

#include <QObject>
#include <mutex>
#include <iostream>
#include "csignal_manager.h"
#include "cdata_record.h"


class CDataPointRecord;
/** @brief point record class worker*/
class CPointRecord : public QObject
{
    Q_OBJECT

public:
    CPointRecord(CDataPointRecord *point_record);
    ~CPointRecord();
    void DoRecord(const QString &file_name);
    void WaitDataBufUpdate();

private:
    CDataPointRecord *point_record_ = nullptr;
    int data_buf_size_ = 0;
    std::mutex point_record_mutex_;
    std::condition_variable condition_var_;

signals:
    void SigPointRecordOver();
};
    
#endif // CPOINTRECORD_H
