#include "CANFDNET.h"
#include "DeviceMgr.h"
#include "zlgcan/canhandle.h"

DEVICE_HANDLE FUNC_CALL ZCAN_OpenDevice(UINT DeviceType, UINT DeviceInd, UINT Reserved)
{
    CAN_HANDLE hDeviceHandle = INVALID_DEVICE_HANDLE;
    if (TheDeviceMgr()->CreateDevice(DeviceType, DeviceInd, Reserved))
    {
        hDeviceHandle.device_type = DeviceType;
        hDeviceHandle.device_index = DeviceInd;
        hDeviceHandle.channel_index = 0;
    }
    return (DEVICE_HANDLE)hDeviceHandle.handle;
}

UINT FUNC_CALL ZCAN_CloseDevice(DEVICE_HANDLE hDevice)
{
    CAN_HANDLE hDeviceHandle(hDevice);
    if (hDeviceHandle.IsInvalid())
    {
        return STATUS_ERR;
    }
    if (!TheDeviceMgr()->DeleteDevice(hDeviceHandle.device_type, hDeviceHandle.device_index))
    {
        return STATUS_ERR;
    }
    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_GetDeviceInf(DEVICE_HANDLE hDevice, ZCAN_DEVICE_INFO * pInfo)
{
    CAN_HANDLE hDeviceHandle(hDevice);
    if (hDeviceHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hDeviceHandle.device_type, hDeviceHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->GetDeviceInfo(pInfo))
    {
        return STATUS_ERR;
    }

    return STATUS_OK;
}

CHANNEL_HANDLE FUNC_CALL ZCAN_InitCAN(DEVICE_HANDLE hDevice, UINT CANInd, ZCAN_CHANNEL_INIT_CONFIG * pInitConfig)
{
    CAN_HANDLE hDeviceHandle(hDevice);
    if (hDeviceHandle.IsInvalid())
    {
        return INVALID_CHANNEL_HANDLE;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hDeviceHandle.device_type, hDeviceHandle.device_index);
    if (!dev)
    {
        return INVALID_CHANNEL_HANDLE;
    }

    hDeviceHandle.channel_index = CANInd;
    return (DEVICE_HANDLE)hDeviceHandle.handle;
}

UINT FUNC_CALL ZCAN_StartCAN(CHANNEL_HANDLE hChannel)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->StartCAN(hCANHandle.channel_index))
    {
        return STATUS_ERR;
    }

    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_ResetCAN(CHANNEL_HANDLE hChannel)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->StopCAN(hCANHandle.channel_index))
    {
        return STATUS_ERR;
    }
    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_ClearBuffer(CHANNEL_HANDLE hChannel)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev) 
    {
        return STATUS_ERR;
    }

    dev->ClearBuffer(hCANHandle.channel_index);

    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_ReadChannelErrInfo(CHANNEL_HANDLE hChannel, ZCAN_CHANNEL_ERR_INFO * pErrInfo)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->GetChnlErrorInfo(hCANHandle.channel_index, pErrInfo))
    {
        return STATUS_ERR;
    }

    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_ReadChannelStatus(CHANNEL_HANDLE hChannel, ZCAN_CHANNEL_STATUS * pCANStatus)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->GetChnlStatus(hCANHandle.channel_index, pCANStatus))
    {
        return STATUS_ERR;
    }

    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_GetReceiveNum(CHANNEL_HANDLE hChannel, BYTE type)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev) 
    {
        return STATUS_ERR;
    }

    return dev->GetReceiveNum(hCANHandle.channel_index, type);
}

UINT FUNC_CALL ZCAN_Transmit(CHANNEL_HANDLE hChannel, ZCAN_Transmit_Data * pTransmit, UINT Len)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev) 
    {
        return STATUS_ERR;
    }

    return dev->Transmit(hCANHandle.channel_index, pTransmit, Len);
}

UINT FUNC_CALL ZCAN_Receive(CHANNEL_HANDLE hChannel, ZCAN_Receive_Data * pReceive, UINT Len, int WaitTime /*= -1*/)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->Receive(hCANHandle.channel_index, pReceive, Len, WaitTime);
}

UINT FUNC_CALL ZCAN_TransmitFD(CHANNEL_HANDLE hChannel, ZCAN_TransmitFD_Data * pTransmit, UINT Len)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->TransmitFD(hCANHandle.channel_index, pTransmit, Len);
}

UINT FUNC_CALL ZCAN_ReceiveFD(CHANNEL_HANDLE hChannel, ZCAN_ReceiveFD_Data * pReceive, UINT Len, int WaitTime /*= -1*/)
{
    CAN_HANDLE hCANHandle(hChannel);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->ReceiveFD(hCANHandle.channel_index, pReceive, Len, WaitTime);
}

UINT FUNC_CALL ZCAN_TransmitData(DEVICE_HANDLE device_handle, ZCANDataObj* pTransmit, UINT Len)
{
    CAN_HANDLE hCANHandle(device_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->TransmitData(pTransmit, Len);
}

UINT FUNC_CALL ZCAN_ReceiveData(DEVICE_HANDLE device_handle, ZCANDataObj* pReceive, UINT Len, int WaitTime /*= -1*/)
{
    CAN_HANDLE hCANHandle(device_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->ReceiveData(pReceive, Len, WaitTime);
}

//获取属性接口
IProperty * FUNC_CALL GetIProperty(DEVICE_HANDLE hDevice)
{
    return nullptr;
}

UINT FUNC_CALL ReleaseIProperty(IProperty * pIProperty)
{
    return 0;
}

UINT FUNC_CALL ZCAN_GetReference(UINT DeviceType, UINT nDevIndex, UINT nChnlIndex, UINT nRefType, void* pData)
{
    Device_PTR dev = TheDeviceMgr()->FindDevice(DeviceType, nDevIndex);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->GetReference(nChnlIndex, nRefType, pData))
    {
        return STATUS_ERR;
    }

    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_SetReference(UINT DeviceType, UINT nDevIndex, UINT nChnlIndex, UINT nRefType, void* pData)
{
    Device_PTR dev = TheDeviceMgr()->FindDevice(DeviceType, nDevIndex);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->SetReference(nChnlIndex, nRefType, pData))
    {
        return STATUS_ERR;
    }

    return STATUS_OK;
}

CHANNEL_HANDLE FUNC_CALL ZCAN_InitLIN(DEVICE_HANDLE device_handle, UINT can_index, PZCAN_LIN_INIT_CONFIG pLINInitConfig)
{
    CAN_HANDLE hDeviceHandle(device_handle);
    if (hDeviceHandle.IsInvalid())
    {
        return INVALID_CHANNEL_HANDLE;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hDeviceHandle.device_type, hDeviceHandle.device_index);
    if (!dev)
    {
        return INVALID_CHANNEL_HANDLE;
    }

    if (!dev->InitLIN(can_index, pLINInitConfig))
    {
        return INVALID_CHANNEL_HANDLE;
    }

    hDeviceHandle.channel_index = can_index;
    return (DEVICE_HANDLE)hDeviceHandle.handle;
}

UINT FUNC_CALL ZCAN_StartLIN(CHANNEL_HANDLE channel_handle)
{
    CAN_HANDLE hCANHandle(channel_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->StartLIN(hCANHandle.channel_index))
    {
        return STATUS_ERR;
    }

    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_ResetLIN(CHANNEL_HANDLE channel_handle)
{
    CAN_HANDLE hCANHandle(channel_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    if (!dev->ResetLIN(hCANHandle.channel_index))
    {
        return STATUS_ERR;
    }

    return STATUS_OK;
}

UINT FUNC_CALL ZCAN_TransmitLIN(CHANNEL_HANDLE channel_handle, PZCAN_LIN_MSG pSend, UINT Len)
{
    CAN_HANDLE hCANHandle(channel_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->TransmitLIN(hCANHandle.channel_index, pSend, Len);
}

UINT FUNC_CALL ZCAN_GetLINReceiveNum(CHANNEL_HANDLE channel_handle)
{
    CAN_HANDLE hCANHandle(channel_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->GetLINReceiveNum(hCANHandle.channel_index);
}

UINT FUNC_CALL ZCAN_ReceiveLIN(CHANNEL_HANDLE channel_handle, PZCAN_LIN_MSG pReceive, UINT Len, int WaitTime)
{
    CAN_HANDLE hCANHandle(channel_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->ReceiveLIN(hCANHandle.channel_index, pReceive, Len, WaitTime);
}

UINT FUNC_CALL ZCAN_SetLINSlaveMsg(CHANNEL_HANDLE channel_handle, PZCAN_LIN_MSG pSend, UINT nMsgCount)
{
    CAN_HANDLE hCANHandle(channel_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->SetLINSlaveMsg(hCANHandle.channel_index, pSend, nMsgCount);
}

UINT FUNC_CALL ZCAN_ClearLINSlaveMsg(CHANNEL_HANDLE channel_handle, BYTE* pLINID, UINT nIDCount)
{
    CAN_HANDLE hCANHandle(channel_handle);
    if (hCANHandle.IsInvalid())
    {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hCANHandle.device_type, hCANHandle.device_index);
    if (!dev)
    {
        return STATUS_ERR;
    }

    return dev->ClearLINSlaveMsg(hCANHandle.channel_index, pLINID, nIDCount);
}

ZCAN_RET_STATUS FUNC_CALL ZCAN_UDS_Request(DEVICE_HANDLE device_handle, const ZCAN_UDS_REQUEST* req, ZCAN_UDS_RESPONSE* resp)
{
    CAN_HANDLE hDeviceHandle(device_handle);
    if (hDeviceHandle.IsInvalid()) {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hDeviceHandle.device_type, hDeviceHandle.device_index);
    if (!dev) {
        return STATUS_ERR;
    }

    return dev->UDS_Request(req, resp);
}

ZCAN_RET_STATUS FUNC_CALL ZCAN_UDS_Control(DEVICE_HANDLE device_handle, const ZCAN_UDS_CTRL_REQ *ctrl, ZCAN_UDS_CTRL_RESP* resp )
{
    CAN_HANDLE hDeviceHandle(device_handle);
    if (hDeviceHandle.IsInvalid()) {
        return STATUS_ERR;
    }

    Device_PTR dev = TheDeviceMgr()->FindDevice(hDeviceHandle.device_type, hDeviceHandle.device_index);
    if (!dev) {
        return STATUS_ERR;
    }

    return dev->UDS_Control(ctrl, resp);
}

void FUNC_CALL ZCAN_UDS_Release(DEVICE_HANDLE device_handle, ZCAN_UDS_RESPONSE* data)
{
    (void)device_handle;
    Device::UDS_Release(data);
}