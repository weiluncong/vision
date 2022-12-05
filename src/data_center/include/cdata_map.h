#ifndef CDATAMAP_H
#define CDATAMAP_H

#include <QMap>
#include <QDebug>
#include <mutex>
#include <iostream>
#include "cglobal_param.h"

template <class T>
class CDataMap
{
public:
    CDataMap() {}
    ~CDataMap() { SafeClear(map_); }

    const QMap<double, T> Key(const QMap<double, T> &value, const QString &defaultKey = QString()) const
    {
        return map_.key(value, defaultKey);
    }
    const QString &FirstKey() const { return map_.firstKey(); }
    const QString &LastKey() const { return map_.lastKey(); }
    QList<QString> Keys() const { return map_.keys(); }

    const T Value(const QString &key, const double &time) const
    {
        if (map_.contains(key))
        {
            if (map_[key].contains(time))
                return map_[key][time];
        }
        return T();
    }

    const T Pop(const QString &key, const double &time)
    {
        if (map_.contains(key))
        {
            if (map_[key].contains(time))
                return map_[key].take(time);
        }

        return T();
    }

    void Clear() { SafeClear(map_); }

    inline void Insert(const QString &key, const double &time, const T &data)
    {
        std::lock_guard<std::mutex> map_lock(map_mutex_);
        map_[key][time] = data;
    }

    template <typename K>
    inline void Append(const QString &key, const double &time, const K &data)
    {
        std::lock_guard<std::mutex> map_lock(map_mutex_);
        map_[key][time].append(data);
    }

    typename QMap<double, T>::iterator Begin(const QString &key) { return map_[key].begin(); }
    typename QMap<double, T>::iterator End(const QString &key) { return map_[key].end(); }

    typename QMap<double, T>::iterator LowerBound(const QString &key, const double &time)
    {
        auto iter = map_[key].lowerBound(time);
        if (iter == map_[key].end())
        {
            return --iter;
        }
        else
        {
            return iter;
        }
    }

    bool IsEmpty() const { return map_.isEmpty(); }
    bool IsEmpty(const QString &key) const { return map_[key].isEmpty(); }
    bool IsContains(const QString &key) const { return map_.contains(key); }
    bool IsContains(const QString &key, const double &time) const { return map_[key].contains(time); }
    int Size() const { return map_.size(); }
    int Size(const QString &key) const { return map_[key].size(); }

private:
    QMap<QString, QMap<double, T>> map_;
    std::mutex map_mutex_;
};

#endif // CDATAMAP_H