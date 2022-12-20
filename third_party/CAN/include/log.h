#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <string>
#include <time.h>


#ifdef WIN32
#include <Windows.h>
class TimeCalc
{
public:
    TimeCalc()
    {
        QueryPerformanceFrequency(&freq_);
        QueryPerformanceCounter(&start_);
    }
    ~TimeCalc()
    {
    }

public:
    void Print(const char* fmt, ...)
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        double fTime = (now.QuadPart - start_.QuadPart) / double(freq_.QuadPart);
        char fmtmsg[512] = { 0 };
        char msg[1024] = { 0 };
        va_list args;
        va_start(args, fmt);
        _vsnprintf_s(fmtmsg, sizeof(fmtmsg), fmt, args);
        va_end(args);
        sprintf_s(msg, 1024, "[%.6f] %s", fTime, fmtmsg);
        OutputDebugStringA(msg);
    }

public:
    LARGE_INTEGER freq_;
    LARGE_INTEGER start_;
};
#else
class TimeCalc
{
public:
    TimeCalc()
    {
    }
    ~TimeCalc()
    {
    }

public:
    void Print(const char* fmt, ...)
    {
    }
};

#endif

class CLog
{
    friend CLog* TheLog();
private:
    CLog();
public:
    ~CLog();

    void Write(int level, const char* fmt, ...);
    void Write(char* str);

private:
    FILE *m_fp;
    std::string m_filepath;
};

CLog* TheLog();
TimeCalc* TheTimeCal();

#ifdef _DEBUG
#define ENABLE_LOG_ERR  1
#define ENABLE_LOG_INFO 0
#else
#define ENABLE_LOG_ERR  1
#define ENABLE_LOG_INFO 0
#endif

#if _MSC_VER
//WINDOWS MSVC
#include <Windows.h>

#define LOG_ALWAYS(fmt,...)  { \
    char time[50] = {0}; \
    char msg[1024] = {0}; \
    SYSTEMTIME st; \
    GetLocalTime(&st); \
    sprintf(time, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); \
    _snprintf_s(msg , 1024, "%s" __FUNCTION__ "<%d> : " fmt , time, __LINE__, ##__VA_ARGS__); \
    OutputDebugStringA(msg); \
    TheLog()->Write(msg); \
}

#if ENABLE_LOG_INFO
#define LOG_INFO(fmt,...)  { \
    char time[50] = {0}; \
    char msg[1024] = {0}; \
    SYSTEMTIME st; \
    GetLocalTime(&st); \
    sprintf(time, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); \
    _snprintf_s(msg , 1024, "[INF] %s" __FUNCTION__ "<%d> : " fmt , time, __LINE__, ##__VA_ARGS__); \
    OutputDebugStringA(msg); \
    TheLog()->Write(msg); \
}
#else //ENABLE_LOG_INFO
#define LOG_INFO(fmt,...)
#endif //ENABLE_LOG_INFO

#if ENABLE_LOG_ERR
#define LOG_ERR(fmt,...)  { \
    char time[50] = {0}; \
    char msg[1024] = {0}; \
    SYSTEMTIME st; \
    GetLocalTime(&st); \
    sprintf(time, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds); \
    _snprintf_s(msg , 1024, "[ERR] %s" __FUNCTION__ "<%d> : " fmt , time, __LINE__, ##__VA_ARGS__); \
    TheLog()->Write(msg); \
}//    OutputDebugStringA(msg); 
#else  //ENABLE_LOG_ERR
#define LOG_ERR(fmt,...)
#endif //ENABLE_LOG_ERR

#else
//linux etc
#define LOG_INFO(fmt, ...)  printf("[INF] ");printf(fmt, ##__VA_ARGS__)
#define LOG_ERR(fmt, ...)   printf("[ERR] ");printf(fmt, ##__VA_ARGS__)
#define LOG_ALWAYS(fmt,...) printf("[ALWAYS] ");printf(fmt, ##__VA_ARGS__)
#endif //_MSC_VER

#endif //LOG_H_


