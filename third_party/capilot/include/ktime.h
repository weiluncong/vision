/**
 * @file ktime.h
 * @author wang kuan (wangkuan@changan.com.cn)
 * @brief define platform-independent time-related class
 * @version 0.1
 * @date 2020-05-01
 *
 * @copyright Copyright (c) 2020 Changan Automobile Co., Ltd. All rights
 * reserved.
 *
 */

#ifndef KTIME_H
#define KTIME_H
#include <assert.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <vector>

struct KLocalTime {
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    int millisecond = 0;

    KLocalTime(int year = 0, int month = 0, int day = 0, int hour = 0,
               int minute = 0, int second = 0, int millisecond = 0)
        : year(year),
          month(month),
          day(day),
          hour(hour),
          minute(minute),
          second(second),
          millisecond(millisecond) {
        assert(month <= 12);
        assert(day <= 31);
        assert(hour <= 24);
        assert(minute < 60);
        assert(second < 60);
        assert(millisecond < 1000);
    }

    bool operator==(const KLocalTime &x) const {
        if (year == x.year && month == x.month && day == x.day &&
            hour == x.hour && minute == x.minute && second == x.second &&
            millisecond == x.millisecond) {
            return true;
        } else {
            return false;
        }
    }
};

std::ostream &operator<<(std::ostream &os, const KLocalTime &local);

/**
 * 自定义时间类
 */
class KTime {
public:
    KTime();

    static double getTime();
    static double getMonotonicTime();
    static std::string getStringTimeSecond();
    static std::string getStringTimeMillisecond();
    double getElapseTime();
    double getElapseCpuTime();

    static KLocalTime utc2local(double utc);
    static double local2utc(const KLocalTime &local);
    static std::string local2str(const KLocalTime &local);
    static std::string utc2str(double utc);
    static KLocalTime str2Local(const std::string &val);

private:
    double mono_time_;
    static const int oneDaySeconds = 86400;
    static const int oneHourSeconds = 3600;
    static const int oneMinuteSeconds = 60;

    timespec cpu_time_start_;
    timespec time_start_;
};

#endif