#include "common.h"
#include "log.h"

#ifdef WIN32
#include <windows.h>
#endif

void cc_time_begin_period(int ms)
{
#ifdef WIN32
	timeBeginPeriod(ms);
#endif
}

void cc_time_end_period(int ms)
{
#ifdef WIN32	
	timeEndPeriod(ms);
#endif
}

bool thread_set_priority(std::thread& thread, int prio)
{
#ifdef WIN32
    BOOL bRet = SetThreadPriority(thread.native_handle(), prio);
    LOG_INFO("set thread(id:%d) prio to : %d ret:%d\n", GetThreadId(thread.native_handle()), prio, bRet);
    return bRet > 0;
#else
    return false;
#endif
}

std::string GetHexString(const unsigned char *data, size_t len)
{
    std::string tmp;
    char by[10];
    for (size_t i = 0; i < len; i++) {
        snprintf(by, sizeof(by), "%02X ", data[i]);
        tmp += by;
    }
    return tmp;
}

std::string GetAppPathA()
{
#ifdef WIN32
    char sDrive[_MAX_DRIVE];
    char sDir[_MAX_DIR];
    char sFilename[_MAX_FNAME], Filename[_MAX_FNAME];
    char sExt[_MAX_EXT];

    GetModuleFileNameA(NULL, Filename, _MAX_PATH);
    _splitpath(Filename, sDrive, sDir, sFilename, sExt);

    std::string strRet = sDrive;
    sDir[strlen(sDir) - 1] = '\0';
    strRet += sDir;

    return strRet;
#else
    return "";
#endif
}