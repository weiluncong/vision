#ifndef CDATA_PRODUCTER_H
#define CDATA_PRODUCTER_H

#include <QtWidgets>
#include <functional>
#include <iostream>
#include <thread>
#include "cglobal_param.h"
#include "creceive_data.h"
#include "cload_data.h"

class CDataProducer : public QObject
{
    Q_OBJECT
public:
    CDataProducer(QObject *parent = nullptr);
    ~CDataProducer();

public:
    void Switch();
    void OpenFile(const QString &file_path);
    inline void PauseReceive();
    inline void StartReceive();

private:
    CReceiveData *receive_data_ = nullptr;
    CLoadData *load_data_ = nullptr;
    CDataCenter *data_center_;
signals:
    void sigParseOver();
};

inline void CDataProducer::PauseReceive() { receive_data_->switch_flag_ = false; }
inline void CDataProducer::StartReceive() { receive_data_->switch_flag_ = true; }

#endif // CDATA_PRODUCTER_H
