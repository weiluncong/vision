#include "log.h"
#include <string>
#include "common.h"

CLog* TheLog()
{
    static CLog _inst;
    return &_inst;
}

TimeCalc* TheTimeCal()
{
    static TimeCalc _inst;
    return &_inst;
}

CLog::CLog()
{
    m_filepath = GetAppPathA() + "\\CANFDNET.log";
    //使用 "r+"方式打开日志，日志文件不存在则打开失败，此时不保存到文件
    //需要查看日志的时候，在dll同级目录下建立文件，就会写入日志
    //使用 "a+"方式打开日志，日志文件不存在则创建文件，文件存在则在尾部添加新数据
    //m_fp = fopen(m_filepath.c_str(), "r+");
    m_fp = fopen(m_filepath.c_str(), "r+");

    if (!m_fp)
    {
        Write(0, "Log file[%s] open failed!", m_filepath.c_str());
        //OutputDebugStringA("[Err] Log file[cloud.log] open failed! \n");
    }
    else
    {
        Write(0, "Write to file[%s]", m_filepath.c_str());
        Write(0, "================= Logging =================");
    }

}

CLog::~CLog()
{
    if (m_fp) {
        fflush(m_fp);
        fclose(m_fp);
    }
}

void CLog::Write(int level, const char* fmt, ...)
{
#ifdef WIN32
    char buftime[100] = { 0 };
    char buf[2048] = { 0 };
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(buftime, "[%04d-%02d-%02d %02d:%02d:%02d.%03d] ",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    va_list args;
    va_start(args, fmt);
    _vsnprintf_s(buf, sizeof(buf) - 4, fmt, args);
    buf[sizeof(buf) - 1] = 0;         // 如果信息太长最后显示...
    buf[sizeof(buf) - 2] = '.';
    buf[sizeof(buf) - 3] = '.';
    buf[sizeof(buf) - 4] = '.';
    va_end(args);

    //打印到调试
    OutputDebugStringA(std::string(std::string(buftime) + std::string(buf) + "\r").c_str());

    //打印到文件
    if (!m_fp) {
        return;
    }

    // 限制文件大小
    const long limit = 50 * 1000 * 1000;
    fseek(m_fp, 0, SEEK_END);
    long len = ftell(m_fp);
    if (len > limit) {
        fclose(m_fp);
        m_fp = fopen(m_filepath.c_str(), "w");
        if (!m_fp) {
            OutputDebugStringA("[Err] Clear log file failed! \n");
        }
    }

    fprintf(m_fp, buftime);
    if (level == 1)
    {
        fprintf(m_fp, "[ERR] ");
    }

    fprintf(m_fp, buf);
    fprintf(m_fp, "\n");

    fflush(m_fp);
#endif
}

void CLog::Write(char* str)
{
#ifdef WIN32
    //打印到文件
    if (!m_fp) {
        return;
    }

    // 限制文件大小
    const long limit = 50 * 1000 * 1000;
    fseek(m_fp, 0, SEEK_END);
    long len = ftell(m_fp);
    if (len > limit) {
        fclose(m_fp);
        m_fp = fopen(m_filepath.c_str(), "w");
        if (!m_fp) {
            OutputDebugStringA("[Err] Clear log file failed! \n");
			return;
        }
    }

    fprintf(m_fp, str);
    //fprintf(m_fp, "\n");

    fflush(m_fp);
#endif
}
