#include "DeviceMgr.h"
#include "common.h"

#define ENABLE_DUMP     0

#ifdef WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    std::string path = GetAppPathA() + "\\crash";
    if (_access(path.c_str(), 0) != 0) {
        _mkdir(path.c_str());
    }

    SYSTEMTIME st;
    GetLocalTime(&st);
    char filepath[MAX_PATH];
    sprintf_s(filepath, "%s/CANFDNET_%04d%02d%02d%02d%02d%02d.dmp", path.c_str(), 
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    HANDLE hDumpFile = CreateFile(filepath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
    dumpInfo.ExceptionPointers = pException;
    dumpInfo.ThreadId = GetCurrentThreadId();
    dumpInfo.ClientPointers = TRUE;
    BOOL ret = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal,
        &dumpInfo, NULL, NULL);
    CloseHandle(hDumpFile);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

union DeviceHandle
{
    DeviceHandle(UINT devType, UINT devIdx){
        d.devType = devType;
        d.devIdx = devIdx;
    };
    struct 
    {
        UINT devType:16;
        UINT devIdx:16;
    }d;
    UINT hdl;
};


DeviceMgr *TheDeviceMgr()
{
    static DeviceMgr inst;
    return &inst;
}

DeviceMgr::DeviceMgr()
{
    cc_time_begin_period(1);

#ifdef _WIN32
#if ENABLE_DUMP
        SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
#endif
#endif
}

DeviceMgr::~DeviceMgr()
{
    // Global object exit may occur crash when close device!
    // ClearDevice();
    
    cc_time_end_period(1);
}

Device_PTR DeviceMgr::CreateDevice(UINT type, UINT idx, UINT Reserved /* = 0*/)
{
    Device_PTR dev = FindDevice(type, idx);
    if (!dev) {
        std::lock_guard<std::recursive_mutex> guard_(m_mutex);
        dev = std::make_shared<Device>(type, idx, Reserved);
        DeviceHandle handle(type, idx);
        m_devices[handle.hdl] = dev;
    }

    return dev;
}

Device_PTR DeviceMgr::FindDevice(UINT type, UINT idx)
{
    DeviceHandle handle(type, idx);
    std::lock_guard<std::recursive_mutex> guard_(m_mutex);
    if (m_devices.find(handle.hdl) != m_devices.end()) {
        return m_devices[handle.hdl];
    }

    return nullptr;
}

bool DeviceMgr::DeleteDevice(UINT type, UINT idx)
{
    std::lock_guard<std::recursive_mutex> guard_(m_mutex);
    Device_PTR dev = FindDevice(type, idx);
    if (!dev) {
        return false;
    }

    DeviceHandle handle(type, idx);
    m_devices.erase(handle.hdl);
    return true;
}

void DeviceMgr::ClearDevice()
{
    std::lock_guard<std::recursive_mutex> guard_(m_mutex);
    m_devices.clear();
}
