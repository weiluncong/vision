#ifndef CLOADDATA_H
#define CLOADDATA_H

#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <thread>
#include <memory>
#include "malloc.h"
#include "cglobal_param.h"
#include "cyaml_handler.h"
#include "cproto_pool.h"
#include "dat_struct.h"
#include "csignal_filter.h"
#include "cparser_manager.h"
#include "ktime.h"

using namespace cav;

class CLoadData : public QObject
{
    Q_OBJECT
public:
    CLoadData(const QString &file_path);
    ~CLoadData();

    void Dat2Replay();

private:
    void ReplayGetData(const QString &filter);
    void Dat2Csv();

private:
    CProtoPool *proto_pool_;
    CParserManager *parser_manager_;
    QList<cReplayData> data2parse_;
    QString file_path_;
    std::ifstream ifs_;
};

#endif // CLOADDATA_H