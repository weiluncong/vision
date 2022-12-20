#ifndef CDATATOTALRECORD_H
#define CDATATOTALRECORD_H

#include <QtWidgets>
#include <iostream>
#include "cproto_pool.h"
#include "cdata_center.h"
#include "cdata_record.h"

class CDataTotalRecord : public QObject
{
    Q_OBJECT
public:
    CDataTotalRecord(const QString &path, QObject *parent = nullptr);
    ~CDataTotalRecord() {}

public:
    void OpenFile();
    void GetProtoContent();
    void DoRecord();
    void StartRecord();
    void ReleaseRecord();

private:
    CDataRecord *data_record_ = nullptr;
    CProtoPool *proto_pool_ = nullptr;
    CDataCenter *data_center_ = nullptr;
    std::string proto_content_;
    QString file_path_;
    bool exit_record_ = false;
};

#endif // CDATATOTALRECORD_H