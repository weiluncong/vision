#ifndef CDATACENTER_H
#define CDATACENTER_H

#include <QObject>
#include <google/protobuf/dynamic_message.h>
#include "cglobal_param.h"
#include "cdata_map.h"
#include "ccamera_struct.h"
#include "cobj_struct.h"
// #include "cline_struct.h"
#include "cpoint_struct.h"
#include "cdat_struct.h"
#include "cvision_obj_struct.h"
#include "cprediction_struct.h"
#include "cmap_struct.h"

using namespace cav;

class CDataCenter : public QObject
{
    Q_OBJECT
private:
    explicit CDataCenter() {}
    ~CDataCenter() {}
    CDataCenter(const CDataCenter &) = delete;
    CDataCenter &operator=(const CDataCenter &) = delete;

private:
    static CDataCenter *data_center_;
    QMap<QString, void *> data_ptr_map_;

public:
    static CDataCenter *GetCDataCenter();
    static void Destory();
    void ClearAllData();

    template <class T>
    void InsertValue(const QString &topic_name, double time, const T &data);

    template <class T>
    CDataMap<T> *GetDataPtr();

    template <class T>
    T GetValue(const QString &topic_name, double &time);

public:
    // data record
    std::mutex record_mutex_;
    std::multimap<double, std::string> data_buf_;

    double data_start_time_ = 0;
    double data_end_time_ = 0;
};

template <class T>
void CDataCenter::InsertValue(const QString &topic_name, double time, const T &data)
{
    QString class_name = TOQSTR(typeid(T).name());
    if (!data_ptr_map_.contains(class_name))
    {
        CDataMap<T> *map = new CDataMap<T>();
        data_ptr_map_[class_name] = map;
    }

    CDataMap<T> *map_ptr = static_cast<CDataMap<T> *>(data_ptr_map_[class_name]);
    if (map_ptr)
        map_ptr->Insert(topic_name, time, data);
}

template <class T>
CDataMap<T> *CDataCenter::GetDataPtr()
{
    QString class_name = TOQSTR(typeid(T).name());
    if (data_ptr_map_.contains(class_name))
    {
        return static_cast<CDataMap<T> *>(data_ptr_map_[class_name]);
    }
    return nullptr;
}

template <class T>
T CDataCenter::GetValue(const QString &topic_name, double &time)
{
    QString class_name = TOQSTR(typeid(T).name());
    CDataMap<T> *map_ptr = static_cast<CDataMap<T> *>(data_ptr_map_[class_name]);
    if (!map_ptr)
    {
        return T();
    }
    else
    {
        if (FLAGS_v_online)
            return map_ptr->Pop(topic_name, time);
        else
        {
            auto it = map_ptr->LowerBound(topic_name, time);
            time = it.key();
            return it.value();
        }
    }
}

#endif // CDATACENTER_H
