#ifndef DEVICE_MGR_H_
#define DEVICE_MGR_H_

#include "Device.h"
#include <map>
#include <mutex>

class DeviceMgr
{
    friend DeviceMgr *TheDeviceMgr();
private:
    DeviceMgr();

public:
    ~DeviceMgr();
    Device_PTR CreateDevice(UINT type, UINT idx, UINT Reserved = 0);
    bool DeleteDevice(UINT type, UINT idx);
    Device_PTR FindDevice(UINT type, UINT idx);

    void ClearDevice();

public:
    std::recursive_mutex       m_mutex;
    std::map<UINT, Device_PTR> m_devices;
};

DeviceMgr *TheDeviceMgr();


#endif


