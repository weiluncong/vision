#ifndef NET_PACKET_H_
#define NET_PACKET_H_

#include <vector>
#include <deque>
#include <memory>
#include <mutex>
#include <string.h>
#include <algorithm>    // for std::min
#include "CANFDNET.h"

const int  DATA_LEN_CAN = 8;
const int  DATA_LEN_CANFD = 64;
const int  PACKET_CRC_LEN = 1;
const uint8_t PACKET_HEADER = 0x55;

const uint8_t PACKET_TYPE_CAN = 0x00;
const uint8_t PACKET_TYPE_CANFD = 0x01;
const uint8_t PACKET_TYPE_CAN_AUTO_SEND = 0x02;
const uint8_t PACKET_TYPE_BUS_USAGE = 0x03;        // 总线利用率包
const uint8_t PACKET_TYPE_DEV_STATE = 0x04;        // 设备状态包，客户端配置开启后，设备周期上传数据，只能由设备上报，数据区为JSON字符串
const uint8_t PACKET_TYPE_GPS       = 0x05;        // GPS数据包，只能由设备上报，包类型参数保留，长度为n*GPS结构
const uint8_t PACKET_TYPE_LIN       = 0x06;        // LIN数据包，设备可以上报，包类型参数保留，长度为n*LIN结构
const uint8_t PACKET_TYPE_REQUEST_RESPONSE = 0x50; // 客户端发起请求，设备端回应

const uint8_t PACKET_TYPE_AUTH_REQ = 0x40;         // ZCANPro认证请求包，不开放给用户
const uint8_t PACKET_TYPE_AUTH_RES = 0x41;         // ZCANPro认证应答包，不开放给用户
const uint8_t PACKET_TYPE_DEV_REQ = 0x80;          // 2020.7.17 已经不在使用
const uint8_t PACKET_TYPE_DEV_RES = 0x81;          // 2020.7.17 已经不在使用

const uint8_t PACKET_TYPE_PARAM_FLAG_COMPRESS = 0x01;  // 包类型为PACKET_TYPE_CAN，PACKET_TYPE_CANFD时，包类型参数Bit0表示数据是否压缩(zlib压缩)，仅压缩数据区域

//PACKET_TYPE_DEV_REQ/PACKET_TYPE_DEV_RES包对应的nPacketTypeParam的Value
const uint8_t PACKET_TYPE_PARAM_DEV_REQ_RES_TIMESTAMP = 0;
const uint8_t PACKET_TYPE_DEV_REQ_TIMESTAMP_LEN = 8;
const uint8_t PACKET_TYPE_DEV_RES_TIMESTAMP_LEN = 0;

//const uint8_t PACKET_TYPE_DEV_STATE = 0x04;
const uint8_t PACKET_TYPE_DEV_STATE_PARAM_SYS_INFO = 0x01;
const uint8_t PACKET_TYPE_DEV_STATE_PARAM_CAN_INFO = 0x02;
const uint8_t PACKET_TYPE_DEV_STATE_PARAM_RECORDER_INFO = 0x03;
const uint8_t PACKET_TYPE_DEV_STATE_PARAM_NET_INFO = 0x04;

typedef std::vector<uint8_t>   Packet;         //用于表示一个包的数据，包含 PacketHead + Data + 1BYTE Checksum
typedef std::vector<Packet> VecPackets;     //包含N个完整的Packet
typedef std::vector<uint8_t>   FrameData;      //用于表示有效数据，PacketDataCAN/PacketDataCANFD/AUTH DATA

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketHead
{
    uint8_t    nPacketHead;       //包头 固定0x55 PACKET_HEADER
    uint8_t    nPacketType;       //类型 0x00:CAN;  0x01:CANFD; 0x40:认证数据
    uint8_t    nPacketTypeParam;  //类型参数，与bPacketType有关
    uint8_t    nReServed;         //保留参数，为0
    USHORT  nPacketDataLen;    //数据区长度

#ifdef _WIN32
} PacketHead;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketHead;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagCANFrameInfo
{
    struct
    {
        USHORT    nSendtype : 2;    //bit 0-1, 0:正常发送; 1:单次发送(CANFDNET不支持); 2:自发自收 (发送有效，接收为0)
        USHORT    nTx: 1;           //bit 2, 2019.11.26, 接收有效，0：表示设备正常接收数据帧；1：表示设备发送回显(Echo)帧
        USHORT    nEcho: 1;         //bit 3, 2019.11.26, 发送有效，0：表示设备正常发送不需要回显；1：表示设备发送数据需要回显(Echo)帧
        USHORT    bFD : 1;          //bit4, 0::CAN;     1:CANFD
        USHORT    bRTR : 1;         //bit5, 0:数据帧;   1:远程帧
        USHORT    bEXT : 1;         //bit6, 0:标准帧;   1:扩展帧
        USHORT    bERR : 1;         //bit7, 0:正常报文; 1:错误报文  (接收有效)
        USHORT    bBRS : 1;         //bit8, 0:不加速;   1:CANFD加速 (CANFD有效)
        USHORT    bESI : 1;         //bit9, 0:主动错误; 1:被动错误  (CANFD接收有效)
        USHORT    bSendDelay : 1;   //bit10, 2020.7.17，0:普通发送; 1:发送后延时，发送当前帧时开始计时，到达时间后从队列中的取出下一帧发送
        USHORT    nReserved2 : 5;   //bit11-15, 保留
    }         unionVal;
    USHORT    rawVal;
#ifdef _WIN32
} CANFrameInfo;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) CANFrameInfo;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketDataCANHead
{
    UINT64  nTimestamp;        //时间戳，8Byte，单位us，溢出清0
    UINT    nID;               //帧ID, 标准帧11位，扩展帧29位
    CANFrameInfo frameInfo;     //帧信息
    uint8_t      nChnl;            //CAN/CANFD 通道号
    uint8_t      nDataLen;         //报文数据长度
#ifdef _WIN32
} PacketDataCANHead;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketDataCANHead;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketDataCAN
{
    PacketDataCANHead canHead;
    uint8_t              canData[DATA_LEN_CAN];  //报文数据, CAN:8字节; CANFD:64字节
#ifdef _WIN32
} PacketDataCAN;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketDataCAN;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketDataCANFD
{
    PacketDataCANHead canHead;
    uint8_t              canData[DATA_LEN_CANFD];  //报文数据, CAN:8字节; CANFD:64字节
#ifdef _WIN32
} PacketDataCANFD;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketDataCANFD;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagLINMsgID
{
    struct
    {       
        uint8_t    nID : 6;            //消息ID,Bit0~5帧ID
        uint8_t    nIDChk : 2;         //消息ID,Bit6~7校验
    }       unionValue;
    uint8_t    nRawID;                //消息ID,Bit0~5帧ID，Bit6~7校验
#ifdef _WIN32
}LINMsgID;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) LINMsgID;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagLINFrameInfo
{
    struct
    {
        USHORT    bTx : 1;          //bit0, TX-控制器发送在总线上消息
        USHORT    bRx : 1;          //bit1, RX-控制器接收总线上的消息
        USHORT    bNoData: 1;       //bit2, NoData-无数据区
        USHORT    bChkSumErr : 1;   //bit3, ChkSumErr-校验和错误 
        USHORT    bParityErr : 1;   //bit4, ParityErr-奇偶校验错误， 此时消息中的 chksum 无效 
        USHORT    bSyncErr : 1;     //bit5, SyncErr-同步段错误
        USHORT    bBitERR : 1;      //bit6, BitErr-发送时位错误
        USHORT    bWakeup : 1;      //bit7, Wakeup-收到唤醒帧， 此时消息 ID、数据长度、数据域、校验值无效
        USHORT    nReserved : 8;    //bit8-15, 保留
    }         unionVal;
    USHORT    rawVal;
#ifdef _WIN32
} LINFrameInfo;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) LINFrameInfo;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketDataLIN
{
    UINT64  nTimestamp;         //时间戳，8Byte，单位us，接收有效，发送时保留为0
    uint8_t    nChnl;              //LIN 通道号，取值 0~N-1，N为通道数
    LINMsgID stMsgID;           //消息ID,Bit0~5帧ID，Bit6~7校验
    LINFrameInfo frameInfo;     //帧Flag
    uint8_t    nReserved[2];       //保留
    uint8_t    nDataLen;           //数据区有效长度，1~8
    uint8_t    nData[8];           //数据
    uint8_t    nChkSum;            //总线接收的校验和
#ifdef _WIN32
}PacketDataLIN;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketDataLIN;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagZLGCAN_PASSIVE_ERRORINFO_BYTE
{
    struct
    {
        uint8_t     nErrSection : 5;    //错误段表示
        uint8_t     nErrProperty : 1;   //0:发送时发生错误，1：接收时发生错误
        uint8_t     nErrCodeType : 2;   //错误代码类型, 0:位错；1:格式错；2:填充错；3:其他错
    }            unionValue;
    uint8_t         nRawValue;
#ifdef _WIN32
}ZLGCAN_PASSIVE_ERRORINFO_BYTE;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) ZLGCAN_PASSIVE_ERRORINFO_BYTE;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagPacketDataAutoSendFlag
{
    struct {
        USHORT nRes0 : 1;               //Bit0,设备内部使用，默认为0，不可修改
        USHORT nUseDataTimestamp: 1;    //Bit1,为1表示发送周期使用报文中timestamp字段低4字节，单位 100us,为0表示使用nPeriodMs字段，单位ms，1-60000ms
        USHORT nRes1 : 14;              //Bit2-15,保留，默认为0
    } unionValue;
    USHORT  rawValue;                   //原始值，2字节
#ifdef _WIN32
}PacketDataAutoSendFlag;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketDataAutoSendFlag;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketDataAutoSend
{
    uint8_t    nIndex;         //定时发送编号，0-31
    uint8_t    bEnable;        //定时发送使能，1：使能；0：失能
    USHORT  nPeriodMs;      //发送周期，单位ms，1-60000ms
    USHORT  nRepeatCount;   //发送次数，0-65535，0表示一直发送
    PacketDataAutoSendFlag  stFlag;          //标志
    PacketDataCANFD pktData;//发送帧，可能为can或者canfd帧
#ifdef _WIN32
}PacketDataAutoSend;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketDataAutoSend;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketDataGPS
{
    UINT64  nTimestamp;         //时间戳，Bit0-55表示UTC时间戳，单位us，Bit56-62保留，Bit63表示数据是否有效，1表示有效，0表示无效定位，沿用上次有效定位数据
    UINT    nLongitude;         //经度,Bit0-27以度为单位值乘以10的6次方，精确到百万分之一度，
                                //bit28-29保留，bit30为0表示东经，为1表示西经，bit31表示数据是否有效，1表示有效，0表示无效定位，沿用上次有效定位数据
    UINT    nLatitude;          //纬度,Bit0-27以度为单位值乘以10的6次方，精确到百万分之一度，
                                //bit28-29保留，bit30为0表示北纬，为1表示南纬，bit31表示数据是否有效，1表示有效
    UINT    nSpeed;             //速度，Bit0-15，单位0.1Km/h,取值0-10000表示0Km/h-1000.0km/h, bit31表示数据是否有效，1表示有效
    UINT    nCourseAngle;       //航向角,Bit0-30,以度为单位乘以10的6次方， bit31表示数据是否有效，1表示有效
    UINT    nAltitude;          //海拔，Bit0-15，单位米，偏移1000m，bit31表示数据是否有效，1表示有效
    UINT    nReserved;          //保留
#ifdef _WIN32
}PacketDataGPS;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketDataGPS;
#endif

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketDataBusUsage
{
    UINT64  nTimeStampBegin;//测量起始时间戳，单位us
    UINT64  nTimeStampEnd;  //测量结束时间戳，单位us
    uint8_t    nChnl;          //通道
    uint8_t    nReserved;      //保留
    USHORT  nBusUsage;      //总线利用率(%),总线利用率*100展示。取值0~10000，如8050表示80.50%
    UINT    nFrameCount;    //收发报文数

#ifdef _WIN32
}PacketDataBusUsage;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketDataBusUsage;
#endif

// const uint8_t PACKET_TYPE_DEV_REQ = 0x80;
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagPacketTypeREQParam
{
    struct{
        uint8_t    nReqVal : 7;     //bit0-6,请求值
        uint8_t    nReserved : 1;   //bit7,未使用
    }       unionValue;
    uint8_t    rawValue;

#ifdef _WIN32
} PacketTypeREQParam;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketTypeREQParam;
#endif

// const uint8_t PACKET_TYPE_DEV_RES = 0x81;
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagPacketTypeRESParam
{
    struct{
        uint8_t    nReqVal : 7;     //bit0-6,请求值
        uint8_t    nResult: 1;      //bit7,请求结果，1：执行成功；0：执行失败
    }       unionValue;
    uint8_t    rawValue;

#ifdef _WIN32
} PacketTypeRESParam;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketTypeRESParam;
#endif

// const uint8_t PACKET_TYPE_REQUEST_RESPONSE = 0x50;包类型为0x50时的包类型参数
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagPacketTypeRequestResponseParam
{
    struct{
        uint8_t    bResponse : 1;  //bit0, 0:请求包， 1：应答包
        uint8_t    nSeq : 3;       //bit1-3,包序号，每次请求递增，应答与请求一致
        uint8_t    nReserved : 4;  //bit4-7,保留
    }       unionValue;
    uint8_t    rawValue;
#ifdef _WIN32
} PacketTypeRequestResponseParam;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketTypeRequestResponseParam;
#endif

// 网络传输GPS数据帧时间戳
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagGPSTimeVal
{
    struct{
        UINT64    nUTC : 56;    //bit0-55, UTC 时间戳,单位us
        UINT64    nReserved: 7; //bit56-62,保留
        UINT64    bValid : 1;   //bit63, 0:无效定位，沿用上一次有效数据，1：有效定位
    }       unionValue;
    UINT64    rawValue;
#ifdef _WIN32
} GPSTimeVal;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) GPSTimeVal;
#endif

// 网络传输GPS经纬度
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagGPSLatLongVal
{
    struct{
        UINT    nData : 28;     //bit0-27, 以度为单位的经纬度乘以10的6次方
        UINT    nReserved : 2;  //bit28-29,保留
        UINT    bWestSourth : 1;//bit30：表示西经或者南纬；0：表示东经或者北纬
        UINT    bValid : 1;     //bit31, 0:无效定位，沿用上一次有效数据，1：有效定位
    }       unionValue;
    UINT    rawValue;
#ifdef _WIN32
} GPSLatLongVal;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) GPSLatLongVal;
#endif

// 网络传输GPS速度，高度
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagGPSSpdAltitude
{
    struct{
        UINT    nData : 16;     //bit0-15, 速度单位0.1Km/h，范围0-10000(1000.0km/h)； 高度单位米，偏移1000m
        UINT    nReserved : 15; //bit16-30,保留
        UINT    bValid : 1;     //bit31, 0:无效定位，沿用上一次有效数据，1：有效定位
    }       unionValue;
    UINT    rawValue;
#ifdef _WIN32
} GPSSpdAltitude;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) GPSSpdAltitude;
#endif

// 网络传输GPS航向角
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagGPSCourseAngle
{
    struct{
        UINT    nData : 31;     //bit0-30, 以度为单位的经纬度乘以10的6次方
        UINT    bValid : 1;     //bit31, 0:无效定位，沿用上一次有效数据，1：有效定位
    }       unionValue;
    UINT    rawValue;
#ifdef _WIN32
} GPSCourseAngle;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) GPSCourseAngle;
#endif

// const uint8_t PACKET_TYPE_REQUEST_RESPONSE = 0x50; 包类型为0x50时的请求，应答数据
struct PacketDataRequest
{
    uint8_t    nTID;       // 事务ID,客户端自己定义
    uint8_t    nRequest;   // 请求类型
    Packet  vecData;    // 请求数据，长度不定
};
struct PacketDataResponse
{
    uint8_t    nTID;       // 事务ID,与请求ID一致
    uint8_t    nResult;    // 应答结果，0：应答错误，1：应答正确
    Packet  vecData;    // 应答数据，长度不定
};
const uint8_t PACKET_REQUEST_GET_DEVICE_INFO  = 1;             // 获取设备信息
const uint8_t PACKET_REQUEST_GET_DEVICE_STATE = 2;             // 获取设备状态
const uint8_t PACKET_REQUEST_GET_DEVICE_AUTOSEND_LIST = 3;     // 获取定时发送列表
const uint8_t PACKET_REQUEST_GET_DEVICE_DELAY_SEND_STATE = 4;  // 获取延时队列状态
const uint8_t PACKET_REQUEST_CLEAR_DELAY_SEND_QUEUE = 5;       // 清空发送延时队列
const uint8_t PACKET_REQUEST_SET_CONFIG = 6;                   // 设置动态配置
const uint8_t PACKET_REQUEST_CLEAR_LIN_AS_SLAVE_RESPONSE = 7;  // 清空LIN作为从站时的应答数据
const uint8_t PACKET_REQUEST_GET_LIN_FIFO_INFO = 8;            // 获取 LIN 发送FIFO 状态
const uint8_t PACKET_REQUEST_DO_UDS_REQUEST = 0x0F;            // UDS请求
const uint8_t PACKET_REQUEST_GET_UDS_STATUS = 0x10;            // 获取UDS状态
const uint8_t PACKET_REQUEST_DO_UDS_CONTROL = 0x11;            // UDS指令控制

struct AutoSendData
{
    int                         sendcount;  //发送次数，0-65535，0表示一直发送
    int                         iscanfd;    //标明data类型是can还是canfd
    ZCANFD_AUTO_TRANSMIT_OBJ    data;       //保留用户设置定时发送数据，可以放CAN/CANFD帧
};
typedef std::vector<AutoSendData>  VecAutoSendData;

struct LINFIFOInfo
{
    UINT    nChnl;          // 通道号
    UINT    nFIFOSize;      // 发送FIFO总大小
    UINT    nFIFOAvailable; // 发送FIFO剩余可填充帧数
};

struct LINConfig
{
    UINT    nEnable;        // LIN 通道使能，1 - 启用；0 - 禁用
    UINT    IsMaster;       // 是否作为主机，1-主机，0-从机 该参数当前未开发，应设置为 0
    UINT    nBaud;          // 波特率，取值 1000~20000 
    union {
        struct {
            UINT bEnhancedChksum:1;     // Bit0:  增强型校验，1-增强型校验和，0-经典型校验和
            UINT bVarDLCDisabled : 1;   // Bit1:  关闭可变消息长度，1-关闭可变 DLC，0-可变 DLC ，关闭可变消息长度后，消息长度将不再依赖消息 ID 
            UINT nReserved : 30;        // Bit2~31:  保留，为 0 
        } unionVal;
        UINT rawVal;                    // 注：对于 LIN2.X，Bit0和 Bit1 均设为 1,对于LIN1.X，Bit0和 Bit1 均设为 0
    }       stFeature;      // LIN 特性位
};

const int PACKET_HEAD_LEN = sizeof(PacketHead);     //6
const int FRAME_LEN_CAN   = sizeof(PacketDataCAN);  //24
const int FRAME_LEN_CANFD = sizeof(PacketDataCANFD);//80
const int FRAME_LEN_GPS = sizeof(PacketDataGPS);    //32
const int FRAME_LEN_LIN = sizeof(PacketDataLIN);    //24
const int FRAME_LEN_AUTH  = 16;                     //16
const int FRAME_LEN_AUTO_SEND = sizeof(PacketDataAutoSend); //8+80=88
const int FRAME_LEN_BUS_USAGE = sizeof(PacketDataBusUsage); //24
const int PACKET_LEN_MIN = PACKET_HEAD_LEN + 1; //帧头(6Byte)+数据(0)+校验(1Byte)
const int PACKET_REQUEST_RESPONSE_DATALEN_MIN = 2;  //const uint8_t PACKET_TYPE_REQUEST_RESPONSE = 0x50 时，Data部分最短长度为2，包含 1Byte tid 和 1Byte result
const int PACKET_DATA_LEN_DECOMPRESS_MAX = 10000 * FRAME_LEN_CANFD;    //一个包包含压缩数据解压后最大长度，暂定为10000帧CANFD

typedef std::vector<const ZCANDataObj*>     VecZCANDataObjPtr;
typedef std::vector<VecZCANDataObjPtr>      VecVecZCANDataObjPtr;

//环形缓冲区结构，从尾部插入数据，从头部取数据
class CircularBuffer
{
public:
    CircularBuffer(size_t capacity = 10 * 1024 * 1024)
        : beg_index_(0)
        , end_index_(0)
        , size_(0)
        , capacity_(capacity)
    {
        data_ = new uint8_t[capacity];
    }

    ~CircularBuffer()
    {
        delete[] data_;
        data_ = nullptr;
    }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

    // 写入数据到缓冲去中，返回值表示写入的数据量
    size_t write(const uint8_t *data, size_t bytes)
    {
        if (bytes == 0) return 0;

        size_t capacity = capacity_;
        size_t bytes_to_write = std::min(bytes, capacity - size_);

        // Write in a single step
        if (bytes_to_write <= capacity - end_index_)
        {
            memcpy(data_ + end_index_, data, bytes_to_write);
            end_index_ += bytes_to_write;
            if (end_index_ == capacity) end_index_ = 0;
        }
        // Write in two steps
        else
        {
            size_t size_1 = capacity - end_index_;
            memcpy(data_ + end_index_, data, size_1);
            size_t size_2 = bytes_to_write - size_1;
            memcpy(data_, data + size_1, size_2);
            end_index_ = size_2;
        }

        size_ += bytes_to_write;
        return bytes_to_write;
    }

    // 从缓冲去读取数据，返回值表示读取的数据量
    // 通过此接口读取数据后，数据就不在缓冲区中了
    size_t read(uint8_t *data, size_t bytes)
    {
        if (bytes == 0) return 0;

        size_t capacity = capacity_;
        size_t bytes_to_read = std::min(bytes, size_);

        // Read in a single step
        if (bytes_to_read <= capacity - beg_index_)
        {
            memcpy(data, data_ + beg_index_, bytes_to_read);
            beg_index_ += bytes_to_read;
            if (beg_index_ == capacity) beg_index_ = 0;
        }
        // Read in two steps
        else
        {
            size_t size_1 = capacity - beg_index_;
            memcpy(data, data_ + beg_index_, size_1);
            size_t size_2 = bytes_to_read - size_1;
            memcpy(data + size_1, data_, size_2);
            beg_index_ = size_2;
        }

        size_ -= bytes_to_read;
        return bytes_to_read;
    }

    // 从缓冲去读取数据，requestLen表示要读取的数据量，返回值表示读取数据是否成功
    // 通过此接口读取数据后，数据仍旧还缓冲区中,后续需要
    // 使用pop_front()接口移除对应的数据
    bool read(Packet& pkt, size_t requestLen)
    {
        if (requestLen == 0) return true;
        if (requestLen <= size_)
        {
            size_t capacity = capacity_;
            size_t bytes_to_read = requestLen;

            // Read in a single step
            if (bytes_to_read <= capacity - beg_index_)
            {
                for (size_t i = 0; i < bytes_to_read; ++i)
                {
                    pkt.push_back(data_[beg_index_ + i]);
                }
            }
            // Read in two steps
            else
            {
                size_t size_1 = capacity - beg_index_;
                for (size_t i = 0; i < size_1; ++i)
                {
                    pkt.push_back(data_[beg_index_ + i]);
                }
                size_t size_2 = bytes_to_read - size_1;
                for (size_t i = 0; i < size_2; ++i)
                {
                    pkt.push_back(data_[i]);
                }
            }
            return true;
        }
        return false;
    }

    //从缓冲区头部移除bytes字节数据
    size_t pop_front(size_t bytes = 1)
    {
        if (bytes == 0) return 0;

        size_t capacity = capacity_;
        size_t bytes_to_read = std::min(bytes, size_);

        // pop in a single step
        if (bytes_to_read <= capacity - beg_index_)
        {
            beg_index_ += bytes_to_read;
            if (beg_index_ == capacity) beg_index_ = 0;
        }
        // pop in two steps
        else
        {
            size_t size_1 = capacity - beg_index_;
            size_t size_2 = bytes_to_read - size_1;
            beg_index_ = size_2;
        }

        size_ -= bytes_to_read;
        return bytes_to_read;
    }

    //获取缓冲区头部数据，要求缓冲中必须有数据
    uint8_t front()
    {
        return data_[beg_index_];
    }

    void clear()
    {
        size_ = beg_index_ = end_index_ = 0;
    }

private:
    size_t beg_index_, end_index_, size_, capacity_;
    uint8_t *data_;
};

//typedef std::deque<uint8_t>    PacketBuffer;   //双端队列，用于缓存接收到的数据并进行处理，数据从队列尾部进入，处理完毕的从队列头部删掉
typedef CircularBuffer PacketBuffer;

class CPacketUtil
{
public:
    CPacketUtil();
    ~CPacketUtil();

public:
    static uint8_t CalcCRC(const uint8_t* data, unsigned int len);
    static bool IsValidPacketHead(const Packet& packet);
    static bool GetPacket(PacketBuffer& dequeBuffer, Packet& packet);

    static uint8_t GetPacketFrameType(const Packet& packet);
    static uint8_t GetPacketFrameTypeParam(const Packet& packet);
    static int  GetPacketFrameCount(const Packet& packet);
    static int  GetPacketDataLen(const Packet& packet);
    static bool IsPacketCompressed(const Packet& packet);

    static int  GetPacketDataCAN(const Packet& packet, std::vector<PacketDataCAN>& vecFrame);
    static int  GetPacketDataCANFD(const Packet& packet, std::vector<PacketDataCANFD>& vecFrame);
    static int  GetPacketDataGPS(const Packet& packet, std::vector<PacketDataGPS>& vecFrame);
    static int  GetPacketDataLIN(const Packet& packet, std::vector<PacketDataLIN>& vecFrame);
    static int  GetPacketDataAUTH(const Packet& packet, FrameData& vecAuth, uint8_t& pktTypeParam);
    static int  GetPacketDataBusUsage(const Packet& packet, PacketDataBusUsage& pktBusUsage);
    static int  GetPacketDataDevReq(const Packet& packet, FrameData& vecBusDevReq, uint8_t& pktTypeParam);
    static int  GetPacketDataDevRes(const Packet& packet, FrameData& vecBusDevRes, uint8_t& pktTypeParam);
    static int  GetPacketDataRequest(const Packet& packet, PacketDataRequest& request);
    static int  GetPacketDataResponse(const Packet& packet, PacketDataResponse& response);
    static int  GetPacketDataDevState(const Packet& packet, FrameData& vecDevStat, uint8_t& pktTypeParam);

    static bool GetPacket(const FrameData& pktFrame, PacketDataCAN& pktCAN);
    static bool GetPacket(const FrameData& pktFrame, PacketDataCANFD& pktCANFD);
    static bool GetPacket(const FrameData& pktFrame, PacketDataGPS& pktGPS);
    static bool GetPacket(const FrameData& pktFrame, PacketDataLIN& pktLIN);
    static bool GetPacket(const FrameData& pktFrame, PacketDataAutoSend& pktAutoSend);

    static UINT64 GetNowTimeStampAsMicrosecond();
    static void ZCANTx2CANFrame(const ZCAN_Transmit_Data& zTx, int nChnl, bool bTxEchoEnable, PacketDataCAN& pktFrame);
    static void ZCANTx2CANFrame(const ZCAN_TransmitFD_Data& zTx, int nChnl, bool bTxEchoEnable, PacketDataCANFD& pktFrame);
    static bool ZCANDataObj2CANFrame(const ZCANDataObj& zTx, PacketDataCANFD& pktFrame);

    static void CANFrame2ZCANRx(const PacketDataCAN& pktFrame, ZCAN_Receive_Data& zRx, int* pChnl);
    static void CANFrame2ZCANRx(const PacketDataCAN& pktFrame, ZCANDataObj& zRx);
    static void CANFrame2ZCANRx(const PacketDataCANFD& pktFrame, ZCAN_ReceiveFD_Data& zRx, int* pChnl, bool bRecvMerge);
    static void CANFrame2ZCANRx(const PacketDataCANFD& pktFrame, ZCANDataObj& zRx);

    static void CANFrame2CANFDFrame(const PacketDataCAN& pktFrameCAN, PacketDataCANFD& pktFrameCANFD);
    static void CANFDFrame2CANFrame(const PacketDataCANFD& pktFrameCANFD, PacketDataCAN& pktFrameCAN);

    static bool CANFrame2ErrFrame(const PacketDataCAN& pktFrame, ZCAN_CHANNEL_ERR_INFO& errInfo);
    static bool CANFrame2ErrFrame(const PacketDataCANFD& pktFrame, ZCAN_CHANNEL_ERR_INFO& errInfo);

    static void ZCANAutoSend2AutoSendFrame(const ZCAN_AUTO_TRANSMIT_OBJ& zCAN, int nChnl, bool bTxEchoEnable, PacketDataAutoSend& autoSend);
    static void ZCANAutoSend2AutoSendFrame(const ZCANFD_AUTO_TRANSMIT_OBJ& zCAN, int nChnl, bool bTxEchoEnable, PacketDataAutoSend& autoSend);
    static void AutoSendData2AutoSendFrame(const AutoSendData& autoSendData, int nChnl, bool bTxEchoEnable, PacketDataAutoSend& autoSendFrame);
    static void AutoSendFrame2ZCANAutoSend(const PacketDataAutoSend& autoSend, ZCAN_AUTO_TRANSMIT_OBJ& zCAN, int& nChnl);
    static void AutoSendFrame2ZCANAutoSend(const PacketDataAutoSend& autoSend, ZCANFD_AUTO_TRANSMIT_OBJ& zCAN, int& nChnl);

    static void PacketGPS2ZGPS(const PacketDataGPS& pktGPS, ZCAN_GPS_FRAME& zGPS);
    static void PacketGPS2ZCANDataObj(const PacketDataGPS& pktGPS, ZCANDataObj& zGPS);

    //LIN
    static void PacketLIN2ZLIN(const PacketDataLIN& pktLIN, ZCAN_LIN_MSG& zLIN, int* pChnl);
    static void PacketLIN2ZCANDataObj(const PacketDataLIN& pktLIN, ZCANDataObj& zRx);
    static void ZLIN2PacketLIN(int nChnl, const ZCAN_LIN_MSG& zLIN, PacketDataLIN& pktLIN);
    static bool ZCANDataObj2PacketLIN(const ZCANDataObj& zTx, PacketDataLIN& pktLIN);

    static bool CANErrorData2ZcanErrInfo(const PacketDataCANHead& canHead, const uint8_t* data, ZCAN_CHANNEL_ERR_INFO& errInfo);
    static bool CANErrorData2ZCANDataObj(const PacketDataCANHead& canHead, const uint8_t* data, ZCANDataObj& errInfo);

    static void DumpData(uint8_t* pData, int nLen);

    static int  GetNodeStateByTxRxErrCounter(int nTxErr, int nRxErr);

	//通道利用率
	static void PacketDataBusUsage2ZCANDataObj(const PacketDataBusUsage& pktBusUsage, ZCANDataObj& zRx);

    static VecVecZCANDataObjPtr SplitZCANData(const ZCANDataObj* pSend, ULONG len);
    static bool PacketData2ZCANDataObj(const PacketDataCANFD& pktFrame, ZCANDataObj& zCANObj);
    static bool PacketData2ZCANDataObj(const PacketDataCAN& pktFrame, ZCANDataObj& zCANObj);
    static bool PacketData2ZCANDataObj(const PacketDataGPS& pktFrame, ZCANDataObj& zCANObj);
    static bool PacketData2ZCANDataObj(const PacketDataLIN& pktFrame, ZCANDataObj& zCANObj);
	static bool PacketData2ZCANDataObj(const PacketDataBusUsage &pktBusUsage, ZCANDataObj& zCANObj);
};

class CPacketDecoder
{
public:
    CPacketDecoder(const char* rmt);
    ~CPacketDecoder();

public:
    void    AppendData(const char* data, unsigned int len);
    size_t  GetPackets(VecPackets& vecPackets);
    const std::string& GetDecoderName() const { return m_strName; }

private:
    PacketBuffer   m_dequeData;
    std::mutex     m_dequeMutex;
    std::string    m_strName;
};

class CPacketEncoder
{
public:
    CPacketEncoder();
    ~CPacketEncoder();

public:
    static VecPackets BuildPacketCAN(int nChnl, const ZCAN_Transmit_Data* pSend, ULONG len, bool bTxEchoEnable);
    static VecPackets BuildPacketCANFD(int nChnl, const ZCAN_TransmitFD_Data* pSend, ULONG len, bool bTxEchoEnable);
    static VecPackets BuildPacketCANCANFD(const VecZCANDataObjPtr& vecData);
    static VecPackets BuildPacketMerge(const ZCANDataObj* pSend, ULONG len);
    static Packet     BuildPacketAUTH(bool bRequest, const uint8_t* data, unsigned int len, uint8_t autoIncIndex);
    static Packet     BuildPacketAutosend(int nChnl, const ZCAN_AUTO_TRANSMIT_OBJ* pObj, bool bTxEchoEnable);
    static Packet     BuildPacketAutosend(int nChnl, const ZCANFD_AUTO_TRANSMIT_OBJ* pObj, bool bTxEchoEnable);
    static VecPackets BuildPacketAutosend(int nChnl, const VecAutoSendData& vecAutoSend, bool bTxEchoEnable);
    static Packet     BuildPacketDevReqRes(bool bRequest, const uint8_t* data, unsigned int len, uint8_t pktTypeParam);
    static Packet     BuildPacketRequest(uint8_t nSeq, const PacketDataRequest& request);
    static Packet     BuildPacketEmulateResponse(uint8_t nSeq, const PacketDataRequest& requestData);
    static void       GenRequestResponseEmulateData(const PacketDataRequest& requestData, PacketDataResponse& responseData);
    static VecPackets BuildPacketLIN(int nChnl, const PZCAN_LIN_MSG pSend, ULONG len);
    static VecPackets BuildPacketLIN(const VecZCANDataObjPtr& vecData);
    static Packet     BuildPacketEchoResponse(const PacketDataCAN& canFrame);
    static Packet     BuildPacketEchoResponse(const PacketDataCANFD& canFrame);

private:
    static PacketHead BuildPacketHead(uint8_t pktType, int pktCnt, uint8_t pktTypeParam = 0);

    static void       PacketAddHead(Packet& packet, const PacketHead& pktHead);
    static void       PacketAddData(Packet& packet, const PacketDataCAN& canFrame);
    static void       PacketAddData(Packet& packet, const PacketDataCANFD& canFDFrame);
    static void       PacketAddData(Packet& packet, const PacketDataAutoSend& autoSendFrame);
    static void       PacketAddData(Packet& packet, const PacketDataLIN& pktLIN);
    static void       PacketAddData(Packet& packet, const uint8_t* data, unsigned int len);
    static void       PacketAddCRC(Packet& packet);
};


#endif //NET_PACKET_H_