#ifndef CDATACENTER_H
#define CDATACENTER_H

#include <QObject>
#include <google/protobuf/dynamic_message.h>
#include "cglobal_param.h"
#include "cdata_map.h"
#include "camera_struct.h"
#include "dat_struct.h"

using namespace cav;

class CDataCenter : public QObject
{
    Q_OBJECT
private:
    explicit CDataCenter() { InitDataMap(); }
    ~CDataCenter() {}
    CDataCenter(const CDataCenter &) = delete;
    CDataCenter &operator=(const CDataCenter &) = delete;
    void InitDataMap();

private:
    static CDataCenter *data_center_;

public:
    static CDataCenter *GetCDataCenter();
    static void Destory();
    void ClearAllData();

public:
    CDataMap<CImageData> *img_datas_;
    CDataMap<google::protobuf::Message *> *middle_datas_;

    // data record
    std::mutex record_mutex_;
    std::multimap<double, std::string> data_buf_;

    double data_start_time_ = 0;
    double data_end_time_ = 0;
};

#endif // CDATACENTER_H
