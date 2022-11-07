#ifndef CDATAPOINTRECORD_H
#define CDATAPOINTRECORD_H

#include <QtWidgets>
#include <mutex>
#include <iostream>
#include "cdata_center.h"
#include "csignal_manager.h"
#include "cproto_pool.h"
#include "cpoint_record.h"

class CDataPointRecord : public QObject
{
    Q_OBJECT

public:
    CDataPointRecord(const QString &file_path, QObject *parent = nullptr);
    ~CDataPointRecord();

    void StartPointRecord();
    void GetProtoContent();

public:
    CDataCenter *data_center_ = nullptr;
    CSignalManager *signal_manager_ = nullptr;
    CProtoPool *proto_pool_ = nullptr;
    QString file_path_;
    std::string proto_content_;
    std::vector<CPointRecord *> point_recorders_;
};

#endif // CDATAPOINTRECORD_H