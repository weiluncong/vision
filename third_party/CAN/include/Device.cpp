#include "Device.h"
#include "common.h"
#include "log.h"
#include <memory>
#include <string.h>
#include <math.h>
#include <cassert>
#include "network.h"
#include "cJSON/cJSON.h"
#include "service/TcpClient.h"
#include "service/TcpServer.h"
#include "service/UdpService.h"

#ifdef WIN32
#include <Windows.h>
#endif


using namespace std;

const int REQUEST_TIMEOUT = 1500;           //设置命令超时时间


std::string UtilGetcJSONString(cJSON* obj)
{
    std::string result;
    if (obj)
    {
        if (cJSON_IsString(obj))
        {
            result = obj->valuestring;
        }
    }
    return result;
}

double UtilGetcJSONNumber(cJSON* obj)
{
    if (obj)
    {
        if (cJSON_IsNumber(obj))
        {
            return obj->valuedouble;
        }
    }
    return 0.0f;
}

int UtilJsonByteArray(cJSON* msgData, BYTE* pData, int nLen)
{
    int nJsonArrayLen = 0;
    if (msgData && cJSON_IsArray(msgData))
    {
        nJsonArrayLen = cJSON_GetArraySize(msgData);
        nJsonArrayLen = min(nJsonArrayLen, nLen);
        for (int i = 0; i<nJsonArrayLen && i<nLen; ++i)
        {
            pData[i] = (BYTE)UtilGetcJSONNumber(cJSON_GetArrayItem(msgData, i));
        }
    }

    return nJsonArrayLen;
}

USHORT UtilVersionString2USHORT(const char* version)
{
    //version : V1.03.09
    CANFDNetVersion stVersion;
    std::string strVersion;
    for (size_t i = 0; i < strlen(version); i++)
    {
        char v = version[i];
        if (isdigit(v) || v == '.' )
        {
            strVersion.append(1, v);
        }
    }

    std::string strArray[3];
    size_t nIndex = strVersion.find('.');
    if (nIndex != std::string::npos)
    {
        strArray[0].assign(strVersion.begin(), strVersion.begin() + nIndex);
        size_t nIndex1 = strVersion.find('.', nIndex + 1);
        if (nIndex1 != std::string::npos)
        {
            strArray[1].assign(strVersion.begin() + nIndex + 1, strVersion.begin() + nIndex1);
            strArray[2].assign(strVersion.begin() + nIndex1 + 1, strVersion.end());
        }
        else
        {
            strArray[1].assign(strVersion.begin() + nIndex1 + 1, strVersion.end());
            strArray[2] = "00";
        }
    }
    else
    {
        strArray[0] = strVersion;
        strArray[1] = "00";
        strArray[2] = "00";
    }

    stVersion.st3Version.nType = 1;
    stVersion.st3Version.nMajor = atoi(strArray[0].c_str());
    stVersion.st3Version.nMinor = atoi(strArray[1].c_str());
    stVersion.st3Version.nRevision = atoi(strArray[2].c_str());

    return stVersion.nRawVersion;
}

#include <sstream>
std::string UtilStringAppendIndex(const char* strSrc, int nIndex)
{
    std::string str(strSrc);
    std::ostringstream strStream;
    strStream << nIndex;
    return str + strStream.str();
}

static struct DeviceInfo
{
    UINT            devType;
    UINT            devChnlCount;   //CAN通道数量
    UINT            devLINCount;    //LIN通道数量
    UINT            devTCP;         //标识 tcp or udp， 1：tcp， 0：udp
    UINT            devTxEcho;      //设备是否支持发送回显，ZCANPRO中支持发送回显的会同时使用通道合并功能
    const char*     devName;
} s_devinfo[] = {
    /* DevType                   CHNL    LIN     TCP     TxEcho  Name*/
    { ZCAN_CANFDNET_TCP,         2,      0,      1,      1,      "CANFDNET-200U-TCP" },
    { ZCAN_CANFDNET_UDP,         2,      0,      0,      1,      "CANFDNET-200U-UDP" },
    { ZCAN_CANFDWIFI_TCP,        1,      0,      1,      1,      "CANFDWIFI-100U-TCP" },
    { ZCAN_CANFDWIFI_UDP,        1,      0,      0,      1,      "CANFDWIFI-100U-UDP" },
    { ZCAN_CANFDNET_400U_TCP,    4,      0,      1,      1,      "CANFDNET-400U-TCP" },
    { ZCAN_CANFDNET_400U_UDP,    4,      0,      0,      1,      "CANFDNET-400U-UDP" },
    { ZCAN_CANFDNET_100U_TCP,    1,      0,      1,      1,      "CANFDNET-100mini-TCP" },
    { ZCAN_CANFDNET_100U_UDP,    1,      0,      0,      1,      "CANFDNET-100mini-UDP" },
    { ZCAN_CANFDNET_800U_TCP,    8,      0,      1,      1,      "CANFDNET-800U-TCP" },
    { ZCAN_CANFDNET_800U_UDP,    8,      0,      0,      1,      "CANFDNET-800U-UDP" },
    { ZCAN_CANFDDTU_400_TCP,     4,      4,      1,      1,      "CANFDDTU-400-TCP" },
    { ZCAN_CANFDDTU_400_UDP,     4,      4,      0,      1,      "CANFDDTU-400-UDP" },
    { ZCAN_CANFDWIFI_200U_TCP,   2,      0,      1,      1,      "CANFDWIFI-200U-TCP" },
    { ZCAN_CANFDWIFI_200U_UDP,   2,      0,      0,      1,      "CANFDWIFI-200U-UDP" },
	{ ZCAN_CANFDDTU_800ER_TCP,   8,      4,      1,      1,      "CANFDDTU-800ER-TCP" },
	{ ZCAN_CANFDDTU_800ER_UDP,   8,      4,      0,      1,      "CANFDDTU-800ER-UDP" },
	{ ZCAN_CANFDDTU_800EWGR_TCP, 8,      4,      1,      1,      "CANFDDTU-800EWGR-TCP" },
	{ ZCAN_CANFDDTU_800EWGR_UDP, 8,      4,      0,      1,      "CANFDDTU-800EWGR-UDP" },
	{ ZCAN_CANFDDTU_600EWGR_TCP, 6,      0,      1,      1,      "CANFDDTU-600EWGR-TCP" },
	{ ZCAN_CANFDDTU_600EWGR_UDP, 6,      0,      0,      1,      "CANFDDTU-600EWGR-UDP" },
	{ ZCAN_CANFDDTU_CASCADE_TCP, 30, 0, 1, 1, "CANFDDTU_CASCADE_TCP" },
	{ ZCAN_CANFDDTU_CASCADE_UDP, 30, 0, 0, 1, "CANFDDTU_CASCADE_UDP" },
};

Device::Device(UINT devType, UINT devIdx, UINT Reserved /*= 0*/)
    : m_devType(devType), m_devIdx(devIdx), m_srcPort(Reserved), m_frmMerge(CAN_DATA_QUEUE_BUFF_SIZE * 2)
{
    LOG_INFO("Device::Device() devtype:%d devIndex:%d SrcPort:%d\n", devType, devIdx, Reserved);
    m_service = nullptr;
    m_bDevInfoUpdated = false;
    memset(&m_stDevInfo, 0, sizeof(m_stDevInfo));
    m_stDevInfo.dr_Version = 0x0100;
    m_stDevInfo.in_Version = 0x0100;
    for (size_t i = 0; i < sizeof(s_devinfo)/sizeof(s_devinfo[0]); i++)
    {
        if (devType == s_devinfo[i].devType)
        {
            m_devChnlCount = s_devinfo[i].devChnlCount;
            m_devLINCount = s_devinfo[i].devLINCount;
            m_devName = s_devinfo[i].devName;
            m_devTCP = s_devinfo[i].devTCP;

            m_stDevInfo.can_Num = s_devinfo[i].devChnlCount;
            memcpy(m_stDevInfo.str_hw_Type, m_devName.c_str(), min(m_devName.length(), (size_t)40));

            m_bRecvMerge = false;

            #ifdef WIN32
			LOG_ALWAYS("tid = %u Device::Device() devtname:%s chnl:%d TCP:%d RecvMerge:%d\n", GetCurrentThreadId(), m_devName.c_str(), m_devChnlCount, m_devTCP, m_bRecvMerge);
            #endif
            
            break;
        }
    }

    m_bitChnlStarted.reset();
    memset(m_chnlStatus, 0, sizeof(m_chnlStatus));
    memset(&m_statistic, 0, sizeof(m_statistic));
    memset(&m_busUsage, 0, sizeof(m_busUsage));
    m_bTxEcho = false;

    m_pktRequestSeq = 0;
    m_pktTID = 0;

    m_bEchoResponse = true;

#ifdef WIN32
    m_hDecoderData = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
}

Device::~Device()
{
    Stop();

#ifdef WIN32
    CloseHandle(m_hDecoderData);
    m_hDecoderData = NULL;
#endif
}

bool Device::GetReference(UINT chnIdx, UINT refType, void* pData)
{
    switch(refType)
    { 
    case CMD_DESIP:
        strcpy((char*)pData, m_destIp.c_str());
        break;
    case CMD_DESPORT:
        *(UINT*)pData = m_destPort;
        break;
    case CMD_TCP_TYPE:
        *(UINT*)pData = m_workMode;
        break;
    case CMD_SRCPORT:
        *(UINT*)pData = m_srcPort;
        break;
    case CMD_CLIENT:
        {
            REMOTE_CLIENT *pRClient = (REMOTE_CLIENT*)pData;
            TcpServer *svr = dynamic_cast<TcpServer*>(m_service);
            if (!pRClient || pRClient->iIndex < 0 
                || !svr || pRClient->iIndex >= (int)svr->GetClients()->size()) {
                return false;
            }
            const cc_socket* sck = svr->GetClient(pRClient->iIndex);
            if (!sck) {
                return false;
            }
            // Linux sizeof(HANDLE)=8, Windows sizeof(HANDLE)=4
            pRClient->hClient = 0;
            memcpy(&pRClient->hClient, &sck->handle, sizeof(sck->handle));
            pRClient->port = sck->port;
            strcpy(pRClient->szip, sck->ip);
        }
        break;
    case CMD_CLIENT_COUNT: 
        {
            TcpServer *svr = dynamic_cast<TcpServer*>(m_service);
            if (svr) {
                *(UINT*)pData = svr->GetClients()->size();
            }
            else {
                *(UINT*)pData = 0;
            }
        }
        break;
    case CMD_DISCONN_CLINET:
        return false;
    case SETGETREF_VIRIFY_DEVICE:
        {
            return true;
        }
        break;
    case SETGETREF_VIRIFY_DEVICE_BY_PASS:
        {
            VerifyDeviceData* data = (VerifyDeviceData*)pData;
            if (data)
            {
                return VerifyDeviceByPass(*data);
            }
            return false;
        }
        break;
    case SETGETREF_UDP_MULTICAST_LOOPBACK:
        *(int*)pData = m_udpmulticastloopback;
        break;
    case GETREF_GET_TX_TIMESTAMP:
        {
            if (chnIdx < m_devChnlCount && pData != NULL)
            {
                TxTimeStamp* pVal = reinterpret_cast<TxTimeStamp*>(pData);
                UINT nRetCount = GetTxEchoTimeStamp(chnIdx, *pVal);
                pVal->nBufferTimeStampCount = nRetCount;
                return false;
            }
            else
            {
                return false;
            }
        }
        break;
    case GETREF_GET_BUS_USAGE:
        {
            if (chnIdx < m_devChnlCount && pData != NULL)
            {
                BusUsage* pVal = reinterpret_cast<BusUsage*>(pData);
                return GetBusUsage(chnIdx, *pVal);
            }
            else
            {
                return false;
            }
        }
        break;
    case GETREF_GET_DELAY_SEND_AVALIABLE_COUNT:
        {
            if (chnIdx < m_devChnlCount && pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                bool bRet = GetDelaySendAvailableTxCount(chnIdx, *pVal);
                return bRet;
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_CAN_AUTO_SEND_COUNT:
        {
            if (chnIdx < m_devChnlCount && pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                bool bRet = GetDevAutoSendListCount(chnIdx, false, *pVal);
                return bRet;
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_CAN_AUTO_SEND_DATA:
        {
            if (chnIdx < m_devChnlCount && pData != NULL)
            {
                bool bRet = GetDevAutoSendListData(chnIdx, false, pData);
                return bRet;
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_CANFD_AUTO_SEND_COUNT:
        {
            if (chnIdx < m_devChnlCount && pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                bool bRet = GetDevAutoSendListCount(chnIdx, true, *pVal);
                return bRet;
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_CANFD_AUTO_SEND_DATA:
        {
            if (chnIdx < m_devChnlCount && pData != NULL)
            {
                bool bRet = GetDevAutoSendListData(chnIdx, true, pData);
                return bRet;
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_STATE_SYS_INFO:
        {
            if (pData != NULL)
            {
                ZCAN_RAW_DATA* pVal = (ZCAN_RAW_DATA*)pData;
                std::string strResult;
                if (GetDevState(PACKET_TYPE_DEV_STATE_PARAM_SYS_INFO, strResult))
                {
                    pVal->nResultLen = strResult.length() + 1;
                    memcpy(pVal->pData, strResult.c_str(), min(pVal->nResultLen, pVal->nDataLen));
                    return true;
                }
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_STATE_CAN_INFO:
        {
            if (pData != NULL)
            {
                ZCAN_RAW_DATA* pVal = (ZCAN_RAW_DATA*)pData;
                std::string strResult;
                if (GetDevState(PACKET_TYPE_DEV_STATE_PARAM_CAN_INFO, strResult))
                {
                    pVal->nResultLen = strResult.length() + 1;
                    memcpy(pVal->pData, strResult.c_str(), min(pVal->nResultLen, pVal->nDataLen));
                    return true;
                }
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_STATE_RECORDER_INFO:
        {
            if (pData != NULL)
            {
                ZCAN_RAW_DATA* pVal = (ZCAN_RAW_DATA*)pData;
                std::string strResult;
                if (GetDevState(PACKET_TYPE_DEV_STATE_PARAM_RECORDER_INFO, strResult))
                {
                    pVal->nResultLen = strResult.length() + 1;
                    memcpy(pVal->pData, strResult.c_str(), min(pVal->nResultLen, pVal->nDataLen));
                    return true;
                }
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_STATE_NET_INFO:
        {
            if (pData != NULL)
            {
                ZCAN_RAW_DATA* pVal = (ZCAN_RAW_DATA*)pData;
                std::string strResult;
                if (GetDevState(PACKET_TYPE_DEV_STATE_PARAM_NET_INFO, strResult))
                {
                    pVal->nResultLen = strResult.length() + 1;
                    memcpy(pVal->pData, strResult.c_str(), min(pVal->nResultLen, pVal->nDataLen));
                    return true;
                }
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_GPS_COUNT:
        {
            if (pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                *pVal = GetGPSRecvNum();
                return true;
            }
            return false;
        }
        break;
    case GETREF_GET_DEV_GPS_DATA:
        {
            if (pData != NULL)
            {
                ZCAN_GPS_DATA* pVal = reinterpret_cast<ZCAN_GPS_DATA*>(pData);
                pVal->nRet = GetGPSData(pVal->pData, pVal->nFrameCount, pVal->nWaitTime);
                return true;
            }
            return false;
        }
        break;
    case GETREF_GET_LIN_TX_FIFO_TOTAL:
        {
            if (chnIdx < m_devLINCount && pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                LINFIFOInfo fifoInfo;
                bool bRet = GetLINTxFIFOInfo(chnIdx, fifoInfo);
                if (bRet)
                {
                    *pVal = fifoInfo.nFIFOSize;
                }
                return bRet;
            }
            return false;
        }
        break;
    case GETREF_GET_LIN_TX_FIFO_AVAILABLE:
        {
            if (chnIdx < m_devLINCount && pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                LINFIFOInfo fifoInfo;
                bool bRet = GetLINTxFIFOInfo(chnIdx, fifoInfo);
                if (bRet)
                {
                    *pVal = fifoInfo.nFIFOAvailable;
                }
                return bRet;
            }
            return false;
        }
        break;
    case GETREF_GET_DATA_RECV_MERGE:
        {
            if (pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                *pVal = m_bRecvMerge;
                return true;
            }
            return false;
        }
        break;
    case GETREF_GET_TX_ECHO_ENABLE:
        {
            if (pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                *pVal = m_bTxEcho;
                return true;
            }
            return false;
        }
        break;
    default:
        return false;
    }

    return true;
}

bool Device::SetReference(UINT chnIdx, UINT refType, void* pData)
{
    switch(refType)
    {
    case CMD_DESIP:
        m_destIp = (char*)pData;
        LOG_ALWAYS("Set dest ip:%s\n", m_destIp.c_str());
        break;
    case CMD_DESPORT:
        m_destPort = (USHORT)*(UINT*)pData;
        LOG_ALWAYS("Set dest port:%d\n", m_destPort);
        break;
    case CMD_SRCPORT:
        m_srcPort = (USHORT)*(UINT*)pData;
        LOG_ALWAYS("Set src port:%d\n", m_srcPort);
        break;
    case CMD_TCP_TYPE:
        m_workMode = *(UINT*)pData;
        LOG_ALWAYS("Set work mode:%s\n", m_workMode == TCP_SERVER ? "Server" : "Client");
        break;
    case CMD_DISCONN_CLINET:
        {
            if (m_workMode != TCP_SERVER) {
                return false;
            }

            REMOTE_CLIENT* pClient =  (REMOTE_CLIENT*)pData;
            TcpServer *svr = dynamic_cast<TcpServer*>(m_service);
            if (!pClient || !svr) {
                return false;
            }

            // Linux sizeof(HANDLE)=8, Windows sizeof(HANDLE)=4
            int hdl = pClient->hClient;
            cc_socket_handle sckh;
            memcpy(&sckh, &hdl, sizeof(sckh));    
            return svr->DisconnectClient(sckh);
        }
        break;
    case SETGETREF_VIRIFY_DEVICE:
        {
            return true;
        }
        break;
    case SETGETREF_UDP_MULTICAST_LOOPBACK:
        m_udpmulticastloopback = *(int*)pData;
        break;
    case SETGETREF_VIRIFY_DEVICE_BY_PASS:
        {
            VerifyDeviceData* data = (VerifyDeviceData*)pData;
            if (data)
            {
                return VerifyDeviceByPass(*data);
            }
            return false;
        }
        break;
    case SETREF_ADD_TIMER_SEND_CAN_DIRECT:
        {
            if (chnIdx < m_devChnlCount && pData != nullptr)
            {
                ZCAN_AUTO_TRANSMIT_OBJ* pObj = reinterpret_cast<ZCAN_AUTO_TRANSMIT_OBJ*>(pData);
                if (pObj)
                {
                    return SetAutoSend(chnIdx, pObj);
                }
            }
            return false;
        }
        break;
    case SETREF_ADD_TIMER_SEND_CANFD_DIRECT:
        {
            if (chnIdx < m_devChnlCount && pData != nullptr)
            {
                ZCANFD_AUTO_TRANSMIT_OBJ* pObj = reinterpret_cast<ZCANFD_AUTO_TRANSMIT_OBJ*>(pData);
                if (pObj)
                {
                    return SetAutoSend(chnIdx, pObj);
                }
            }
            return false;
        }
        break;
    case SETREF_ADD_TIMER_SEND_CAN:
        {
            if (chnIdx < m_devChnlCount && pData != nullptr)
            {
                ZCAN_AUTO_TRANSMIT_OBJ* pObj = reinterpret_cast<ZCAN_AUTO_TRANSMIT_OBJ*>(pData);
                if (pObj)
                {
                    return AddAutoSend(chnIdx, pObj);
                }
            }
            return false;
        }
        break;
    case SETREF_ADD_TIMER_SEND_CANFD:
        {
            if (chnIdx < m_devChnlCount && pData != nullptr)
            {
                ZCANFD_AUTO_TRANSMIT_OBJ* pObj = reinterpret_cast<ZCANFD_AUTO_TRANSMIT_OBJ*>(pData);
                if (pObj)
                {
                    return AddAutoSend(chnIdx, pObj);
                }
            }
            return false;
        }
        break;
    case SETREF_APPLY_TIMER_SEND:
        {
            if (chnIdx < m_devChnlCount)
            {
                return ApplyAutoSend(chnIdx);
            }
            return false;
        }
        break;
    case SETREF_CLEAR_TIMER_SEND:
        {
            if (chnIdx < m_devChnlCount)
            {
                return ClearAutoSend(chnIdx);
            }
            return false;
        }
        break;
    case SETREF_SET_DEV_TIMESTAMP:
        {
            if (pData != nullptr)
            {
                UINT64 nTime = *(UINT64*)pData;
                return SetDevTimeStamp(nTime);
            }
            return false;
        }
        break;
    case SETREF_SET_TX_ECHO_ENABLE:
        {
            if (pData != NULL)
            {
                UINT* pVal = reinterpret_cast<UINT*>(pData);
                m_bTxEcho = *pVal > 0;
                return true;
            }
            return false;
        }
        break;
    case SETREF_CLEAR_DELAY_SEND_QUEUE:
        if (chnIdx < m_devChnlCount)
        {
            return ClearDelaySendQueue(chnIdx);
        }
        break;
    case SETREF_SET_DATA_RECV_MERGE:
    {
        if (pData != NULL)
        {
            UINT* pVal = reinterpret_cast<UINT*>(pData);
            m_bRecvMerge = *pVal > 0;
            return true;
        }
        return false;
    }
    break;
    default:
        return false;
    }

    return true;
}

ULONG Device::GetReceiveNum(UINT chnIdx, BYTE type)
{
    //type:TYPE_CAN TYPE_CANFD TYPE_ALL_DATA
    if (m_bRecvMerge || type == TYPE_ALL_DATA)
    {
        return m_frmMerge.size();
    }
    else if (chnIdx < m_devChnlCount)
    {
        if (type == TYPE_CANFD)
        {
            return m_frmCANFD[chnIdx].size();
        }
        else if (type == TYPE_CAN)
        {
            return m_frmCAN[chnIdx].size();
        }
    }
    return 0;
}

void Device::ClearBuffer(UINT chnIdx)
{
    if (chnIdx < m_devChnlCount)
    {
        LOG_INFO("ClearBuffer Chnl %d: CAN:%d CANFD:%d Err:%d\n", chnIdx, m_frmCAN[chnIdx].size(), m_frmCANFD[chnIdx].size(), m_errQueue[chnIdx].size());
        m_frmCAN[chnIdx].clear();
        m_frmCANFD[chnIdx].clear();
        m_errQueue[chnIdx].clear();
        memset(&m_chnlStatus[chnIdx], 0, sizeof(ZCAN_CHANNEL_STATUS));

        m_devAutoSendList.chnl[chnIdx].vecCAN.clear();
        m_devAutoSendList.chnl[chnIdx].vecCANFD.clear();
    }
}

ULONG Device::Transmit(UINT chnIdx, const ZCAN_Transmit_Data* pSend, ULONG len)
{
    ULONG sentCnt = 0;
    VecPackets vecPackets;
    if (chnIdx >= m_devChnlCount || pSend == nullptr || len == 0)
    {
        return sentCnt;
    }
    vecPackets = CPacketEncoder::BuildPacketCAN(chnIdx, pSend, len, m_bTxEcho);
    for (auto& packet : vecPackets)
    {
        if (packet.size() != SendData(packet.data(), packet.size()))
        {
            m_statistic.chnl[chnIdx].nTxCAN += sentCnt;
            return sentCnt;
        }
        sentCnt += CPacketUtil::GetPacketFrameCount(packet);
    }
    m_statistic.chnl[chnIdx].nTxCAN += sentCnt;
    return sentCnt;
}


ULONG Device::TransmitFD(UINT chnIdx, const ZCAN_TransmitFD_Data* pSend, ULONG len)
{
    ULONG sentCnt = 0;
    VecPackets vecPackets;
    if (chnIdx >= m_devChnlCount || pSend == nullptr || len == 0)
    {
        return sentCnt;
    }
	//m_bTxEcho = false;
    vecPackets = CPacketEncoder::BuildPacketCANFD(chnIdx, pSend, len, m_bTxEcho);
    for (auto& packet : vecPackets)
    {
        if (packet.size() != SendData(packet.data(), packet.size()))
        {
            m_statistic.chnl[chnIdx].nTxCANFD += sentCnt;
            return sentCnt;
        }
        sentCnt += CPacketUtil::GetPacketFrameCount(packet);
    }
    m_statistic.chnl[chnIdx].nTxCANFD += sentCnt;
    return sentCnt;
}

ULONG Device::TransmitData(const ZCANDataObj* pSend, ULONG len)
{
    ULONG sentCnt = 0;
    VecPackets vecPackets;
    if (pSend == nullptr || len == 0)
    {
        return sentCnt;
    }
    vecPackets = CPacketEncoder::BuildPacketMerge( pSend, len);
    for (auto& packet : vecPackets)
    {
        if (packet.size() != SendData(packet.data(), packet.size()))
        {
            m_statistic.nTxMerge += sentCnt;
            return sentCnt;
        }
        sentCnt += CPacketUtil::GetPacketFrameCount(packet);
    }
    m_statistic.nTxMerge += sentCnt;
    return sentCnt;
}

ULONG Device::Receive(UINT chnIdx, ZCAN_Receive_Data* pReceive, ULONG len, int waitTime/* = -1*/)
{
    if (chnIdx >= m_devChnlCount || pReceive == nullptr || len == 0)
    {
        return 0;
    }

    CanDataQueue<PacketDataCAN>& frmdata = m_frmCAN[chnIdx];

#ifdef WIN32
    frmdata.WaitEvent(waitTime);
#else
    const int per_sleep_ms = 1;
    if (waitTime > 0)
    {
        int sleepTimes = (int)ceil(waitTime / (float)per_sleep_ms);
        while (sleepTimes--)
        {
            if (frmdata.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
    else if (waitTime < 0)
    {
        while (true)
        {
            if (frmdata.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
#endif

    memset(pReceive, 0, len*sizeof(ZCAN_Receive_Data));
    frmdata.lock();
    ULONG actLen = frmdata.size() > len ? len : frmdata.size();
    for (ULONG i = 0; i < actLen; i++)
    {
        CPacketUtil::CANFrame2ZCANRx(frmdata.front(), pReceive[i], nullptr);
        frmdata.pop();
    }
    frmdata.unlock();

    //LOG_INFO("Receive Chnl:%d, count:%d\n", chnIdx, actLen);

    return actLen;
}

ULONG Device::ReceiveFD(UINT chnIdx, ZCAN_ReceiveFD_Data* pReceive, ULONG len, int waitTime/* = -1*/)
{
    if (chnIdx >= m_devChnlCount || pReceive == nullptr || len == 0)
    {
        return 0;
    }

    CanDataQueue<PacketDataCANFD>& frmdata = m_frmCANFD[chnIdx];

#ifdef WIN32
    frmdata.WaitEvent(waitTime);
#else
    const int per_sleep_ms = 1;
    if (waitTime > 0)
    {
        int sleepTimes = (int)ceil(waitTime / (float)per_sleep_ms);
        while (sleepTimes--)
        {
            if (frmdata.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
    else if (waitTime < 0)
    {
        while (true)
        {
            if (frmdata.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
#endif

    memset(pReceive, 0, len*sizeof(ZCAN_ReceiveFD_Data));

    frmdata.lock();
    ULONG actLen = frmdata.size() > len ? len : frmdata.size();
    for (ULONG i = 0; i < actLen; i++)
    {
        CPacketUtil::CANFrame2ZCANRx(frmdata.front(), pReceive[i], nullptr, false);
        frmdata.pop();
    }
    frmdata.unlock();

    //LOG_INFO("ReceiveFD Chnl:%d, count:%d\n", chnIdx, actLen);

    return actLen;
}

ULONG Device::ReceiveData(ZCANDataObj* pReceive, ULONG len, int waitTime /*= -1*/)
{
    if (pReceive == nullptr || len == 0)
    {
        return 0;
    }

    CanDataQueue<ZCANDataObj>& frmdata = m_frmMerge;

#ifdef WIN32
    frmdata.WaitEvent(waitTime);
#else
    const int per_sleep_ms = 1;
    if (waitTime > 0)
    {
        int sleepTimes = (int)ceil(waitTime / (float)per_sleep_ms);
        while (sleepTimes--)
        {
            if (frmdata.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
    else if (waitTime < 0)
    {
        while (true)
        {
            if (frmdata.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
#endif

    memset(pReceive, 0, len*sizeof(ZCANDataObj));

    frmdata.lock();
    ULONG actLen = frmdata.size() > len ? len : frmdata.size();
    for (ULONG i = 0; i < actLen; i++)
    {
        pReceive[i] = frmdata.front();
        frmdata.pop();
    }
    frmdata.unlock();

    //LOG_INFO("ReceiveData count:%d\n", actLen);
    m_statistic.nAppRxMerge += actLen;

    return actLen;
}

bool Device::GetChnlErrorInfo(UINT chnIdx, ZCAN_CHANNEL_ERR_INFO* pError)
{
    if (nullptr == pError || chnIdx >= m_devChnlCount)
    {
        return false;
    }

    memset(pError, 0, sizeof(ZCAN_CHANNEL_ERR_INFO));
    CanDataQueue<ZCAN_CHANNEL_ERR_INFO>& frmdata = m_errQueue[chnIdx];
    frmdata.lock();
    if (frmdata.size() > 0)
    {
        *pError = frmdata.front();
        frmdata.pop();
    }
    frmdata.unlock();
    return true;
}

bool Device::GetChnlStatus(UINT chnIdx, ZCAN_CHANNEL_STATUS * pCANStatus)
{
    if (pCANStatus == nullptr || chnIdx >= m_devChnlCount)
    {
        return false;
    }

    *pCANStatus = m_chnlStatus[chnIdx];
    return true;
}

bool Device::GetDeviceInfo(ZCAN_DEVICE_INFO * pInfo)
{
    if (nullptr != pInfo)
    {
        if (!m_bDevInfoUpdated)
        {
            GetDeviceInfo(m_stDevInfo);
        }

        memcpy(pInfo, &m_stDevInfo, sizeof(ZCAN_DEVICE_INFO));
        return true;
    }
    return false;
}

bool Device::SetAutoSend(UINT chnIdx, const ZCAN_AUTO_TRANSMIT_OBJ* pObj)
{
    if (nullptr == pObj || chnIdx >= m_devChnlCount)
    {
        return false;
    }

    if (pObj->index >= DEV_AUTO_SEND_INDEX_MAX)
    {
        return false;
    }

    Packet packet = CPacketEncoder::BuildPacketAutosend(chnIdx, pObj, m_bTxEcho);
    if (packet.size() == SendData(packet.data(), packet.size()))
    {
        return true;
    }
    return false;
}

bool Device::SetAutoSend(UINT chnIdx, const ZCANFD_AUTO_TRANSMIT_OBJ* pObj)
{
    if (nullptr == pObj || chnIdx >= m_devChnlCount)
    {
        return false;
    }

    if (pObj->index >= DEV_AUTO_SEND_INDEX_MAX)
    {
        return false;
    }

    Packet packet = CPacketEncoder::BuildPacketAutosend(chnIdx, pObj, m_bTxEcho);
    if (packet.size() == SendData(packet.data(), packet.size()))
    {
        return true;
    }
    return false;
}

bool Device::SetAutoSend(UINT chnIdx, VecAutoSendData& vecAutoSend)
{
    if (chnIdx >= m_devChnlCount)
    {
        return false;
    }

    if (vecAutoSend.size() > 0)
    {
        VecPackets vecPackets;
        vecPackets = CPacketEncoder::BuildPacketAutosend(chnIdx, vecAutoSend, m_bTxEcho);
        for (auto& packet : vecPackets)
        {
            if (packet.size() != SendData(packet.data(), packet.size()))
            {
                return false;
            }
        }
    }
    return true;
}

unsigned int Device::SendData(const BYTE* data, unsigned int len)
{
    const char* cdata = reinterpret_cast<const char*>(data);
    if (nullptr == data ||
        len == 0 ||
        !m_service ||
        !m_service->SendData(cdata, len))
    {
        m_statistic.nTxBytesFailed += len;
        return 0;
    }
    m_statistic.nTxBytesSuccess += len;
    return len;
}

bool Device::VerifyDeviceByPass(VerifyDeviceData& data)
{
    bool bRet = false;
    BYTE session = ++m_authIncIndex;
    Packet packet = CPacketEncoder::BuildPacketAUTH(true, reinterpret_cast<const BYTE*>(data.inData), sizeof(data.inData), session);
    LOG_INFO("Auth Data Send Request :%d\n", session);
    if ( packet.size() == SendData(packet.data(), packet.size()) )
    {
        //wait for response
        AuthDeviceData_PTR authData = std::make_shared<AuthDeviceData>();
        if (authData)
        {
            memcpy(&authData->data, &data, sizeof(VerifyDeviceData));
            {
                std::unique_lock<std::mutex> lk_(m_authMapMutex);
                m_authDataMap[session] = authData;
            }
            std::mutex mutex_;
            std::unique_lock<std::mutex> ulk(mutex_);
            if (authData->cv.wait_for(ulk, std::chrono::milliseconds(REQUEST_TIMEOUT)) == std::cv_status::no_timeout)
            {
                memcpy(data.OutData, authData->data.OutData,  sizeof(data.OutData));
                bRet = true;
                LOG_INFO("Auth Data Copyed\n");
            }
            else
            {
                LOG_ERR("Auth Data Response Timeout\n");
            }

            //Clear result
            {
                std::lock_guard<std::mutex> lk_(m_authMapMutex);
                m_authDataMap.erase(session);
            }
        }
    }
    return bRet;
}

bool Device::StartCAN(UINT chnIdx)
{
    if (chnIdx >= m_devChnlCount)
    {
        return false;
    }
    if (m_bitChnlStarted.any())
    {
        m_bitChnlStarted.set(chnIdx);
        return true;
    }

    if (CreateAndStartService())
    {
        m_bitChnlStarted.set(chnIdx);

        //TCP客户端模式,UDP模式会在设备启动时获取一次设备信息
        if (!m_devTCP || TCP_SERVER != m_workMode)
        {
            GetDeviceInfo(m_stDevInfo);
        }

        LOG_ALWAYS("Device start success!\n");
        return true;
    }
    else
    {
        m_bitChnlStarted.reset(chnIdx);
    }
    return false;
}

bool Device::CreateAndStartService()
{
    if (!m_service)
    {
#ifndef BUILD_LIB_UDP
        if (m_devTCP)
        {
            if (m_workMode == TCP_SERVER)
            {
                //tcp server
				//LOG_ALWAYS("tid = %u, Tcp server mode. listen on:%d\n", *(unsigned int *)&std::this_thread::get_id(), m_srcPort);
                m_service = new TcpServer(m_srcPort, DEV_MAX_CLIENTS);
            }
            else
            {
                //tcp client 
				//LOG_ALWAYS("tid = %u, Tcp client mode. connect to:%s:%d\n", *(unsigned int *)&std::this_thread::get_id(), m_destIp.c_str(), m_destPort);
                m_service = new TcpClient(m_destIp.c_str(), m_destPort);
            }
        }
        else
        {
			//LOG_ALWAYS("tid = %u, UDP mode. %d to:%s:%d\n", *(unsigned int *)&std::this_thread::get_id(), m_srcPort, m_destIp.c_str(), m_destPort);
            m_service = new UdpService(m_srcPort, m_destIp.c_str(), m_destPort, m_udpmulticastloopback);
        }
#else
        {
            LOG_ALWAYS("UDP mode. %d to:%s:%d\n", m_srcPort, m_destIp.c_str(), m_destPort);
            m_service = new UdpService(m_srcPort, m_destIp.c_str(), m_destPort, m_udpmulticastloopback);
        }
#endif

        if (!m_service)
        {
            return false;
        }

        m_service->SetRecvCallback(std::bind(&Device::DealReceivedData, this,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    if (m_service->IsStart())
    {
        return true;
    }
    else
    {
        if (m_service->Start())
        {
            StartDataProcessThread();
            m_tpDevStartTickCount = std::chrono::steady_clock::now();

            return true;
        }
    }

    return false;
}

bool Device::StopCAN(UINT chnIdx)
{
    if (chnIdx >= m_devChnlCount)
    {
        return false;
    }

    m_bitChnlStarted.reset(chnIdx);

    if (m_bitChnlStarted.any())
    {
        return true;
    }

    return _StopAndClear();
}

bool Device::Stop()
{
    m_bitChnlStarted.reset();

    return _StopAndClear();
}

#include <iostream>
bool Device::_StopAndClear()
{
    LOG_INFO("_StopAndClear+++\n");
    if (!m_service)
    {
        LOG_INFO("_StopAndClear0---\n");
        return false;
    }

#ifdef WIN32
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);//获取时钟周期
	QueryPerformanceCounter(&nBeginTime);
#endif

    m_service->Stop();

#ifdef WIN32
	QueryPerformanceCounter(&nEndTime);
	double ddf = (double)nFreq.QuadPart;
	double ddt = (double)(nEndTime.QuadPart - nBeginTime.QuadPart);
	double time = ddt / ddf;
	// std::cout << __FUNCTION__ << "^^^^ StopCAN time is: " << time * 1000 << "ms" << std::endl;
	//LOG_ALWAYS("%s^^^^ DisconnectServer time is: %fms", __FUNCTION__, time * 1000);
#endif

    StopDataProcessThread();

    {
        std::lock_guard<std::mutex> lk_(m_recvDecodersMutex);
        for (auto& decoder : m_vecDecoders)
        {
            delete decoder;
            decoder = nullptr;
        }
        m_vecDecoders.clear();
    }

    LOG_ALWAYS("Device TxBytes:%I64u TxFailedBytes:%I64u RxBytes:%I64u\n", m_statistic.nTxBytesSuccess, m_statistic.nTxBytesFailed, m_statistic.nRxBytes);

    //CAN
    for (size_t i = 0; i < m_devChnlCount; i++)
    {
        //Clear can buffer
        ClearBuffer(i);

        //auto send data
        m_autoSendData[i].clear();

        //log chnl statistic
        LOG_ALWAYS("Chnl %d: TxCAN:%I64u(S:%I64u) TxCANFD:%I64u(S:%I64u) RxCAN:%I64u(D:%I64u) RxCANFD:%I64u(D:%I64u) Err:%I64u TxEcho:%I64u RxMerge:%I64u(D:%I64u) AppRxMerge:%I64u\n", 
            i, m_statistic.chnl[i].nTxCAN, m_statistic.chnl[i].nTxCANSim, m_statistic.chnl[i].nTxCANFD, m_statistic.chnl[i].nTxCANFDSim,
            m_statistic.chnl[i].nRxCAN, m_statistic.chnl[i].nRxCANDrop, m_statistic.chnl[i].nRxCANFD, m_statistic.chnl[i].nRxCANFDDrop,
            m_statistic.chnl[i].nError, m_statistic.chnl[i].nTxEcho, m_statistic.chnl[i].nRxMerge, m_statistic.chnl[i].nRxMergeDrop, m_statistic.chnl[i].nAppRxMerge);
    }

    //LIN
    for (size_t i = 0; i < m_devLINCount; i++)
    {
        m_frmLIN[i].clear();
    }

    //Other
    {
        m_frmGPS.clear();
        m_frmMerge.clear();
    }

    {
        std::lock_guard<std::mutex> lk(m_authMapMutex);
        m_authDataMap.clear();
    }

    {
        std::lock_guard<std::mutex> lk(m_pktRequestMutex);
        m_pktRequestDataMap.clear();
    }

    delete m_service;
    m_service = nullptr;

    memset(&m_statistic, 0, sizeof(m_statistic));
    m_bTxEcho = false;
    m_bRecvMerge = false;

    //Clear Dev State
    m_stateSysInfo.bNewData = false;
    m_stateSysInfo.strState.clear();
    m_stateCANInfo.bNewData = false;
    m_stateCANInfo.strState.clear();
    m_stateRecorderInfo.bNewData = false;
    m_stateRecorderInfo.strState.clear();
    m_stateNetInfo.bNewData = false;
    m_stateNetInfo.strState.clear();

    m_bDevInfoUpdated = false;
    m_stDevInfo.fw_Version = 0;
    m_stDevInfo.hw_Version = 0;
    memset(m_stDevInfo.str_Serial_Num, 0, sizeof(m_stDevInfo.str_Serial_Num));
    memcpy(m_stDevInfo.str_hw_Type, m_devName.c_str(), min(m_devName.length(), (size_t)40));

    LOG_INFO("_StopAndClear1---\n");
    return true;
}

bool Device::InitLIN(UINT chnIdx, PZCAN_LIN_INIT_CONFIG pLINInitConfig)
{
    if (chnIdx < DEV_LIN_COUNT_MAX && pLINInitConfig != NULL)
    {
        m_linConfig[chnIdx].IsMaster = (pLINInitConfig->linMode == LIN_MODE_MASTER);
        m_linConfig[chnIdx].nEnable = 0;
        m_linConfig[chnIdx].stFeature.rawVal = 0;
        m_linConfig[chnIdx].stFeature.unionVal.bEnhancedChksum = !!(pLINInitConfig->linFlag & LIN_FLAG_CHK_ENHANCE);
        m_linConfig[chnIdx].stFeature.unionVal.bVarDLCDisabled = !(pLINInitConfig->linFlag & LIN_FLAG_VAR_DLC);
        m_linConfig[chnIdx].nBaud = pLINInitConfig->linBaud;
        return true;
    }
    return false;
}

UINT Device::StartLIN(UINT chnIdx)
{
    if (chnIdx >= m_devLINCount)
    {
        return false;
    }
    UINT nLINIndex = DEV_CHNL_COUNT_MAX + chnIdx;
    if (m_bitChnlStarted.test(nLINIndex))
    {
        return true;
    }

    if (CreateAndStartService())
    {
        //TCP客户端模式,UDP模式会在设备启动时获取一次设备信息
        if (!m_devTCP || TCP_SERVER != m_workMode)
        {
            GetDeviceInfo(m_stDevInfo);
        }

        //Start
        m_linConfig[chnIdx].nEnable = 1;
        bool bRet = SetLINConfig(chnIdx, m_linConfig[chnIdx]);
        if (bRet)
        {
            m_bitChnlStarted.set(nLINIndex);
            LOG_ALWAYS("Device LIN :%d start success!\n", chnIdx);
        }
        else
        {
            LOG_ALWAYS("Device LIN :%d start failed!\n", chnIdx);
        }

        return bRet;
    }
    else
    {
        m_bitChnlStarted.reset(nLINIndex);
    }
    return false;
}

UINT Device::ResetLIN(UINT chnIdx)
{
    if (chnIdx >= m_devChnlCount)
    {
        return false;
    }

    m_linConfig[chnIdx].nEnable = 0;
    bool bRet = SetLINConfig(chnIdx, m_linConfig[chnIdx]);
    LOG_ALWAYS("Device LIN :%d Reset result:%d!\n", chnIdx, bRet);

    UINT nLINIndex = DEV_CHNL_COUNT_MAX + chnIdx;
    m_bitChnlStarted.reset(nLINIndex);

    if (m_bitChnlStarted.any())
    {
        return true;
    }

    return _StopAndClear();
}

UINT Device::TransmitLIN(UINT chnIdx, PZCAN_LIN_MSG pSend, UINT Len)
{
    if (chnIdx >= m_devLINCount || pSend == nullptr || Len == 0 || !m_linConfig[chnIdx].IsMaster)
    {
        return 0;
    }

    return SendLINMsgs(chnIdx, pSend, Len);
}

UINT Device::GetLINReceiveNum(UINT chnIdx)
{
    if (chnIdx < m_devLINCount)
    {
        return m_frmLIN[chnIdx].size();
    }
    return 0;
}

UINT Device::ReceiveLIN(UINT chnIdx, PZCAN_LIN_MSG pReceive, UINT Len, int WaitTime)
{
    if (chnIdx >= m_devLINCount || pReceive == nullptr || Len == 0)
    {
        return 0;
    }

    CanDataQueue<PacketDataLIN>& frmdata = m_frmLIN[chnIdx];

#ifdef WIN32
    frmdata.WaitEvent(WaitTime);
#else
    const int per_sleep_ms = 1;
    if (WaitTime > 0)
    {
        int sleepTimes = (int)ceil(WaitTime / (float)per_sleep_ms);
        while (sleepTimes--)
        {
            if (frmdata.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
    else if (WaitTime < 0)
    {
        while (true)
        {
            if (frmdata.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
#endif

    memset(pReceive, 0, Len*sizeof(ZCAN_LIN_MSG));

    frmdata.lock();
    UINT actLen = frmdata.size() > Len ? Len : frmdata.size();
    for (ULONG i = 0; i < actLen; i++)
    {
        CPacketUtil::PacketLIN2ZLIN(frmdata.front(), pReceive[i], nullptr);
        frmdata.pop();
    }
    frmdata.unlock();

    //LOG_INFO("ReceiveLIN Chnl:%d, count:%d\n", chnIdx, actLen);

    return actLen;
}

UINT Device::SetLINSlaveMsg(UINT chnIdx, PZCAN_LIN_MSG pSend, UINT nMsgCount)
{
    //从站模式时直接发送，即为设置从站响应信息
    if (chnIdx < m_devLINCount && m_linConfig[chnIdx].IsMaster == 0)
    {
        return SendLINMsgs(chnIdx, pSend, nMsgCount);
    }
    return false;
}

UINT Device::ClearLINSlaveMsg(UINT chnIdx, BYTE* pLINID, UINT nIDCount)
{
    //协议中只有清除单个通道的lin从站响应命令，如果需要清除单个id的响应
    //直接在从站中发送对应的id，将数据长度置为0即为清除对应id的响应
    if (chnIdx < m_devLINCount && m_linConfig[chnIdx].IsMaster == 0)
    {
        if (pLINID == nullptr || nIDCount == 0)
        {
            return ClearLINAsSlaveResponse(chnIdx);
        }
        else
        {
            std::vector<ZCAN_LIN_MSG> vecLINMsg;
            vecLINMsg.resize(nIDCount);
            for (UINT i = 0; i < nIDCount; ++i)
            {
                memset(&vecLINMsg[i], 0, sizeof(ZCAN_LIN_MSG));
                vecLINMsg[i].ID = pLINID[i];
                vecLINMsg[i].DataLen = 0;
            }
            return SendLINMsgs(chnIdx, vecLINMsg.data(), vecLINMsg.size());
        }
    }
    return false;
}

UINT Device::SendLINMsgs(UINT chnIdx, PZCAN_LIN_MSG pSend, UINT Len)
{
    UINT sentCnt = 0;
    VecPackets vecPackets;
    if (chnIdx >= m_devLINCount || pSend == nullptr || Len == 0)
    {
        return sentCnt;
    }

    vecPackets = CPacketEncoder::BuildPacketLIN(chnIdx, pSend, Len);
    for (auto& packet : vecPackets)
    {
        if (packet.size() != SendData(packet.data(), packet.size()))
        {
            m_statistic.chnl[chnIdx].nTxLIN += sentCnt;
            return sentCnt;
        }
        sentCnt += CPacketUtil::GetPacketFrameCount(packet);
    }
    m_statistic.chnl[chnIdx].nTxLIN += sentCnt;
    return sentCnt;
}

bool Device::AddAutoSend(UINT chnIdx, const ZCAN_AUTO_TRANSMIT_OBJ* pObj)
{
    if (chnIdx < m_devChnlCount && pObj && pObj->index < DEV_AUTO_SEND_INDEX_MAX)
    {
        VecAutoSendData& vecAutoSend = m_autoSendData[chnIdx];
        AutoSendData* pAutoSendData = GetAutoSendIndex(vecAutoSend, pObj->index);
        if (pAutoSendData != nullptr)
        {
            ZCANAutoSend2AutoSend(pObj, *pAutoSendData);
        }
        else
        {
            AutoSendData autoSend;
            ZCANAutoSend2AutoSend(pObj, autoSend);
            vecAutoSend.push_back(autoSend);
        }

        return true;
    }
    return false;
}

bool Device::AddAutoSend(UINT chnIdx, const ZCANFD_AUTO_TRANSMIT_OBJ* pObj)
{
    if (chnIdx < m_devChnlCount && pObj && pObj->index < DEV_AUTO_SEND_INDEX_MAX)
    {
        VecAutoSendData& vecAutoSend = m_autoSendData[chnIdx];
        AutoSendData* pAutoSendData = GetAutoSendIndex(vecAutoSend, pObj->index);
        if (pAutoSendData != nullptr)
        {
            ZCANAutoSend2AutoSend(pObj, *pAutoSendData);
        }
        else
        {
            AutoSendData autoSend;
            ZCANAutoSend2AutoSend(pObj, autoSend);
            vecAutoSend.push_back(autoSend);
        }

        return true;
    }
    return false;
}

bool Device::ApplyAutoSend(UINT chnIdx)
{
    if (chnIdx < m_devChnlCount)
    {
        VecAutoSendData& vecAutoSend = m_autoSendData[chnIdx];
        return SetAutoSend(chnIdx, vecAutoSend);
    }
    return false;
}

bool Device::ClearAutoSend(UINT chnIdx)
{
    if (chnIdx < m_devChnlCount)
    {
        VecAutoSendData& vecAutoSend = m_autoSendData[chnIdx];
        for (auto & item : vecAutoSend)
        {
            item.data.enable = 0;
        }

        bool bret = SetAutoSend(chnIdx, vecAutoSend);
        vecAutoSend.clear();
        return bret;
    }
    return false;
}

AutoSendData* Device::GetAutoSendIndex(VecAutoSendData& vecAutoSend, UINT autoSendIndex)
{
    for (auto& item : vecAutoSend)
    {
        if (item.data.index == autoSendIndex)
        {
            return &item;
        }
    }
    return nullptr;
}

void Device::ZCANAutoSend2AutoSend(const ZCAN_AUTO_TRANSMIT_OBJ* pObj, AutoSendData& autoSend)
{
    memset(&autoSend, 0, sizeof(autoSend));
    autoSend.iscanfd = 0;
    autoSend.sendcount = 0;
    memcpy(&autoSend.data, pObj, sizeof(ZCAN_AUTO_TRANSMIT_OBJ));
}

void Device::ZCANAutoSend2AutoSend(const ZCANFD_AUTO_TRANSMIT_OBJ* pObj, AutoSendData& autoSend)
{
    memset(&autoSend, 0, sizeof(autoSend));
    autoSend.iscanfd = 1;
    autoSend.sendcount = 0;
    memcpy(&autoSend.data, pObj, sizeof(ZCANFD_AUTO_TRANSMIT_OBJ));
}

bool Device::IsDevStartLessThan(long long ms)
{
    std::chrono::steady_clock::time_point tp_recv = std::chrono::steady_clock::now();
    std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(tp_recv - m_tpDevStartTickCount);
    return time_span.count() <= ms;
}

bool Device::SetDevTimeStamp(UINT64 nTimeStampUs)
{
    PacketTypeREQParam reqParam;
    reqParam.rawValue = 0;
    reqParam.unionValue.nReqVal = PACKET_TYPE_PARAM_DEV_REQ_RES_TIMESTAMP;
    UINT64 nTimeNetOrder = cc_htonll(nTimeStampUs);
    BYTE* data = (BYTE*)&nTimeNetOrder;
    Packet packet = CPacketEncoder::BuildPacketDevReqRes(true, data, sizeof(nTimeNetOrder), reqParam.rawValue);
    return SendData(packet.data(), packet.size()) == packet.size();
}

UINT Device::GetTxEchoTimeStamp(UINT chnIdx, TxTimeStamp& rTxTimeStamp)
{
    if (chnIdx >= m_devChnlCount || rTxTimeStamp.pTxTimeStampBuffer == nullptr || rTxTimeStamp.nBufferTimeStampCount == 0)
    {
        return 0;
    }

    return 0;
}

bool Device::GetBusUsage(UINT chnIdx, BusUsage& rBusUsage)
{
    if (chnIdx < m_devChnlCount && m_busUsage.newdata[chnIdx])
    {
        rBusUsage.nBusUsage = m_busUsage.chnl[chnIdx].nBusUsage;
        rBusUsage.nChnl = m_busUsage.chnl[chnIdx].nChnl;
        rBusUsage.nFrameCount = m_busUsage.chnl[chnIdx].nFrameCount;
        rBusUsage.nReserved = m_busUsage.chnl[chnIdx].nReserved;
        rBusUsage.nTimeStampBegin = m_busUsage.chnl[chnIdx].nTimeStampBegin;
        rBusUsage.nTimeStampEnd = m_busUsage.chnl[chnIdx].nTimeStampEnd;

        m_busUsage.newdata[chnIdx] = false;
        return true;
    }
    return false;
}

bool Device::GetDelaySendAvailableTxCount(UINT chnIdx, UINT& nCount)
{
    if (chnIdx < m_devChnlCount)
    {
        std::string strDelayInfo;
        if (GetDelaySendInfo(chnIdx, strDelayInfo))
        {
            cJSON* pInfo = cJSON_Parse(strDelayInfo.c_str());
            if (pInfo)
            {
                UINT nSize = (UINT)UtilGetcJSONNumber(cJSON_DetachItemFromObjectCaseSensitive(pInfo, "Size"));
                nCount = (UINT)UtilGetcJSONNumber(cJSON_DetachItemFromObjectCaseSensitive(pInfo, "Remain"));
                cJSON_Delete(pInfo);
                return true;
            }
        }
    }
    return false;
}

bool Device::GetDevAutoSendListCount(UINT chnIdx, bool bCANFD, UINT& nCount)
{
    if (chnIdx < m_devChnlCount)
    {
        std::chrono::steady_clock::time_point tp_now = std::chrono::steady_clock::now();
        std::chrono::milliseconds time_span = std::chrono::duration_cast<std::chrono::milliseconds>(tp_now - m_devAutoSendList.chnl[chnIdx].m_tpRecv);
        if (time_span.count() > REQUEST_TIMEOUT)
        {
            std::vector<PacketDataAutoSend> vecAutoSend;
            if (!GetDevAutoSendList(chnIdx, vecAutoSend))
            {
                return false;
            }

            m_devAutoSendList.chnl[chnIdx].vecCAN.clear();
            m_devAutoSendList.chnl[chnIdx].vecCANFD.clear();
            m_devAutoSendList.chnl[chnIdx].m_tpRecv = std::chrono::steady_clock::now();
            ZCAN_AUTO_TRANSMIT_OBJ zCAN;
            ZCANFD_AUTO_TRANSMIT_OBJ zCANFD;
            int nChnl;
            for (auto& item : vecAutoSend)
            {
                if (item.bEnable)
                {
                    if (item.pktData.canHead.frameInfo.unionVal.bFD)
                    {
                        CPacketUtil::AutoSendFrame2ZCANAutoSend(item, zCANFD, nChnl);
                        m_devAutoSendList.chnl[chnIdx].vecCANFD.push_back(zCANFD);
                    }
                    else
                    {
                        CPacketUtil::AutoSendFrame2ZCANAutoSend(item, zCAN, nChnl);
                        m_devAutoSendList.chnl[chnIdx].vecCAN.push_back(zCAN);
                    }
                }
            }
        }

        if (bCANFD)
        {
            nCount = m_devAutoSendList.chnl[chnIdx].vecCANFD.size();
        }
        else
        {
            nCount = m_devAutoSendList.chnl[chnIdx].vecCAN.size();
        }
        return true;
    }
    return false;
}

bool Device::GetDevAutoSendListData(UINT chnIdx, bool bCANFD, void* pData)
{
    if (chnIdx < m_devChnlCount && pData != nullptr)
    {
        int nCount = 0, nSize=0;
        if (bCANFD)
        {
            nCount = m_devAutoSendList.chnl[chnIdx].vecCANFD.size();
            nSize = sizeof(ZCANFD_AUTO_TRANSMIT_OBJ) * nCount;
            memcpy(pData, m_devAutoSendList.chnl[chnIdx].vecCANFD.data(), nSize);
        }
        else
        {
            nCount = m_devAutoSendList.chnl[chnIdx].vecCAN.size();
            nSize = sizeof(ZCAN_AUTO_TRANSMIT_OBJ) * nCount;
            memcpy(pData, m_devAutoSendList.chnl[chnIdx].vecCAN.data(), nSize);
        }
        return true;
    }
    return false;
}

bool Device::RequestDevData(PacketDataRequest& devRequest, PacketDataResponse& devResponse)
{
    bool bRet = false;
    m_pktRequestSeq = (++m_pktRequestSeq)%8;
    m_pktTID++;

    BYTE nSeq = m_pktRequestSeq;
    BYTE nTID = m_pktTID;

    devRequest.nTID = nTID;
    DevRequestResponse_PTR data = std::make_shared<DevRequestResponse>();
    if (data)
    {
        data->nSeq = nSeq;
        data->nTID = nTID;
        data->requestData = devRequest;
    }
    else
    {
        return false;
    }

    int key = GetDevRequestKey(nSeq, nTID);
    Packet packet = CPacketEncoder::BuildPacketRequest(nSeq, devRequest);
    LOG_INFO("Request Dev Data request:%d, seq:%d, tid:%d\n", devRequest.nRequest, nSeq, nTID);

	//wait for response
	{
		std::unique_lock<std::mutex> lk_(m_pktRequestMutex);
		m_pktRequestDataMap[key] = data;
	}
    if (packet.size() == SendData(packet.data(), packet.size()))
    {
        std::mutex mutex_;
        std::unique_lock<std::mutex> ulk(mutex_);
		if (data->cv.wait_for(ulk, std::chrono::milliseconds(REQUEST_TIMEOUT), [&data]{
			return data->responsed; }))
        {
            devResponse = data->responseData;
            bRet = true;
            LOG_INFO("Resonse Data Copyed, seq:%d tid:%d/%d\n", nSeq, nTID, devResponse.nTID);
        }
        else
        {
            LOG_ERR("Resonse Data Response Timeout\n");
        }

        //Clear result
        {
            std::lock_guard<std::mutex> lk_(m_pktRequestMutex);
            m_pktRequestDataMap.erase(key);
        }
    }
    else
    {
        LOG_ERR("RequestDevData send data error\n");
    }
    return bRet;
}

int Device::GetDevRequestKey(BYTE pktSeq, BYTE nTID)
{
    pktSeq = pktSeq % 8;
    return (pktSeq << 8 | nTID);
}
void Device::GenGetDevInfoRequest(PacketDataRequest& devRequest)
{
    devRequest.nRequest = PACKET_REQUEST_GET_DEVICE_INFO;
    devRequest.nTID = 0;
    devRequest.vecData.clear();
}

void Device::GenGetDevStateRequest(BYTE nType, PacketDataRequest& devRequest)
{
    devRequest.nRequest = PACKET_REQUEST_GET_DEVICE_STATE;
    devRequest.nTID = 0;
    char requestJson[64];
    memset(requestJson, 0, sizeof(requestJson));
    int nLen = snprintf(requestJson, 64, "{\r\n\"Type\":%d\r\n}", nType) + 1;
    BYTE* pData = (BYTE*)&requestJson[0];
    devRequest.vecData.assign(pData, pData + nLen);
}

void Device::GenGetAutoSendListRequest(UINT chnIdx, UINT nOffset, UINT nRequestCount, PacketDataRequest& devRequest)
{
    devRequest.nRequest = PACKET_REQUEST_GET_DEVICE_AUTOSEND_LIST;
    devRequest.nTID = 0;
    char requestJson[200];
    memset(requestJson, 0, sizeof(requestJson));
    int nLen = snprintf(requestJson, 200, "{\r\n\"Chn\":%d,\r\n\"Offset\":%d,\r\n\"Num\":%d\r\n}", chnIdx, nOffset, nRequestCount) + 1;
    BYTE* pData = (BYTE*)&requestJson[0];
    devRequest.vecData.assign(pData, pData + nLen);
}

void Device::GenGetDevDelaySendRequest(UINT chnIdx, PacketDataRequest& devRequest)
{
    devRequest.nRequest = PACKET_REQUEST_GET_DEVICE_DELAY_SEND_STATE;
    devRequest.nTID = 0;
    char requestJson[64];
    memset(requestJson, 0, sizeof(requestJson));
    int nLen = snprintf(requestJson, 64, "{\r\n\"Chn\":%d\r\n}", chnIdx) + 1;
    BYTE* pData = (BYTE*)&requestJson[0];
    devRequest.vecData.assign(pData, pData + nLen);
}

void Device::GenClearDelaySendRequest(UINT chnIdx, PacketDataRequest& devRequest)
{
    devRequest.nRequest = PACKET_REQUEST_CLEAR_DELAY_SEND_QUEUE;
    devRequest.nTID = 0;
    char requestJson[64];
    memset(requestJson, 0, sizeof(requestJson));
    int nLen = snprintf(requestJson, 64, "{\r\n\"Chn\":%d\r\n}", chnIdx) + 1;
    BYTE* pData = (BYTE*)&requestJson[0];
    devRequest.vecData.assign(pData, pData + nLen);
}

bool Device::GetDevInfo(std::string& strDevInfo)
{
    PacketDataRequest request;
    PacketDataResponse response;
    GenGetDevInfoRequest(request);
    if (RequestDevData(request, response))
    {
        if (response.nResult)
        {
            std::string strResult(response.vecData.begin(), response.vecData.end());
            strDevInfo = strResult.c_str();
        }
        return response.nResult > 0;
    }
    return false;
}

bool Device::GetDevState(BYTE nType, std::string& strDevState)
{
    PacketDataRequest request;
    PacketDataResponse response;
    GenGetDevStateRequest(nType, request);
    if (RequestDevData(request, response))
    {
        if (response.nResult)
        {
            std::string strResult(response.vecData.begin(), response.vecData.end());
            strDevState = strResult.c_str();
        }
        return response.nResult > 0;
    }
    return false;
}

bool Device::GetDevAutoSendList(UINT chnIdx, std::vector<PacketDataAutoSend>& vecAutoSend)
{
    //由于帧长度限制，一帧定时发送占用88字节，一个以太网帧可以包装(1460-6-1)/88 = 16 帧
    UINT nTotalCount = DEV_AUTO_SEND_INDEX_MAX;
    UINT nReaded = 0;
    PacketDataRequest request;
    PacketDataResponse response;
    vecAutoSend.clear();
    std::vector<PacketDataAutoSend> vecAutoSendOnce;
    while (nReaded < nTotalCount)
    {
        //由于一次通讯需要在单包内完成传输，一次获取量过大会导致响应数据超出单包的数据长度
        //这里限制一次获取的条目数量设置为4
        UINT nReadOnce = min((UINT)4, nTotalCount - nReaded);
        GenGetAutoSendListRequest(chnIdx, nReaded, nReadOnce, request);
        if (RequestDevData(request, response))
        {
            if (response.nResult)
            {
                //response中的vecData包含定时报文的json字符串
                std::string strResult(response.vecData.begin(), response.vecData.end());
                UINT nCount = GetAutoSendListFromJsonStr(chnIdx, nReaded, nReadOnce, strResult, vecAutoSendOnce);
                vecAutoSend.insert(vecAutoSend.end(), vecAutoSendOnce.begin(), vecAutoSendOnce.end());
                nReaded += nCount;
                if (0 == nCount || nCount < nReadOnce)
                {
                    break;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    assert(nReaded == vecAutoSend.size());
    return true;
}

bool Device::GetDelaySendInfo(UINT chnIdx, std::string& strDelaySendInfo)
{
    PacketDataRequest request;
    PacketDataResponse response;
    GenGetDevDelaySendRequest(chnIdx, request);
    if (RequestDevData(request, response))
    {
        if (response.nResult)
        {
            std::string strResult(response.vecData.begin(), response.vecData.end());
            strDelaySendInfo = strResult.c_str();
        }
        return response.nResult > 0;
    }
    return false;
}

bool Device::ClearDelaySendQueue(UINT chnIdx)
{
    if (chnIdx < m_devChnlCount)
    {
        PacketDataRequest request;
        PacketDataResponse response;
        GenClearDelaySendRequest(chnIdx, request);
        if (RequestDevData(request, response))
        {
            return response.nResult > 0;
        }
        return false;
    }
    return false;
}

bool Device::GetDevInfoFromJsonStr(const std::string& strDevInfo, ZCAN_DEVICE_INFO * pInfo)
{
    LOG_INFO("Device info string:%s\n", strDevInfo.c_str());
    if (pInfo == nullptr)
    {
        return false;
    }

    cJSON* pDevInfo = cJSON_Parse(strDevInfo.c_str());
    if (pDevInfo)
    {
        std::string strModel = UtilGetcJSONString(cJSON_GetObjectItemCaseSensitive(pDevInfo, "Model"));
        std::string strFirmware = UtilGetcJSONString(cJSON_GetObjectItemCaseSensitive(pDevInfo, "Firmware"));
        std::string strHardware = UtilGetcJSONString(cJSON_GetObjectItemCaseSensitive(pDevInfo, "Hardware"));
        std::string strSN = UtilGetcJSONString(cJSON_GetObjectItemCaseSensitive(pDevInfo, "SN"));
        std::string strProtoVersion = UtilGetcJSONString(cJSON_GetObjectItemCaseSensitive(pDevInfo, "ProtoVer"));   //20201105，通讯协议版本，V1.10后存在此字段

        memset(pInfo, 0, sizeof(ZCAN_DEVICE_INFO));
        pInfo->can_Num = m_devChnlCount;
        memcpy(pInfo->str_hw_Type, strModel.c_str(), min(strModel.length(), sizeof(pInfo->str_hw_Type)));
        memcpy(pInfo->str_Serial_Num,  strSN.c_str(), min(strSN.length(), sizeof(pInfo->str_Serial_Num)));
        pInfo->dr_Version = 0x0100;
        pInfo->in_Version = 0x0100;
        pInfo->fw_Version = UtilVersionString2USHORT(strFirmware.c_str());
        pInfo->hw_Version = UtilVersionString2USHORT(strHardware.c_str());

        cJSON_Delete(pDevInfo);
        return true;
    }

    return false;
}

UINT Device::GetAutoSendListFromJsonStr(UINT chnIdx, UINT nOffset, UINT nRequestCount, const std::string& strAutoSendJson, std::vector<PacketDataAutoSend>& vecAutoSend)
{
    cJSON* pAutoSendMsgs = cJSON_Parse(strAutoSendJson.c_str());
    if (pAutoSendMsgs)
    {
        vecAutoSend.clear();
        PacketDataAutoSend item;
        for (UINT i = nOffset; i<nOffset + nRequestCount; ++i)
        {
            std::string key = UtilStringAppendIndex("Msg", i);
            cJSON* pSendItem = cJSON_GetObjectItemCaseSensitive(pAutoSendMsgs, key.c_str());
            if (pSendItem)
            {
                GetAutoSendFromJson(chnIdx, i, pSendItem, item);
                vecAutoSend.push_back(item);
            }
            else
            {
                break;
            }
        }

        cJSON_Delete(pAutoSendMsgs);
    }

    return vecAutoSend.size();
}

void Device::GetAutoSendFromJson(UINT nChnIdx, UINT nIndex, cJSON* config, PacketDataAutoSend& autoSend)
{
    if (!config) return;
    memset(&autoSend, 0, sizeof(autoSend));

    //20201106 协议版本更新V1.10之后添加发送周期单位字段，无参数或者参数值为0表示单位是ms，值为1表示时间单位为100us(0.1ms)
    int nPeriodUnit = (int)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(config, "PeriodUnit"));
    nPeriodUnit = !!nPeriodUnit;
    int nPeriodValue = (int)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(config, "Period"));
    autoSend.stFlag.unionValue.nUseDataTimestamp = nPeriodUnit;
    autoSend.nPeriodMs = nPeriodUnit == 0 ? nPeriodValue : nPeriodValue/10;
    autoSend.pktData.canHead.nTimestamp = nPeriodValue;

    autoSend.nIndex = (BYTE)nIndex;
    autoSend.bEnable = (BYTE)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(config, "Enable"));
    autoSend.nRepeatCount = (USHORT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(config, "Cnt"));

    autoSend.pktData.canHead.nID = (int)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(config, "MsgID"));
    autoSend.pktData.canHead.frameInfo.rawVal = (int)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(config, "MsgFlag"));
    autoSend.pktData.canHead.nChnl = (BYTE)nChnIdx;
    cJSON* msgData = cJSON_GetObjectItemCaseSensitive(config, "MsgData");
    autoSend.pktData.canHead.nDataLen = UtilJsonByteArray(msgData, &autoSend.pktData.canData[0], DATA_LEN_CANFD);
}

UINT Device::GetGPSRecvNum()
{
    return m_frmGPS.size();
}

UINT Device::GetGPSData(ZCAN_GPS_FRAME* pReceive, UINT len, int waitTime)
{
    if (pReceive == nullptr || len == 0)
    {
        return 0;
    }

    CanDataQueue<PacketDataGPS>& frmGPS = m_frmGPS;

    const int per_sleep_ms = 10;
    if (waitTime > 0)
    {
        int sleepTimes = (int)ceil(waitTime / (float)per_sleep_ms);
        while (sleepTimes--)
        {
            if (frmGPS.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }
    else if (waitTime < 0)
    {
        while (true)
        {
            if (frmGPS.size() <= 0)
            {
                cc_sleep(per_sleep_ms);
            }
            else
            {
                break;
            }
        }
    }

    frmGPS.lock();
    UINT actLen = frmGPS.size() > len ? len : frmGPS.size();
    for (ULONG i = 0; i < actLen; i++)
    {
        CPacketUtil::PacketGPS2ZGPS(frmGPS.front(), pReceive[i]);
        frmGPS.pop();
    }
    frmGPS.unlock();

    return actLen;
}

bool Device::GetDeviceInfo(ZCAN_DEVICE_INFO& devInfo)
{
    std::string strDevInfo;
    if (GetDevInfo(strDevInfo))
    {
        m_bDevInfoUpdated = GetDevInfoFromJsonStr(strDevInfo, &devInfo);
        return m_bDevInfoUpdated;
    }
    return false;
}

bool Device::ClearLINAsSlaveResponse(UINT chnIdx)
{
    if (chnIdx < m_devLINCount)
    {
        PacketDataRequest request;
        PacketDataResponse response;
        GenClearLINAsSlaveResponseRequest(chnIdx, request);
        if (RequestDevData(request, response))
        {
            return response.nResult > 0;
        }
        return false;
    }
    return false;
}

void Device::GenClearLINAsSlaveResponseRequest(UINT chnIdx, PacketDataRequest& devRequest)
{
    devRequest.nRequest = PACKET_REQUEST_CLEAR_LIN_AS_SLAVE_RESPONSE;
    devRequest.nTID = 0;
    char requestJson[64];
    memset(requestJson, 0, sizeof(requestJson));
    int nLen = snprintf(requestJson, 64, "{\r\n\"Chn\":%d\r\n}", chnIdx) + 1;
    BYTE* pData = (BYTE*)&requestJson[0];
    devRequest.vecData.assign(pData, pData + nLen);
}

bool Device::GetLINTxFIFOInfo(UINT chnIdx, LINFIFOInfo& fifoInfo)
{
    if (chnIdx < m_devLINCount)
    {
        PacketDataRequest request;
        PacketDataResponse response;
        GenGetLINTxFIFOInfoRequest(chnIdx, request);
        if (RequestDevData(request, response))
        {
            if (response.nResult)
            {
                std::string strResult(response.vecData.begin(), response.vecData.end());
                return GetFIFOInfoFromJsonStr(strResult, fifoInfo);
            }
            return response.nResult > 0;
        }
        return false;
    }
    return false;
}

void Device::GenGetLINTxFIFOInfoRequest(UINT chnIdx, PacketDataRequest& devRequest)
{
    devRequest.nRequest = PACKET_REQUEST_GET_LIN_FIFO_INFO;
    devRequest.nTID = 0;
    char requestJson[64];
    memset(requestJson, 0, sizeof(requestJson));
    int nLen = snprintf(requestJson, 64, "{\r\n\"Chn\":%d\r\n}", chnIdx) + 1;
    BYTE* pData = (BYTE*)&requestJson[0];
    devRequest.vecData.assign(pData, pData + nLen);
}

bool Device::GetFIFOInfoFromJsonStr(std::string& strFIFOInfo, LINFIFOInfo& fifoInfo)
{
    LOG_INFO("Device LIN FIFO info string:%s\n", strFIFOInfo.c_str());

    cJSON* pInfo = cJSON_Parse(strFIFOInfo.c_str());
    if (pInfo)
    {
        fifoInfo.nChnl = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(pInfo, "Chn"));
        fifoInfo.nFIFOSize = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(pInfo, "Size"));
        fifoInfo.nFIFOAvailable = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(pInfo, "Remain"));

        cJSON_Delete(pInfo);
        return true;
    }
    return false;
}

bool Device::SetLINConfig(UINT chnIdx,const LINConfig& linConfig)
{
    if (chnIdx < m_devLINCount)
    {
        PacketDataRequest request;
        PacketDataResponse response;
        GenSetLINConfigRequest(chnIdx, linConfig, request);
        if (RequestDevData(request, response))
        {
            if (response.nResult)
            {
                std::string strResult(response.vecData.begin(), response.vecData.end());
                return true;
            }
            return response.nResult > 0;
        }
        return false;
    }
    return false;
}

void Device::GenSetLINConfigRequest(UINT chnIdx, const LINConfig& linConfig, PacketDataRequest& devRequest)
{
    devRequest.nRequest = PACKET_REQUEST_SET_CONFIG;
    devRequest.nTID = 0;

    cJSON* pChnlObj = cJSON_CreateObject();
    cJSON_AddNumberToObject(pChnlObj, "Enable", linConfig.nEnable);
    cJSON_AddNumberToObject(pChnlObj, "IsMaster", linConfig.IsMaster);
    cJSON_AddNumberToObject(pChnlObj, "Baudrate", linConfig.nBaud);
    cJSON_AddNumberToObject(pChnlObj, "Feature", linConfig.stFeature.rawVal);

    char chnlKey[10] = { 0 };
    snprintf(chnlKey, 10, "LIN%d", chnIdx);
    cJSON* pChnlConfig = cJSON_CreateObject();
    cJSON_AddItemToObject(pChnlConfig, chnlKey, pChnlObj);

    //20210818,下位机需要在LIN动态配置时添加最外层的LIN节点
    cJSON* pLINConfig = cJSON_CreateObject();
    cJSON_AddItemToObject(pLINConfig, "LIN", pChnlConfig);

    char* pStr = cJSON_Print(pLINConfig);
    std::string strConfig(pStr);
    devRequest.vecData.assign(strConfig.c_str(), strConfig.c_str() + strConfig.length());
    cJSON_free(pStr);
    cJSON_Delete(pLINConfig);
}

void Device::StartDataProcessThread()
{
    // run data process thread
    m_bThdRun = true;
    m_DataProcessThd = std::thread(std::bind(&Device::DataProcessThdLoop, this));
	
#ifdef WIN32
    thread_set_priority(m_DataProcessThd, THREAD_PRIORITY_TIME_CRITICAL);
#endif
}

void Device::StopDataProcessThread()
{
    LOG_INFO("StopDataProcessThread +++\n");
    m_bThdRun = false;
    if (m_DataProcessThd.joinable())
    {
        m_DataProcessThd.join();
    }
    LOG_INFO("StopDataProcessThread ---\n");
}

CPacketDecoder* Device::GetDecoder(const char* rmt)
{
    std::unique_lock<std::mutex> lk_(m_recvDecodersMutex);
    for (auto* decoder : m_vecDecoders)
    {
        if (decoder->GetDecoderName().compare(rmt) == 0)
        {
            return decoder;
        }
    }

    CPacketDecoder* pNewDeocder = new CPacketDecoder(rmt);
    m_vecDecoders.push_back(pNewDeocder);
    return pNewDeocder;
}

void Device::DealReceivedData(const char* src_addr, const char* data, unsigned int len)
{
    //remote endpoint, such as "192.168.0.25:3005"
    std::string rmt_ep(src_addr);
    m_statistic.nRxBytes += len;
    CPacketDecoder* decoder = GetDecoder(src_addr);
    if (decoder)
    {
        decoder->AppendData(data, len);

#ifdef WIN32
        SetEvent(m_hDecoderData);
#endif
    }
    else
    {
        LOG_ERR("No valid decoder for %s!\n", src_addr);
    }
}

void Device::DataProcessThdLoop()
{
#ifdef WIN32
	if (m_workMode == TCP_SERVER) {
		LOG_ALWAYS("Device TcpServer DataProcessThdLoop thread,tid = %u\n", GetCurrentThreadId());
	} else {
		LOG_ALWAYS("Device TcpClient DataProcessThdLoop thread,tid = %u\n", GetCurrentThreadId());
	}
#endif

    LOG_INFO("Device DataProcessThread loop run\n");
    CPacketDecoder* decoder = nullptr;
    std::vector<CPacketDecoder*> vecDecoders;
    bool bProcessed = false;
    VecPackets vecPackets;
    while (m_bThdRun)
    {
        {
            vecDecoders.clear();
            std::unique_lock<std::mutex> lk_(m_recvDecodersMutex);
            vecDecoders = m_vecDecoders;
        }

        bProcessed = false;
        for (auto i : vecDecoders )
        {
            if (i->GetPackets(vecPackets) > 0)
            {
                ProcessPackets(vecPackets);
                bProcessed = bProcessed ? bProcessed : vecPackets.size() > 0;
            }
        }

        if (!bProcessed)
        {
#ifdef WIN32
            WaitForSingleObject(m_hDecoderData, 1);
#else
            cc_sleep(1);
#endif
        }
    }

    LOG_INFO("Device DataProcessThread loop exit\n");
}

void Device::ProcessPackets(VecPackets& vecPackets)
{
    //LOG_INFO("ProcessPackets begin, count:%d\n", vecPackets.size());
    for (auto& packet : vecPackets)
    {
        BYTE pktType = CPacketUtil::GetPacketFrameType(packet);
        if (PACKET_TYPE_CAN == pktType)
        {
            std::vector<PacketDataCAN> vecFrame;
            if (CPacketUtil::GetPacketDataCAN(packet, vecFrame) > 0)
            {
                HandleCANFrame(vecFrame);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_CAN No Valid Data!\n");
            }
        }
        else if (PACKET_TYPE_CANFD == pktType)
        {
            std::vector<PacketDataCANFD> vecFrame;
            if (CPacketUtil::GetPacketDataCANFD(packet, vecFrame) > 0)
            {
                HandleCANFDFrame(vecFrame);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_CANFD No Valid Data!");
            }
        }
        else if (PACKET_TYPE_AUTH_REQ == pktType)
        {
            FrameData vecFrame;
            BYTE pktTypeParam = 0;
            if (CPacketUtil::GetPacketDataAUTH(packet, vecFrame, pktTypeParam) > 0)
            {
                HandleAuthDataRequest(vecFrame, pktTypeParam);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_AUTH_REQ No Valid Data!\n");
            }
        }
        else if (PACKET_TYPE_AUTH_RES == pktType)
        {
            FrameData vecFrame;
            BYTE pktTypeParam = 0;
            if (CPacketUtil::GetPacketDataAUTH(packet, vecFrame, pktTypeParam) > 0)
            {
                HandleAuthDataResponse(vecFrame, pktTypeParam);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_AUTH_RES No Valid Data!\n");
            }
        }
        else if (PACKET_TYPE_CAN_AUTO_SEND == pktType)
        {
            LOG_INFO("Recvied Autosend data, drop packet!\n");
        }
        else if (PACKET_TYPE_BUS_USAGE == pktType)
        {
            PacketDataBusUsage pktBusUsage;
            if (CPacketUtil::GetPacketDataBusUsage(packet, pktBusUsage) > 0)
            {
                HandleBusUsageFrame(pktBusUsage);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_BUS_USAGE No Valid Data!\n");
            }
        }
        else if (PACKET_TYPE_DEV_REQ == pktType)
        {
            //设备发送请求，库进行模拟响应
            FrameData vecFrame;
            BYTE pktTypeParam = 0;
            if (CPacketUtil::GetPacketDataDevReq(packet, vecFrame, pktTypeParam) > 0)
            {
                HandleDevReqFrame(vecFrame, pktTypeParam);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_DEV_REQ No Valid Data!\n");
            }
        }
        else if (PACKET_TYPE_DEV_RES == pktType)
        {
            //库发送请求，设备进行响应
            FrameData vecFrame;
            BYTE pktTypeParam = 0;
            if (CPacketUtil::GetPacketDataDevRes(packet, vecFrame, pktTypeParam) > 0)
            {
                HandleDevResFrame(vecFrame, pktTypeParam);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_DEV_RES No Valid Data!\n");
            }
        }
        else if (PACKET_TYPE_REQUEST_RESPONSE == pktType)
        {
            //客户端请求设备数据
            PacketTypeRequestResponseParam pktTypeParam;
            pktTypeParam.rawValue = CPacketUtil::GetPacketFrameTypeParam(packet);
            if (pktTypeParam.unionValue.bResponse)
            {
                //响应包,设备返回的数据
                PacketDataResponse responseData;
                if (CPacketUtil::GetPacketDataResponse(packet, responseData) > 0)
                {
                    HandleResponseData(pktTypeParam.unionValue.nSeq, responseData);
                }
            }
            else
            {
                //请求包，自己作为Server时，客户端过来请求数据
                PacketDataRequest requestData;
                if (CPacketUtil::GetPacketDataRequest(packet, requestData) > 0)
                {
                    HandleRequestData(pktTypeParam.unionValue.nSeq, requestData);
                }
            }
        }
        else if (PACKET_TYPE_DEV_STATE == pktType)
        {
            //设备周期上报的状态数据
            FrameData vecFrame;
            BYTE pktTypeParam = CPacketUtil::GetPacketFrameTypeParam(packet);
            if (CPacketUtil::GetPacketDataDevState(packet, vecFrame, pktTypeParam) > 0)
            {
                HandleDevStateData(vecFrame, pktTypeParam);
            }
        }
        else if (PACKET_TYPE_GPS == pktType)
        {
            //gps数据
            std::vector<PacketDataGPS> vecFrame;
            if (CPacketUtil::GetPacketDataGPS(packet, vecFrame) > 0)
            {
                HandleGPSFrame(vecFrame);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_GPS No Valid Data!\n");
            }
        }
        else if (PACKET_TYPE_LIN == pktType)
        {
            //LIN数据
            std::vector<PacketDataLIN> vecFrame;
            if (CPacketUtil::GetPacketDataLIN(packet, vecFrame) > 0)
            {
                HandleLINFrame(vecFrame);
            }
            else
            {
                LOG_ERR("PACKET_TYPE_LIN No Valid Data!\n");
            }
        }
        else
        {
            LOG_ERR("Unkown Packet Type, drop packet!\n");
        }
    }
    //LOG_INFO("ProcessPackets done\n");
}

void Device::HandleCANFrame(const std::vector<PacketDataCAN>& vecFrame)
{
    bool bStartfor1Second = IsDevStartLessThan(1000);
    for (auto & pkt : vecFrame)
    {
        if (pkt.canHead.nChnl < m_devChnlCount)
        {
            if (m_bitChnlStarted.test(pkt.canHead.nChnl) || bStartfor1Second)
            {
                //判断是否ZCANPRO发送的请求回显帧
                HandleEchoRequestFrame(pkt);

                if (m_bRecvMerge)
                {
                    CanDataQueue<ZCANDataObj>& canData = m_frmMerge;
                    ZCANDataObj dataObj;
                    if (CPacketUtil::PacketData2ZCANDataObj(pkt, dataObj))
                    {
                        canData.lock();
                        if (canData.push(dataObj))
                        {
                            m_statistic.nRxMergeDrop++;
                        }
                        canData.unlock();
                        m_statistic.nRxMerge++;
                    }
                }
                else
                {
                    ZCAN_CHANNEL_ERR_INFO errInfo{ 0 };
                    if (CPacketUtil::CANFrame2ErrFrame(pkt, errInfo))
                    {
                        CanDataQueue<ZCAN_CHANNEL_ERR_INFO>& errData = m_errQueue[pkt.canHead.nChnl];
                        errData.lock();
                        errData.push(errInfo);
                        errData.unlock();
                        m_chnlStatus[pkt.canHead.nChnl].regRECounter = errInfo.passive_ErrData[1];
                        m_chnlStatus[pkt.canHead.nChnl].regTECounter = errInfo.passive_ErrData[2];
                        m_statistic.chnl[pkt.canHead.nChnl].nError++;
                    }
                    else
                    {
                        CanDataQueue<PacketDataCAN>& canData = m_frmCAN[pkt.canHead.nChnl];
                        canData.lock();
                        if (canData.push(pkt))
                        {
                            m_statistic.chnl[pkt.canHead.nChnl].nRxCANDrop++;
                        }
                        canData.unlock();
                        m_statistic.chnl[pkt.canHead.nChnl].nRxCAN++;
                    }
                }

                //统计信息
                if (pkt.canHead.frameInfo.unionVal.nTx)
                {
                    m_statistic.chnl[pkt.canHead.nChnl].nTxEcho++;
                }
            }
            else
            {
                LOG_ERR("CAN Data Ingored, Chnl:%d, started:%d StartLess1S:%d\n", pkt.canHead.nChnl, m_bitChnlStarted.test(pkt.canHead.nChnl), bStartfor1Second);
            }
        }
        else
        {
            LOG_ERR("CAN Invalid pkt.canHead.nChnl : %d\n", pkt.canHead.nChnl);
        }
    }
}

void Device::HandleCANFDFrame(const std::vector<PacketDataCANFD>& vecFrame)
{
    bool bStartfor1Second = IsDevStartLessThan(1000);
    for (auto & pkt : vecFrame)
    {
        if (pkt.canHead.nChnl < m_devChnlCount)
        {
            if (m_bitChnlStarted.test(pkt.canHead.nChnl) || bStartfor1Second)
            {
                //判断是否ZCANPRO发送的请求回显帧
                HandleEchoRequestFrame(pkt);

                if (m_bRecvMerge)
                {
                    CanDataQueue<ZCANDataObj>& canData = m_frmMerge;
                    ZCANDataObj dataObj;
                    if (CPacketUtil::PacketData2ZCANDataObj(pkt, dataObj))
                    {
                        canData.lock();
                        if (canData.push(dataObj))
                        {
                            m_statistic.nRxMergeDrop++;
                        }
                        canData.unlock();
                        m_statistic.nRxMerge++;
                    }
                }
                else
                {
                    ZCAN_CHANNEL_ERR_INFO errInfo{ 0 };
                    if (CPacketUtil::CANFrame2ErrFrame(pkt, errInfo))
                    {
                        CanDataQueue<ZCAN_CHANNEL_ERR_INFO>& errData = m_errQueue[pkt.canHead.nChnl];
                        errData.lock();
                        errData.push(errInfo);
                        errData.unlock();
                        m_chnlStatus[pkt.canHead.nChnl].regRECounter = errInfo.passive_ErrData[1];
                        m_chnlStatus[pkt.canHead.nChnl].regTECounter = errInfo.passive_ErrData[2];
                        m_statistic.chnl[pkt.canHead.nChnl].nError++;
                    }
                    else
                    {
                        //CANFD包里可能有CAN数据
                        if (pkt.canHead.frameInfo.unionVal.bFD)
                        {
                            CanDataQueue<PacketDataCANFD>& canData = m_frmCANFD[pkt.canHead.nChnl];
                            canData.lock();
                            if (canData.push(pkt))
                            {
                                m_statistic.chnl[pkt.canHead.nChnl].nRxCANFDDrop++;
                            }
                            canData.unlock();
                            m_statistic.chnl[pkt.canHead.nChnl].nRxCANFD++;
                        }
                        else
                        {
                            PacketDataCAN canpkt;
                            CPacketUtil::CANFDFrame2CANFrame(pkt, canpkt);
                            CanDataQueue<PacketDataCAN>& canData = m_frmCAN[pkt.canHead.nChnl];
                            canData.lock();
                            if (canData.push(canpkt))
                            {
                                m_statistic.chnl[pkt.canHead.nChnl].nRxCANDrop++;
                            }
                            canData.unlock();
                            m_statistic.chnl[pkt.canHead.nChnl].nRxCAN++;
                        }
                    }
                }

                //统计信息
                if (pkt.canHead.frameInfo.unionVal.nTx)
                {
                    m_statistic.chnl[pkt.canHead.nChnl].nTxEcho++;
                }
            }
            else
            {
                //LOG_ERR("CANFD Data Ingored, Chnl:%d, started:%d StartLess1S:%d\n", pkt.canHead.nChnl, m_bitChnlStarted.test(pkt.canHead.nChnl), bStartfor1Second);
            }
        }
        else
        {
            LOG_ERR("CANFD Invalid pkt.canHead.nChnl : %d\n", pkt.canHead.nChnl);
        }
    }
}

void Device::HandleGPSFrame(const std::vector<PacketDataGPS>& vecFrame)
{
    if (m_bRecvMerge)
    {
        CanDataQueue<ZCANDataObj>& canData = m_frmMerge;
        ZCANDataObj dataObj;
        canData.lock();
        for (auto & pkt : vecFrame)
        {
            CPacketUtil::PacketData2ZCANDataObj(pkt, dataObj);
            if (canData.push(dataObj))
            {
                m_statistic.nRxMergeDrop++;
            }
            m_statistic.nRxMerge++;
        }
        canData.unlock();
    }
    else
    {
        CanDataQueue<PacketDataGPS>& gpsData = m_frmGPS;
        gpsData.lock();
        for (auto & pkt : vecFrame)
        {
            gpsData.push(pkt);
            m_statistic.nGPS++;
        }
        gpsData.unlock();
    }
}

void Device::HandleLINFrame(const std::vector<PacketDataLIN>& vecFrame)
{
    if (m_bRecvMerge)
    {
        CanDataQueue<ZCANDataObj>& canData = m_frmMerge;
        ZCANDataObj dataObj;
        canData.lock();
        for (auto & pkt : vecFrame)
        {
            CPacketUtil::PacketData2ZCANDataObj(pkt, dataObj);
            if (canData.push(dataObj))
            {
                m_statistic.nRxMergeDrop++;
            }
            m_statistic.nRxMerge++;
        }
        canData.unlock();
    }
    else
    {
        for (auto & pkt : vecFrame)
        {
            if (pkt.nChnl < DEV_LIN_COUNT_MAX)
            {
                CanDataQueue<PacketDataLIN>& data = m_frmLIN[pkt.nChnl];
                data.lock();
                data.push(pkt);
                data.unlock();
                m_statistic.chnl[pkt.nChnl].nRxLIN++;
            }
        }
    }
}

void Device::HandleAuthDataResponse(const FrameData& vecAuth, BYTE pktTypeParam)
{
    std::unique_lock<std::mutex> lk_(m_authMapMutex);
    auto itr = m_authDataMap.find(pktTypeParam);
    if (itr != m_authDataMap.end())
    {
        AuthDeviceData_PTR authData = itr->second;
        lk_.unlock();
        memcpy(authData->data.OutData, vecAuth.data(), FRAME_LEN_AUTH);
        authData->cv.notify_all();
        LOG_INFO("Auth Data Response Recieved:%d\n", pktTypeParam);
    }
    else
    {
        LOG_ERR("Auth Data Response Dropped:%d\n", pktTypeParam);
    }
}

void Device::HandleAuthDataRequest(const FrameData& vecAuth, BYTE pktTypeParam)
{
    LOG_INFO("Auth Data Request Recieved:%d\n", pktTypeParam);
}

void Device::HandleBusUsageFrame(const PacketDataBusUsage& pktBusUsage)
{
    if (pktBusUsage.nChnl < m_devChnlCount)
    {
        m_busUsage.chnl[pktBusUsage.nChnl] = pktBusUsage;
        m_busUsage.newdata[pktBusUsage.nChnl] = true;
        //LOG_INFO("Chnl:%d BusUsage:%d Count:%d Time%I64u-%I64u\n", pktBusUsage.nChnl, pktBusUsage.nBusUsage, pktBusUsage.nFrameCount, pktBusUsage.nTimeStampBegin, pktBusUsage.nTimeStampEnd);
    }
	if (m_bRecvMerge)
	{
		//新增通道利用率缓存
		CanDataQueue<ZCANDataObj>& canData = m_frmMerge;
		ZCANDataObj dataObj;
		if (CPacketUtil::PacketData2ZCANDataObj(pktBusUsage, dataObj))
		{
			canData.lock();
			if (canData.push(dataObj))
			{
				m_statistic.nRxMergeDrop++;
			}
			canData.unlock();
			m_statistic.nRxMerge++;
		}
	}
}

void Device::HandleDevReqFrame(const FrameData& vecDevReq, BYTE pktTypeParam)
{
    PacketTypeREQParam reqParam;
    reqParam.rawValue = pktTypeParam;
    if (reqParam.unionValue.nReqVal == PACKET_TYPE_PARAM_DEV_REQ_RES_TIMESTAMP)
    {
        //客户模拟发送请求，直接发送响应表示成功。
        PacketTypeRESParam resParam;
        resParam.rawValue = 0;
        resParam.unionValue.nReqVal = reqParam.unionValue.nReqVal;
        resParam.unionValue.nResult = 1; // 1:表示执行成功
        Packet packet = CPacketEncoder::BuildPacketDevReqRes(false, nullptr, 0, pktTypeParam);
        SendData(packet.data(), packet.size());
    }
}

void Device::HandleDevResFrame(const FrameData& vecDevRes, BYTE pktTypeParam)
{
    PacketTypeRESParam param;
    param.rawValue = pktTypeParam;
    if (param.unionValue.nReqVal == PACKET_TYPE_PARAM_DEV_REQ_RES_TIMESTAMP)
    {
        // param.unionValue.nResult 表示设备响应结果，1：成功，0：失败
        // param.unionValue.nResult;
    }
}

void Device::HandleEchoRequestFrame(const PacketDataCAN& canFrame)
{
    LOG_INFO("CAN received, .chnl=%d .tx=%d .echo=%d\n", canFrame.canHead.nChnl, canFrame.canHead.frameInfo.unionVal.nTx, canFrame.canHead.frameInfo.unionVal.nEcho);
    if (m_bEchoResponse && canFrame.canHead.frameInfo.unionVal.nEcho)
    {
        Packet packet = CPacketEncoder::BuildPacketEchoResponse(canFrame);
        if (packet.size() == SendData(packet.data(), packet.size()))
        {
            m_statistic.chnl[canFrame.canHead.nChnl].nTxCANSim++;
        }
    }
}

void Device::HandleEchoRequestFrame(const PacketDataCANFD& canFrame)
{
    LOG_INFO("CANFD received, .chnl=%d .tx=%d .echo=%d\n", canFrame.canHead.nChnl, canFrame.canHead.frameInfo.unionVal.nTx, canFrame.canHead.frameInfo.unionVal.nEcho);
    if (m_bEchoResponse && canFrame.canHead.frameInfo.unionVal.nEcho)
    {
        Packet packet = CPacketEncoder::BuildPacketEchoResponse(canFrame);
        if (packet.size() == SendData(packet.data(), packet.size()))
        {
            if (canFrame.canHead.frameInfo.unionVal.bFD)
            {
                m_statistic.chnl[canFrame.canHead.nChnl].nTxCANFDSim++;
            }
            else
            {
                m_statistic.chnl[canFrame.canHead.nChnl].nTxCANSim++;
            }
        }
    }
}

//PktType 0x50数据请求响应
void Device::HandleRequestData(BYTE nSeq, PacketDataRequest& requestData)
{
    //库接收到其他客户端的请求，模拟设备发出响应
    Packet packet = CPacketEncoder::BuildPacketEmulateResponse(nSeq, requestData);
    SendData(packet.data(), packet.size());
}

//PktType 0x50数据请求响应
void Device::HandleResponseData(BYTE nSeq, PacketDataResponse& responseData)
{
    std::unique_lock<std::mutex> lk_(m_pktRequestMutex);
    int key = GetDevRequestKey(nSeq, responseData.nTID);
    auto itr = m_pktRequestDataMap.find(key);
    if (itr != m_pktRequestDataMap.end())
    {
        DevRequestResponse_PTR pktRequest = itr->second;
        lk_.unlock();
        pktRequest->responseData = responseData;
		pktRequest->responsed = true;
        pktRequest->cv.notify_all();
        LOG_INFO("Dev Request Response Recieved, seq:%d tid:%d\n", nSeq, responseData.nTID);
    }
    else
    {
        LOG_ERR("Dev Request Response Dropped, seq:%d tid:%d \n", nSeq, responseData.nTID);
    }
}

//PktType 0x04 设备状态数据
void Device::HandleDevStateData(const FrameData& vecDevState, BYTE pktTypeParam)
{
    switch (pktTypeParam)
    {
    case PACKET_TYPE_DEV_STATE_PARAM_SYS_INFO:
        {
            m_stateSysInfo.strState.assign(vecDevState.begin(), vecDevState.end());
            m_stateSysInfo.bNewData = true;
        }
        break;
    case PACKET_TYPE_DEV_STATE_PARAM_CAN_INFO:
        {
            m_stateCANInfo.strState.assign(vecDevState.begin(), vecDevState.end());
            m_stateCANInfo.bNewData = true;
        }
        break;
    case PACKET_TYPE_DEV_STATE_PARAM_RECORDER_INFO:
        {
            m_stateRecorderInfo.strState.assign(vecDevState.begin(), vecDevState.end());
            m_stateRecorderInfo.bNewData = true;
        }
        break;
    case PACKET_TYPE_DEV_STATE_PARAM_NET_INFO:
        {
            m_stateNetInfo.strState.assign(vecDevState.begin(), vecDevState.end());
            m_stateNetInfo.bNewData = true;
        }
        break;
    default:
        break;
    }

}

bool Device::UDSRequest(UINT reqID, const std::string& reqParam, const BYTE* reqData, UINT reqDataLen, 
                     std::string& respParam, std::vector<BYTE> &respData)
{
    PacketDataRequest request;
    request.nRequest = PACKET_REQUEST_DO_UDS_REQUEST;
    int reqParamLen = reqParam.length();
    request.vecData.resize(reqParamLen + 1 + reqDataLen, 0);
    memcpy(&request.vecData[0], reqParam.c_str(), reqParamLen);
    request.vecData[reqParamLen] = 0;
    if (reqData && reqDataLen > 0) {
        memcpy(&request.vecData[reqParamLen + 1], reqData, reqDataLen);
    }

    bool bRet = false;
    m_pktRequestSeq = (++m_pktRequestSeq) % 8;
    m_pktTID++;

    BYTE nSeq = m_pktRequestSeq;
    BYTE nTID = m_pktTID;

    request.nTID = nTID;
    DevRequestResponse_PTR data = std::make_shared<DevRequestResponse>();
    if (data)
    {
        data->nSeq = nSeq;
        data->nTID = nTID;
        data->requestData = request;
    }
    else
    {
        LOG_ERR("memory is full! \n");
        return false;;
    }

    int key = GetDevRequestKey(nSeq, nTID);
    Packet packet = CPacketEncoder::BuildPacketRequest(nSeq, request);
    LOG_INFO("Request Dev Data request:%d, seq:%d, tid:%d\n", request.nRequest, nSeq, nTID);

	//wait for response
	{
		std::unique_lock<std::mutex> lk_(m_pktRequestMutex);
		m_pktRequestDataMap[key] = data;
	}
    if (packet.size() != SendData(packet.data(), packet.size())) {
        LOG_ERR("UDS_Request send data error! \n");
    }
    else {
        int maxTryCount = 200; // 300s
        while (maxTryCount--)
        {
            std::mutex mutex_;
            std::unique_lock<std::mutex> ulk(mutex_);
            if (data->cv.wait_for(ulk, std::chrono::milliseconds(REQUEST_TIMEOUT), [&data]{
                return data->responsed; }))
            {
                if (data->responseData.nResult == 0) {
                    LOG_ERR("UDSRequest response result error\n");
                    break;
                }

                if (data->responseData.vecData.empty()) {
                    LOG_ERR("UDSRequest response data empty\n");
                    break;
                }

                size_t i=0;
                for (i=0; data->responseData.vecData.size(); i++) {
                    if (data->responseData.vecData[i] == 0) {
                        break;
                    }
                }

                if (i >= data->responseData.vecData.size()) {
                    LOG_ERR("UDSRequest response data format error\n");
                    break;
                }

                respParam = (char*)&data->responseData.vecData[0];
                respData.clear();
                if (i < data->responseData.vecData.size() - 1) {
                    respData.assign(data->responseData.vecData.begin() + i + 1, data->responseData.vecData.end());
                }

                bRet = true;
                LOG_INFO("Response data, seq:%d tid:%d/%d\n", nSeq, nTID, data->responseData.nTID);
                break;
            }
            else
            {
                // 超时后获取UDS当前状态，如果还在请求中，则继续等待
                UDS_STATUS status = UDS_STATUS_IDLE;
                if (!GetUdsStatus(reqID, status)) {
                    LOG_ERR("GetUdsStatus failed\n");
                    break;
                }

                if (status != UDS_STATUS_REQUESTING) {
                    LOG_ERR("uds status is not requesting\n");
                    break;
                }
            }
        }
    }

    //Clear result
    {
        std::lock_guard<std::mutex> lk_(m_pktRequestMutex);
        m_pktRequestDataMap.erase(key);
    }

    return bRet;
}

bool Device::UDSControl(const string& req, string &resp)
{
    PacketDataRequest request;
    PacketDataResponse response;
    request.nRequest = PACKET_REQUEST_DO_UDS_CONTROL;
    request.nTID = 0;
    request.vecData.assign(req.begin(), req.end());

    if (!RequestDevData(request, response)) {
        return false;
    }
    
    if (response.nResult == 0) {
        LOG_ERR("UDS_Control response result error\n");
        return false;
    }

    if (response.vecData.empty()) {
        resp = nullptr;
    }
    else {
        char* strResult = new char[response.vecData.size()]; 
        memcpy(strResult, (char*)&response.vecData[0], response.vecData.size());
        resp = strResult;
    }

    return true;
}

bool Device::GetUdsStatus(UINT reqId, UDS_STATUS &status)
{
    PacketDataRequest request;
    PacketDataResponse response;
    request.nRequest = PACKET_REQUEST_GET_UDS_STATUS;
    request.nTID = 0;

    cJSON* pObj = cJSON_CreateObject();
    cJSON_AddNumberToObject(pObj, "ReqID", reqId);
    char* pStr = cJSON_PrintUnformatted(pObj);
    std::string strConfig(pStr);
    request.vecData.assign(strConfig.c_str(), strConfig.c_str() + strConfig.length());
    cJSON_free(pStr);
    cJSON_Delete(pObj);

    if (!RequestDevData(request, response)) {
        return false;
    }
    
    if (response.nResult == 0) {
        LOG_ERR("GetUdsStatus result error\n");
        return false;
    }

    std::string strResult(response.vecData.begin(), response.vecData.end());
    cJSON* pInfo = cJSON_Parse(strResult.c_str());
    if (!pInfo) {
        LOG_ERR("GetUdsStatus response json parse error\n");
        return false;
    }
    
    status = (UDS_STATUS)(int)UtilGetcJSONNumber(cJSON_DetachItemFromObjectCaseSensitive(pInfo, "Status"));
    cJSON_Delete(pInfo);
    
    return true;
}


ZCAN_RET_STATUS Device::UDS_Request(const ZCAN_UDS_REQUEST* req, ZCAN_UDS_RESPONSE* resp)
{
    if (!req) {
        return STATUS_ERR;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "ReqID", req->reqID);
    cJSON_AddNumberToObject(root, "Chn", req->channelIdx);
    cJSON_AddNumberToObject(root, "SrcAddr", req->src_addr);
    cJSON_AddNumberToObject(root, "DstAddr", req->dst_addr);
    cJSON_AddNumberToObject(root, "SuppressResp", req->suppress_response);
    cJSON_AddNumberToObject(root, "SID", req->sid);
    cJSON_AddNumberToObject(root, "DateLen", req->data_len);

    cJSON* sessionParamNode = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "SessionParam", sessionParamNode);
    cJSON_AddNumberToObject(sessionParamNode, "Timeout", req->session_param.timeout);
    cJSON_AddNumberToObject(sessionParamNode, "EnhancedTimeout", req->session_param.enhanced_timeout);
    cJSON_AddNumberToObject(sessionParamNode, "ChkAnyNegResp", req->session_param.check_any_negative_response);
    cJSON_AddNumberToObject(sessionParamNode, "WaitIfSuppressResp", req->session_param.wait_if_suppress_response);

    cJSON* transParamNode = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "ISO15765Param", transParamNode);
    cJSON_AddNumberToObject(transParamNode, "Version", req->trans_param.version);
    cJSON_AddNumberToObject(transParamNode, "MaxDataLen", req->trans_param.max_data_len);
    cJSON_AddNumberToObject(transParamNode, "LocalStMin", req->trans_param.local_st_min);
    cJSON_AddNumberToObject(transParamNode, "BlockSize", req->trans_param.block_size);
    cJSON_AddNumberToObject(transParamNode, "FillByte", req->trans_param.fill_byte);
    cJSON_AddNumberToObject(transParamNode, "FrameType", req->trans_param.frame_type);
    cJSON_AddNumberToObject(transParamNode, "IsModifyEcuStMin", req->trans_param.is_modify_ecu_st_min);
    cJSON_AddNumberToObject(transParamNode, "RemoteStMin", req->trans_param.remote_st_min);
    cJSON_AddNumberToObject(transParamNode, "FCTimeout", req->trans_param.fc_timeout);

    char* pStr = cJSON_PrintUnformatted(root);
    std::string reqParam(pStr);
    cJSON_free(pStr);
    cJSON_Delete(root);

    std::string respParam;
    std::vector<BYTE> respData;
    if (!UDSRequest(req->reqID, reqParam, req->data, req->data_len, respParam, respData)) {
        return STATUS_ERR;
    }

    // 无需解析响应
    if (!resp) {
        return STATUS_OK;
    }

    cJSON* respRoot = cJSON_Parse(respParam.c_str());
    if (!respRoot) {
        LOG_ERR("response json parse error\n");
        return STATUS_ERR;
    }
    
    memset(resp, 0, sizeof(resp));
    resp->status = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(respRoot, "Status"));
    resp->type = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(respRoot, "RespType"));

    if (resp->type == ZCAN_UDS_RT_POSITIVE) {
        resp->positive.sid = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(respRoot, "SID"));
        resp->positive.data_len = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(respRoot, "DateLen"));
        if (respData.size() > 0) {
            resp->positive.data = new BYTE[respData.size()];
            memcpy(resp->positive.data, &respData[0], respData.size());
        }
    }
    else {
        resp->negative.neg_code = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(respRoot, "NegCode"));
        resp->negative.sid = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(respRoot, "SID"));
        resp->negative.error_code = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(respRoot, "NegErr"));
    }

    cJSON_Delete(respRoot);

    return STATUS_OK;
}

ZCAN_RET_STATUS Device::UDS_Control(const ZCAN_UDS_CTRL_REQ *ctrl, ZCAN_UDS_CTRL_RESP* resp )
{
    
    if (!ctrl) {
        return STATUS_ERR;
    }

    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "ReqID", ctrl->reqID);
    cJSON_AddNumberToObject(root, "CtrlType", ctrl->cmd);

    char* pStr = cJSON_PrintUnformatted(root);
    std::string reqParam(pStr);
    cJSON_free(pStr);
    cJSON_Delete(root);

    std::string respParam;
    if (!UDSControl(reqParam, respParam)) {
        return STATUS_ERR;
    }

    // 无需解析响应
    if (!resp) {
        return STATUS_OK;
    }

    cJSON* respRoot = cJSON_Parse(respParam.c_str());
    if (!respRoot) {
        LOG_ERR("response json parse error\n");
        return STATUS_ERR;
    }
    
    memset(resp, 0, sizeof(resp));
    resp->result = (UINT)UtilGetcJSONNumber(cJSON_GetObjectItemCaseSensitive(respRoot, "Status"));
    cJSON_Delete(respRoot);

    return STATUS_OK;
}

void Device::UDS_Release(ZCAN_UDS_RESPONSE* data)
{
    if (!data) {
        return;
    }

    if (data->positive.data) {
        delete[] data->positive.data;
    }
}