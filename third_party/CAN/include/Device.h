#ifndef DEVICE_H_
#define DEVICE_H_

#include <string>
#include <map>
#include <vector>
#include <bitset>
#include <mutex>
#include <thread>
#include <queue>
#include <memory>
#include <chrono>
#include <condition_variable>
#include <stdlib.h>
#include "Service.h"
#include "NetPacket.h"
#include "CANFDNET.h"
#include "log.h"

#ifdef WIN32
#include <Windows.h>
#endif

struct cJSON;

#define DEV_MAX_CLIENTS             100
#define DEV_CHNL_COUNT_MAX          32
#define DEV_LIN_COUNT_MAX           4       //LIN Chnl Count Max
#define DEV_AUTO_SEND_INDEX_MAX     32      //AUTO SEND INDEX 0-31
#define DEV_AUTO_SEND_PERIOD_MAX    60000   //auto send period 60000ms

#define CAN_DATA_QUEUE_BUFF_SIZE    1024000 //队列最大容量，避免内存无限制的被消耗
#define CAN_DATA_QUEUE_BUFF_INIT    1000    //队列初始容量

#define USE_STL_DEQUE_BUFFER        0

#if USE_STL_DEQUE_BUFFER

template<class _Ty>
class CanDataQueue : public std::queue<_Ty>
{
public:

    CanDataQueue(UINT maxCount = CAN_DATA_QUEUE_BUFF_SIZE) {
        m_maxCount = maxCount;
#ifdef WIN32
        m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL );
#endif //WIN32
    }

    ~CanDataQueue() {
        clear();
#ifdef WIN32
        ::CloseHandle(m_hEvent);
        m_hEvent = NULL;
#endif //WIN32
    }

    _Ty front() {
        return std::queue<_Ty>::front();
    }

    //返回true表示添加新数据时，有旧数据被挤出去了
    bool push(const _Ty& _Val) {
        bool bRet = false;
        if (m_maxCount > 0 && this->size() >= m_maxCount) {
            std::queue<_Ty>::pop();
            bRet = true;
        }
        std::queue<_Ty>::push(_Val);
#ifdef WIN32
        ::SetEvent(m_hEvent);
#endif //WIN32

        return bRet;
    }

    void pop() {
        std::queue<_Ty>::pop();
#ifdef WIN32
        if (std::queue<_Ty>::empty()){
            ::ResetEvent(m_hEvent);
        }
#endif //WIN32
    }

    void clear() {
        lock();
        while (!std::queue<_Ty>::empty()) {
            std::queue<_Ty>::pop();
        }
#ifdef WIN32
        ::ResetEvent(m_hEvent);
#endif //WIN32
        unlock();
    }

    void lock(){
        m_lock.lock();
    }

    void unlock(){
        m_lock.unlock();
    }

#ifdef WIN32
    DWORD WaitEvent(DWORD dwTime)
    {
        return ::WaitForSingleObject(m_hEvent, dwTime);
    }
#endif //WIN32
private:
    std::recursive_mutex m_lock;
    UINT m_maxCount = 0;
#ifdef WIN32
    HANDLE m_hEvent;
#endif //WIN32
};

#else //USE_STL_DEQUE_BUFFER

/*
    20210420
    使用STL的双端队列，会导致没有消费者取数据时候，即使限制队列的中数据的数量(队列不停的pop和push)，
    队列占用的内存仍旧会不断增长，最终导致程序的崩溃。
    因此手工实现环形队列，为了避免队列总容量过大导致的内存浪费，设定队列的最大容量和初始容量，
    数据超过初始容量后，继续添加数据，队列容量会扩大一倍，直到扩充到最大容量后不再扩充，限制
    队列总内存的使用量。
*/
template <class T>
class CanDataQueue
{
private:
    size_t m_capacity_max;          //最大容量
    size_t m_capacity;              //当前容量，在达到最大容量之前，随着队列数据增多，会慢慢增加到最大容量
    size_t m_size;                  //队列当前数据量
    int m_front;                    //队列头索引，队列头负责数据出队列
    int m_rear;                     //队列尾索引，队列尾负责数据入队列
    T*  m_data;                     //队列数据实际存放的内存
    std::recursive_mutex m_lock;

#ifdef WIN32
    HANDLE m_hEvent;                //Windows平台下可以通过事件机制判断队列是否有数据
#endif //WIN32

public:
    CanDataQueue(size_t maxCount = CAN_DATA_QUEUE_BUFF_SIZE) :m_capacity(CAN_DATA_QUEUE_BUFF_INIT),
        m_capacity_max(maxCount),
        m_size(0),
        m_front(0),
        m_rear(0)
    {
        m_data = new T[m_capacity];

#ifdef WIN32
        m_hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
#endif //WIN32
    }

    ~CanDataQueue()
    {
        delete []m_data;
        m_data = nullptr;

#ifdef WIN32
        ::CloseHandle(m_hEvent);
        m_hEvent = NULL;
#endif //WIN32
    }

    bool isEmpty()
    {
        return m_size == 0;
    }

    bool isFull()
    {
        return m_size > 0 && m_size == m_capacity;
    }

    bool isMaxCapacity()
    {
        return m_capacity == m_capacity_max;
    }

    size_t size()
    {
        return m_size;
    }

    void reallocMemory()
    {
        if (!isMaxCapacity())
        {
            size_t size_1 = m_size;
            if (m_size < m_capacity_max / 2)
            {
                size_1 = m_size * 2;
            }
            else if (m_size < m_capacity_max)
            {
                size_1 = m_capacity_max;
            }

            if (size_1 != m_size)
            {
                #ifdef WIN32
				LOG_ALWAYS("tid = %u, realloc to %d, cap:%d cap_max:%d front:%d rear:%d size:%d \n", GetCurrentThreadId(), size_1, m_capacity, m_capacity_max, m_front, m_rear, m_size);
                #endif
                T * p = new T[size_1];
                if (p != nullptr)
                {
                    for (size_t i = 0; i < m_size; i++)
                    {
                        size_t dataIndex = (m_front + i) % m_capacity;
                        p[i] = m_data[dataIndex];
                    }
                    //将旧数据移到新的内存，重置front和rear
                    m_front = 0;
                    m_rear = m_size;
                    delete[] m_data;
                    m_data = p;
                    m_capacity = size_1;
                }
                else
                {
                    LOG_ERR("realloc failed\n");
                }
                #ifdef WIN32
				LOG_ALWAYS("tid = %u，realloc done, cap:%d cap_max:%d front:%d rear:%d size:%d \n", GetCurrentThreadId(), m_capacity, m_capacity_max, m_front, m_rear, m_size);
                #endif
            }
        }
    }

    //返回true表示添加新数据时，有旧数据被挤出去了
    bool push(const T& ele)
    {
        bool bRet = false;
        if (isFull())
        {
            reallocMemory();
            if (isFull())
            {
                pop();
                bRet = true;
            }
        }
        m_data[m_rear] = ele;
        m_rear = (m_rear + 1) % m_capacity;
        m_size++;

#ifdef WIN32
        ::SetEvent(m_hEvent);
#endif //WIN32

        return bRet;
    }

    const T& front() {
        return m_data[m_front];
    }

    void pop()
    {
        if (isEmpty())
        {
            return;
        }
        m_front = (m_front + 1) % m_capacity;
        m_size--;

#ifdef WIN32
        if (isEmpty()){
            ::ResetEvent(m_hEvent);
        }
#endif //WIN32
    }

    void clear() {
        lock();
        m_front = m_rear = m_size = 0;

#ifdef WIN32
        ::ResetEvent(m_hEvent);
#endif //WIN32
        unlock();
    }

    void lock(){
        m_lock.lock();
    }

    void unlock(){
        m_lock.unlock();
    }

#ifdef WIN32
    DWORD WaitEvent(DWORD dwTime)
    {
        return ::WaitForSingleObject(m_hEvent, dwTime);
    }
#endif //WIN32
};

#endif //USE_STL_DEQUE_BUFFER

struct AuthDeviceData
{
    VerifyDeviceData        data;
    std::condition_variable cv;
};
typedef std::shared_ptr<AuthDeviceData> AuthDeviceData_PTR;

struct ChnlStatistic
{
    UINT64  nRxCAN;
    UINT64  nRxCANDrop;
    UINT64  nRxCANFD;
    UINT64  nRxCANFDDrop;
    UINT64  nTxCAN;
    UINT64  nTxCANFD;
    UINT64  nTxCANSim;
    UINT64  nTxCANFDSim;
    UINT64  nError;
    UINT64  nTxEcho;
    UINT64  nTxLIN;
    UINT64  nRxLIN;
    UINT64  nRxMerge;
    UINT64  nRxMergeDrop;
    UINT64  nAppRxMerge;
};

struct DevStatistic
{
    ChnlStatistic   chnl[DEV_CHNL_COUNT_MAX];
    UINT64          nGPS;
    UINT64          nTxBytesSuccess;
    UINT64          nTxBytesFailed;
    UINT64          nRxBytes;
    UINT64          nTxMerge;       //库发送的合并数据帧
    UINT64          nRxMerge;       //库接收到的合并数据帧
    UINT64          nRxMergeDrop;   //合并接收丢弃的数据
    UINT64          nAppRxMerge;    //APP通过接口拿到的合并数据帧
};

//设备上报的状态信息数据
struct DevStateData
{
    std::string     strState;    // 上报的信息是json字符串
    bool            bNewData;    // 是否有新数据

    DevStateData()
    {
        bNewData = false;
        strState.clear();
    }
};

struct DevBusUsage
{
    PacketDataBusUsage chnl[DEV_CHNL_COUNT_MAX];
    bool               newdata[DEV_CHNL_COUNT_MAX];
};

//PktType 0x50 请求应答数据
struct DevRequestResponse
{
    BYTE                nSeq;
    BYTE                nTID;
    std::condition_variable cv;
    PacketDataRequest   requestData;
    PacketDataResponse  responseData;
	bool responsed = false;
};
typedef std::shared_ptr<DevRequestResponse> DevRequestResponse_PTR;

struct DevChnlAutoSendList
{
    struct {
        std::chrono::steady_clock::time_point   m_tpRecv;   //获取设备数据时间，如果短时间内多次请求，则直接使用缓存的数据
        std::vector<ZCAN_AUTO_TRANSMIT_OBJ>     vecCAN;     //CAN定时发送列表
        std::vector<ZCANFD_AUTO_TRANSMIT_OBJ>   vecCANFD;   //CANFD定时发送列表
    }   chnl[DEV_CHNL_COUNT_MAX];
};

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagCANFDNetVersion
{
    struct {
        USHORT nRevision : 8;
        USHORT nMinor: 4;
        USHORT nMajor : 3;
        USHORT nType : 1;       //最高位，0表示2段式，1表示3段式,CANFDNET系列固定3段式
    } st3Version;
    USHORT nRawVersion;
#ifdef _WIN32
} CANFDNetVersion;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) CANFDNetVersion;
#endif

class Device
{
public:
    Device(UINT devType, UINT devIdx, UINT Reserved = 0);
    ~Device();

    bool GetReference(UINT chnIdx, UINT refType, void* pData);
    bool SetReference(UINT chnIdx, UINT refType, void* pData);
    ULONG GetReceiveNum(UINT chnIdx, BYTE type); //type:TYPE_CAN TYPE_CANFD
    void  ClearBuffer(UINT chnIdx);
    ULONG Transmit(UINT chnIdx, const ZCAN_Transmit_Data* pSend, ULONG len);
    ULONG TransmitFD(UINT chnIdx, const ZCAN_TransmitFD_Data* pSend, ULONG len);
    ULONG TransmitData(const ZCANDataObj* pSend, ULONG len);
    ULONG Receive(UINT chnIdx, ZCAN_Receive_Data* pReceive, ULONG len, int waitTime = -1);
    ULONG ReceiveFD(UINT chnIdx, ZCAN_ReceiveFD_Data* pReceive, ULONG len, int waitTime = -1);
    ULONG ReceiveData(ZCANDataObj* pReceive, ULONG len, int waitTime = -1);
    bool  GetChnlErrorInfo(UINT chnIdx, ZCAN_CHANNEL_ERR_INFO* pError);
    bool  GetChnlStatus(UINT chnIdx, ZCAN_CHANNEL_STATUS * pCANStatus);
    bool  GetDeviceInfo(ZCAN_DEVICE_INFO * pInfo);
    bool  SetAutoSend(UINT chnIdx, const ZCAN_AUTO_TRANSMIT_OBJ* pObj);
    bool  SetAutoSend(UINT chnIdx, const ZCANFD_AUTO_TRANSMIT_OBJ* pObj);
    bool  SetAutoSend(UINT chnIdx, VecAutoSendData& vecAutoSend);

    bool StartCAN(UINT chnIdx);
    bool StopCAN(UINT chnIdx);
    bool Stop();

    //LIN 
    bool   InitLIN(UINT chnIdx, PZCAN_LIN_INIT_CONFIG pLINInitConfig);
    UINT   StartLIN(UINT chnIdx);
    UINT   ResetLIN(UINT chnIdx);
    UINT   TransmitLIN(UINT chnIdx, PZCAN_LIN_MSG pSend, UINT Len);
    UINT   GetLINReceiveNum(UINT chnIdx);
    UINT   ReceiveLIN(UINT chnIdx, PZCAN_LIN_MSG pReceive, UINT Len, int WaitTime);
    UINT   SetLINSlaveMsg(UINT chnIdx, PZCAN_LIN_MSG pSend, UINT nMsgCount);
    UINT   ClearLINSlaveMsg(UINT chnIdx, BYTE* pLINID, UINT nIDCount);
    UINT   SendLINMsgs(UINT chnIdx, PZCAN_LIN_MSG pSend, UINT Len);

    // UDS
    ZCAN_RET_STATUS UDS_Request(const ZCAN_UDS_REQUEST* req, ZCAN_UDS_RESPONSE* resp);
    ZCAN_RET_STATUS UDS_Control(const ZCAN_UDS_CTRL_REQ *ctrl, ZCAN_UDS_CTRL_RESP* resp);
    static void UDS_Release(ZCAN_UDS_RESPONSE* data);

public:
    void DataProcessThdLoop();
    void ProcessPackets(VecPackets& vecPackets);
    bool CreateAndStartService();

private:
    void DealReceivedData(const char* src_addr, const char* data, unsigned int len);
    void HandleCANFrame(const std::vector<PacketDataCAN>& vecFrame);
    void HandleCANFDFrame(const std::vector<PacketDataCANFD>& vecFrame);
    void HandleGPSFrame(const std::vector<PacketDataGPS>& vecFrame);
    void HandleLINFrame(const std::vector<PacketDataLIN>& vecFrame);
    void HandleAuthDataResponse(const FrameData& vecAuth, BYTE pktTypeParam);
    void HandleAuthDataRequest(const FrameData& vecAuth, BYTE pktTypeParam);
    void HandleBusUsageFrame(const PacketDataBusUsage& pktBusUsage);
    void HandleDevReqFrame(const FrameData& vecDevReq, BYTE pktTypeParam);
    void HandleDevResFrame(const FrameData& vecDevRes, BYTE pktTypeParam);

    //响应回显帧
    void HandleEchoRequestFrame(const PacketDataCAN& canFrame);
    void HandleEchoRequestFrame(const PacketDataCANFD& canFrame);

    //PktType 0x50数据请求响应
    void HandleRequestData(BYTE nSeq, PacketDataRequest& requestData);
    void HandleResponseData(BYTE nSeq, PacketDataResponse& responseData);

    //PktType 0x04 设备状态数据
    void HandleDevStateData(const FrameData& vecDevState, BYTE pktTypeParam);

    unsigned int SendData(const BYTE* data, unsigned int len);
    bool VerifyDeviceByPass(VerifyDeviceData& data);
    bool _StopAndClear();
    bool AddAutoSend(UINT chnIdx, const ZCAN_AUTO_TRANSMIT_OBJ* pObj);
    bool AddAutoSend(UINT chnIdx, const ZCANFD_AUTO_TRANSMIT_OBJ* pObj);
    bool ApplyAutoSend(UINT chnIdx);
    bool ClearAutoSend(UINT chnIdx);
    AutoSendData* GetAutoSendIndex(VecAutoSendData& vecAutoSend, UINT autoSendIndex);
    void ZCANAutoSend2AutoSend(const ZCAN_AUTO_TRANSMIT_OBJ* pObj, AutoSendData& autoSend);
    void ZCANAutoSend2AutoSend(const ZCANFD_AUTO_TRANSMIT_OBJ* pObj, AutoSendData& autoSend);
    bool IsDevStartLessThan(long long ms);
    bool SetDevTimeStamp(UINT64 nTimeStampUs);
    UINT GetTxEchoTimeStamp(UINT chnIdx, TxTimeStamp& rTxTimeStamp);
    bool GetBusUsage(UINT chnIdx, BusUsage& rBusUsage);

    bool GetDelaySendAvailableTxCount(UINT chnIdx, UINT& nCount);
    bool GetDevAutoSendListCount(UINT chnIdx, bool bCANFD, UINT& nCount);
    bool GetDevAutoSendListData(UINT chnIdx, bool bCANFD, void* pData);

    bool RequestDevData(PacketDataRequest& devRequest, PacketDataResponse& devResponse);

    int  GetDevRequestKey(BYTE pktSeq, BYTE nTID);
    void GenGetDevInfoRequest(PacketDataRequest& devRequest);
    void GenGetDevStateRequest(BYTE nType, PacketDataRequest& devRequest);
    void GenGetAutoSendListRequest(UINT chnIdx, UINT nOffset, UINT nRequestCount, PacketDataRequest& devRequest);
    void GenGetDevDelaySendRequest(UINT chnIdx, PacketDataRequest& devRequest);
    void GenClearDelaySendRequest(UINT chnIdx, PacketDataRequest& devRequest);

    bool GetDevInfo(std::string& strDevInfo);
    bool GetDevState(BYTE nType, std::string& strDevState);     //根据类型主动获取设备状态信息 nType取值为 PACKET_TYPE_DEV_STATE_PARAM_SYS_INFO 等数值
    bool GetDevAutoSendList(UINT chnIdx, std::vector<PacketDataAutoSend>& vecAutoSend);
    bool GetDelaySendInfo(UINT chnIdx, std::string& strDelaySendInfo);
    bool ClearDelaySendQueue(UINT chnIdx);

    bool GetDevInfoFromJsonStr(const std::string& strDevInfo, ZCAN_DEVICE_INFO * pInfo);
    UINT GetAutoSendListFromJsonStr(UINT chnIdx, UINT nOffset, UINT nRequestCount, const std::string& strAutoSendJson, std::vector<PacketDataAutoSend>& vecAutoSend);
    void GetAutoSendFromJson(UINT nChnIdx, UINT nIndex, cJSON* config, PacketDataAutoSend& autoSend);

    UINT GetGPSRecvNum();
    UINT GetGPSData(ZCAN_GPS_FRAME* pReceive, UINT len, int waitTime);

    bool GetDeviceInfo(ZCAN_DEVICE_INFO& devInfo);

    //LIN

    //用于LIN通道作为从机时，清空用户设置的应答数据
    bool ClearLINAsSlaveResponse(UINT chnIdx);
    bool GetLINTxFIFOInfo(UINT chnIdx, LINFIFOInfo& fifoInfo);
    bool SetLINConfig(UINT chnIdx, const LINConfig& linConfig);

    void GenClearLINAsSlaveResponseRequest(UINT chnIdx, PacketDataRequest& devRequest);
    void GenGetLINTxFIFOInfoRequest(UINT chnIdx, PacketDataRequest& devRequest);
    void GenSetLINConfigRequest(UINT chnIdx, const LINConfig& linConfig, PacketDataRequest& devRequest);
    bool GetFIFOInfoFromJsonStr(std::string& strFIFOInfo, LINFIFOInfo& fifoInfo);

    void StartDataProcessThread();
    void StopDataProcessThread();

    CPacketDecoder* GetDecoder(const char* rmt);

    // UDS 
    bool UDSRequest(UINT reqID, const std::string& reqParam, const BYTE* reqData, UINT reqDataLen, 
                    std::string& respParam, std::vector<BYTE> &respData);
    bool UDSControl(const std::string& req, std::string &resp);

    enum UDS_STATUS {
        UDS_STATUS_IDLE = 0,
        UDS_STATUS_REQUESTING = 1,
    };
    bool GetUdsStatus(UINT reqId, UDS_STATUS &status);

private:
    UINT m_devType = 0;
    UINT m_devIdx = 0;
    UINT m_devChnlCount = 2;
    UINT m_devLINCount = 4;
    UINT m_devTCP = 1;
    std::string m_devName = "CANFDNET";

    UINT m_workMode = 0;
    std::string m_destIp;
    USHORT m_destPort = 0;
    USHORT m_srcPort = 0;
    int    m_udpmulticastloopback = 1;
    IService* m_service = nullptr;

    std::bitset<DEV_CHNL_COUNT_MAX + DEV_LIN_COUNT_MAX>     m_bitChnlStarted;       //设备通道是否启动，包含CAN+LIN,CAN通道索引从0开始，LIN通道索引从 DEV_CHNL_COUNT_MAX开始
    std::chrono::steady_clock::time_point                   m_tpDevStartTickCount;  //设备建立连接时的时间，避免设备刚建立连接，通道尚未全部打开时设备立即发送数据可能造成的数据丢失。

    CanDataQueue<PacketDataCAN>             m_frmCAN[DEV_CHNL_COUNT_MAX];
    CanDataQueue<PacketDataCANFD>           m_frmCANFD[DEV_CHNL_COUNT_MAX];
    CanDataQueue<ZCAN_CHANNEL_ERR_INFO>     m_errQueue[DEV_CHNL_COUNT_MAX];

    CanDataQueue<PacketDataGPS>             m_frmGPS;
    CanDataQueue<ZCANDataObj>               m_frmMerge;     //合并接收数据
    bool                                    m_bRecvMerge;   //是否合并接收
    bool                                    m_bEchoResponse;//是否模拟请求端Echo响应

    CanDataQueue<PacketDataLIN>             m_frmLIN[DEV_LIN_COUNT_MAX];
    LINConfig                               m_linConfig[DEV_LIN_COUNT_MAX];

    ZCAN_CHANNEL_STATUS                     m_chnlStatus[DEV_CHNL_COUNT_MAX];

    VecAutoSendData                         m_autoSendData[DEV_CHNL_COUNT_MAX];

    std::vector<CPacketDecoder*>            m_vecDecoders;
    std::mutex                              m_recvDecodersMutex;
#ifdef WIN32
    HANDLE                                  m_hDecoderData;
#endif

    BYTE                                    m_authIncIndex{ 0 };
    std::map<BYTE, AuthDeviceData_PTR>      m_authDataMap;
    std::mutex                              m_authMapMutex;

    DevStatistic                            m_statistic;
    DevBusUsage                             m_busUsage;

    bool                                    m_bTxEcho;

    DevStateData                            m_stateSysInfo;         // pktType：0x04，pktTypeParam：0x01
    DevStateData                            m_stateCANInfo;         // pktType：0x04，pktTypeParam：0x02
    DevStateData                            m_stateRecorderInfo;    // pktType：0x04，pktTypeParam：0x03
    DevStateData                            m_stateNetInfo;         // pktType：0x04，pktTypeParam：0x04

    BYTE                                    m_pktRequestSeq{ 0 };   // pktType: 0x50 ,包序号，3Bit
    BYTE                                    m_pktTID{ 0 };          // pktType: 0x50 ,事务ID
    std::map<int, DevRequestResponse_PTR>   m_pktRequestDataMap;
    std::mutex                              m_pktRequestMutex;

    DevChnlAutoSendList                     m_devAutoSendList;      // 从设备查询的定时列表
    ZCAN_DEVICE_INFO                        m_stDevInfo;            // 设备信息字符，tcp客户端连接启动后会从设备获取，udp，tcp server模式不做获取
    bool                                    m_bDevInfoUpdated;      // 用户指示设备信息 m_stDevInfo 是否是从设备交互获取到的

    // 20200811，从socket接收数据，接收到数据直接处理，处理完毕去接收新数据，在数据量很大的情况下，可能存在数据处理时间过长导致丢帧的问题
    // 启动线程，线程接收数据放入缓冲区，数据处理线程从缓冲区获取数据，独立进行处理，尽量避免处理导致的数据丢失。
    bool                                    m_bThdRun{ true };
    std::thread                             m_DataProcessThd;
};

typedef std::shared_ptr<Device> Device_PTR;

#endif // DEVICE_H_

