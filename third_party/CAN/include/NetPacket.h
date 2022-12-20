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
const uint8_t PACKET_TYPE_BUS_USAGE = 0x03;        // ���������ʰ�
const uint8_t PACKET_TYPE_DEV_STATE = 0x04;        // �豸״̬�����ͻ������ÿ������豸�����ϴ����ݣ�ֻ�����豸�ϱ���������ΪJSON�ַ���
const uint8_t PACKET_TYPE_GPS       = 0x05;        // GPS���ݰ���ֻ�����豸�ϱ��������Ͳ�������������Ϊn*GPS�ṹ
const uint8_t PACKET_TYPE_LIN       = 0x06;        // LIN���ݰ����豸�����ϱ��������Ͳ�������������Ϊn*LIN�ṹ
const uint8_t PACKET_TYPE_REQUEST_RESPONSE = 0x50; // �ͻ��˷��������豸�˻�Ӧ

const uint8_t PACKET_TYPE_AUTH_REQ = 0x40;         // ZCANPro��֤������������Ÿ��û�
const uint8_t PACKET_TYPE_AUTH_RES = 0x41;         // ZCANPro��֤Ӧ����������Ÿ��û�
const uint8_t PACKET_TYPE_DEV_REQ = 0x80;          // 2020.7.17 �Ѿ�����ʹ��
const uint8_t PACKET_TYPE_DEV_RES = 0x81;          // 2020.7.17 �Ѿ�����ʹ��

const uint8_t PACKET_TYPE_PARAM_FLAG_COMPRESS = 0x01;  // ������ΪPACKET_TYPE_CAN��PACKET_TYPE_CANFDʱ�������Ͳ���Bit0��ʾ�����Ƿ�ѹ��(zlibѹ��)����ѹ����������

//PACKET_TYPE_DEV_REQ/PACKET_TYPE_DEV_RES����Ӧ��nPacketTypeParam��Value
const uint8_t PACKET_TYPE_PARAM_DEV_REQ_RES_TIMESTAMP = 0;
const uint8_t PACKET_TYPE_DEV_REQ_TIMESTAMP_LEN = 8;
const uint8_t PACKET_TYPE_DEV_RES_TIMESTAMP_LEN = 0;

//const uint8_t PACKET_TYPE_DEV_STATE = 0x04;
const uint8_t PACKET_TYPE_DEV_STATE_PARAM_SYS_INFO = 0x01;
const uint8_t PACKET_TYPE_DEV_STATE_PARAM_CAN_INFO = 0x02;
const uint8_t PACKET_TYPE_DEV_STATE_PARAM_RECORDER_INFO = 0x03;
const uint8_t PACKET_TYPE_DEV_STATE_PARAM_NET_INFO = 0x04;

typedef std::vector<uint8_t>   Packet;         //���ڱ�ʾһ���������ݣ����� PacketHead + Data + 1BYTE Checksum
typedef std::vector<Packet> VecPackets;     //����N��������Packet
typedef std::vector<uint8_t>   FrameData;      //���ڱ�ʾ��Ч���ݣ�PacketDataCAN/PacketDataCANFD/AUTH DATA

#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef struct _tagPacketHead
{
    uint8_t    nPacketHead;       //��ͷ �̶�0x55 PACKET_HEADER
    uint8_t    nPacketType;       //���� 0x00:CAN;  0x01:CANFD; 0x40:��֤����
    uint8_t    nPacketTypeParam;  //���Ͳ�������bPacketType�й�
    uint8_t    nReServed;         //����������Ϊ0
    USHORT  nPacketDataLen;    //����������

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
        USHORT    nSendtype : 2;    //bit 0-1, 0:��������; 1:���η���(CANFDNET��֧��); 2:�Է����� (������Ч������Ϊ0)
        USHORT    nTx: 1;           //bit 2, 2019.11.26, ������Ч��0����ʾ�豸������������֡��1����ʾ�豸���ͻ���(Echo)֡
        USHORT    nEcho: 1;         //bit 3, 2019.11.26, ������Ч��0����ʾ�豸�������Ͳ���Ҫ���ԣ�1����ʾ�豸����������Ҫ����(Echo)֡
        USHORT    bFD : 1;          //bit4, 0::CAN;     1:CANFD
        USHORT    bRTR : 1;         //bit5, 0:����֡;   1:Զ��֡
        USHORT    bEXT : 1;         //bit6, 0:��׼֡;   1:��չ֡
        USHORT    bERR : 1;         //bit7, 0:��������; 1:������  (������Ч)
        USHORT    bBRS : 1;         //bit8, 0:������;   1:CANFD���� (CANFD��Ч)
        USHORT    bESI : 1;         //bit9, 0:��������; 1:��������  (CANFD������Ч)
        USHORT    bSendDelay : 1;   //bit10, 2020.7.17��0:��ͨ����; 1:���ͺ���ʱ�����͵�ǰ֡ʱ��ʼ��ʱ������ʱ���Ӷ����е�ȡ����һ֡����
        USHORT    nReserved2 : 5;   //bit11-15, ����
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
    UINT64  nTimestamp;        //ʱ�����8Byte����λus�������0
    UINT    nID;               //֡ID, ��׼֡11λ����չ֡29λ
    CANFrameInfo frameInfo;     //֡��Ϣ
    uint8_t      nChnl;            //CAN/CANFD ͨ����
    uint8_t      nDataLen;         //�������ݳ���
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
    uint8_t              canData[DATA_LEN_CAN];  //��������, CAN:8�ֽ�; CANFD:64�ֽ�
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
    uint8_t              canData[DATA_LEN_CANFD];  //��������, CAN:8�ֽ�; CANFD:64�ֽ�
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
        uint8_t    nID : 6;            //��ϢID,Bit0~5֡ID
        uint8_t    nIDChk : 2;         //��ϢID,Bit6~7У��
    }       unionValue;
    uint8_t    nRawID;                //��ϢID,Bit0~5֡ID��Bit6~7У��
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
        USHORT    bTx : 1;          //bit0, TX-��������������������Ϣ
        USHORT    bRx : 1;          //bit1, RX-���������������ϵ���Ϣ
        USHORT    bNoData: 1;       //bit2, NoData-��������
        USHORT    bChkSumErr : 1;   //bit3, ChkSumErr-У��ʹ��� 
        USHORT    bParityErr : 1;   //bit4, ParityErr-��żУ����� ��ʱ��Ϣ�е� chksum ��Ч 
        USHORT    bSyncErr : 1;     //bit5, SyncErr-ͬ���δ���
        USHORT    bBitERR : 1;      //bit6, BitErr-����ʱλ����
        USHORT    bWakeup : 1;      //bit7, Wakeup-�յ�����֡�� ��ʱ��Ϣ ID�����ݳ��ȡ�������У��ֵ��Ч
        USHORT    nReserved : 8;    //bit8-15, ����
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
    UINT64  nTimestamp;         //ʱ�����8Byte����λus��������Ч������ʱ����Ϊ0
    uint8_t    nChnl;              //LIN ͨ���ţ�ȡֵ 0~N-1��NΪͨ����
    LINMsgID stMsgID;           //��ϢID,Bit0~5֡ID��Bit6~7У��
    LINFrameInfo frameInfo;     //֡Flag
    uint8_t    nReserved[2];       //����
    uint8_t    nDataLen;           //��������Ч���ȣ�1~8
    uint8_t    nData[8];           //����
    uint8_t    nChkSum;            //���߽��յ�У���
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
        uint8_t     nErrSection : 5;    //����α�ʾ
        uint8_t     nErrProperty : 1;   //0:����ʱ��������1������ʱ��������
        uint8_t     nErrCodeType : 2;   //�����������, 0:λ��1:��ʽ��2:����3:������
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
        USHORT nRes0 : 1;               //Bit0,�豸�ڲ�ʹ�ã�Ĭ��Ϊ0�������޸�
        USHORT nUseDataTimestamp: 1;    //Bit1,Ϊ1��ʾ��������ʹ�ñ�����timestamp�ֶε�4�ֽڣ���λ 100us,Ϊ0��ʾʹ��nPeriodMs�ֶΣ���λms��1-60000ms
        USHORT nRes1 : 14;              //Bit2-15,������Ĭ��Ϊ0
    } unionValue;
    USHORT  rawValue;                   //ԭʼֵ��2�ֽ�
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
    uint8_t    nIndex;         //��ʱ���ͱ�ţ�0-31
    uint8_t    bEnable;        //��ʱ����ʹ�ܣ�1��ʹ�ܣ�0��ʧ��
    USHORT  nPeriodMs;      //�������ڣ���λms��1-60000ms
    USHORT  nRepeatCount;   //���ʹ�����0-65535��0��ʾһֱ����
    PacketDataAutoSendFlag  stFlag;          //��־
    PacketDataCANFD pktData;//����֡������Ϊcan����canfd֡
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
    UINT64  nTimestamp;         //ʱ�����Bit0-55��ʾUTCʱ�������λus��Bit56-62������Bit63��ʾ�����Ƿ���Ч��1��ʾ��Ч��0��ʾ��Ч��λ�������ϴ���Ч��λ����
    UINT    nLongitude;         //����,Bit0-27�Զ�Ϊ��λֵ����10��6�η�����ȷ�������֮һ�ȣ�
                                //bit28-29������bit30Ϊ0��ʾ������Ϊ1��ʾ������bit31��ʾ�����Ƿ���Ч��1��ʾ��Ч��0��ʾ��Ч��λ�������ϴ���Ч��λ����
    UINT    nLatitude;          //γ��,Bit0-27�Զ�Ϊ��λֵ����10��6�η�����ȷ�������֮һ�ȣ�
                                //bit28-29������bit30Ϊ0��ʾ��γ��Ϊ1��ʾ��γ��bit31��ʾ�����Ƿ���Ч��1��ʾ��Ч
    UINT    nSpeed;             //�ٶȣ�Bit0-15����λ0.1Km/h,ȡֵ0-10000��ʾ0Km/h-1000.0km/h, bit31��ʾ�����Ƿ���Ч��1��ʾ��Ч
    UINT    nCourseAngle;       //�����,Bit0-30,�Զ�Ϊ��λ����10��6�η��� bit31��ʾ�����Ƿ���Ч��1��ʾ��Ч
    UINT    nAltitude;          //���Σ�Bit0-15����λ�ף�ƫ��1000m��bit31��ʾ�����Ƿ���Ч��1��ʾ��Ч
    UINT    nReserved;          //����
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
    UINT64  nTimeStampBegin;//������ʼʱ�������λus
    UINT64  nTimeStampEnd;  //��������ʱ�������λus
    uint8_t    nChnl;          //ͨ��
    uint8_t    nReserved;      //����
    USHORT  nBusUsage;      //����������(%),����������*100չʾ��ȡֵ0~10000����8050��ʾ80.50%
    UINT    nFrameCount;    //�շ�������

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
        uint8_t    nReqVal : 7;     //bit0-6,����ֵ
        uint8_t    nReserved : 1;   //bit7,δʹ��
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
        uint8_t    nReqVal : 7;     //bit0-6,����ֵ
        uint8_t    nResult: 1;      //bit7,��������1��ִ�гɹ���0��ִ��ʧ��
    }       unionValue;
    uint8_t    rawValue;

#ifdef _WIN32
} PacketTypeRESParam;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketTypeRESParam;
#endif

// const uint8_t PACKET_TYPE_REQUEST_RESPONSE = 0x50;������Ϊ0x50ʱ�İ����Ͳ���
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagPacketTypeRequestResponseParam
{
    struct{
        uint8_t    bResponse : 1;  //bit0, 0:������� 1��Ӧ���
        uint8_t    nSeq : 3;       //bit1-3,����ţ�ÿ�����������Ӧ��������һ��
        uint8_t    nReserved : 4;  //bit4-7,����
    }       unionValue;
    uint8_t    rawValue;
#ifdef _WIN32
} PacketTypeRequestResponseParam;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) PacketTypeRequestResponseParam;
#endif

// ���紫��GPS����֡ʱ���
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagGPSTimeVal
{
    struct{
        UINT64    nUTC : 56;    //bit0-55, UTC ʱ���,��λus
        UINT64    nReserved: 7; //bit56-62,����
        UINT64    bValid : 1;   //bit63, 0:��Ч��λ��������һ����Ч���ݣ�1����Ч��λ
    }       unionValue;
    UINT64    rawValue;
#ifdef _WIN32
} GPSTimeVal;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) GPSTimeVal;
#endif

// ���紫��GPS��γ��
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagGPSLatLongVal
{
    struct{
        UINT    nData : 28;     //bit0-27, �Զ�Ϊ��λ�ľ�γ�ȳ���10��6�η�
        UINT    nReserved : 2;  //bit28-29,����
        UINT    bWestSourth : 1;//bit30����ʾ����������γ��0����ʾ�������߱�γ
        UINT    bValid : 1;     //bit31, 0:��Ч��λ��������һ����Ч���ݣ�1����Ч��λ
    }       unionValue;
    UINT    rawValue;
#ifdef _WIN32
} GPSLatLongVal;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) GPSLatLongVal;
#endif

// ���紫��GPS�ٶȣ��߶�
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagGPSSpdAltitude
{
    struct{
        UINT    nData : 16;     //bit0-15, �ٶȵ�λ0.1Km/h����Χ0-10000(1000.0km/h)�� �߶ȵ�λ�ף�ƫ��1000m
        UINT    nReserved : 15; //bit16-30,����
        UINT    bValid : 1;     //bit31, 0:��Ч��λ��������һ����Ч���ݣ�1����Ч��λ
    }       unionValue;
    UINT    rawValue;
#ifdef _WIN32
} GPSSpdAltitude;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) GPSSpdAltitude;
#endif

// ���紫��GPS�����
#ifdef _WIN32
#pragma pack(push,1)
#endif
typedef union _tagGPSCourseAngle
{
    struct{
        UINT    nData : 31;     //bit0-30, �Զ�Ϊ��λ�ľ�γ�ȳ���10��6�η�
        UINT    bValid : 1;     //bit31, 0:��Ч��λ��������һ����Ч���ݣ�1����Ч��λ
    }       unionValue;
    UINT    rawValue;
#ifdef _WIN32
} GPSCourseAngle;
#pragma pack(pop)
#else
} __attribute__((aligned(1), packed)) GPSCourseAngle;
#endif

// const uint8_t PACKET_TYPE_REQUEST_RESPONSE = 0x50; ������Ϊ0x50ʱ������Ӧ������
struct PacketDataRequest
{
    uint8_t    nTID;       // ����ID,�ͻ����Լ�����
    uint8_t    nRequest;   // ��������
    Packet  vecData;    // �������ݣ����Ȳ���
};
struct PacketDataResponse
{
    uint8_t    nTID;       // ����ID,������IDһ��
    uint8_t    nResult;    // Ӧ������0��Ӧ�����1��Ӧ����ȷ
    Packet  vecData;    // Ӧ�����ݣ����Ȳ���
};
const uint8_t PACKET_REQUEST_GET_DEVICE_INFO  = 1;             // ��ȡ�豸��Ϣ
const uint8_t PACKET_REQUEST_GET_DEVICE_STATE = 2;             // ��ȡ�豸״̬
const uint8_t PACKET_REQUEST_GET_DEVICE_AUTOSEND_LIST = 3;     // ��ȡ��ʱ�����б�
const uint8_t PACKET_REQUEST_GET_DEVICE_DELAY_SEND_STATE = 4;  // ��ȡ��ʱ����״̬
const uint8_t PACKET_REQUEST_CLEAR_DELAY_SEND_QUEUE = 5;       // ��շ�����ʱ����
const uint8_t PACKET_REQUEST_SET_CONFIG = 6;                   // ���ö�̬����
const uint8_t PACKET_REQUEST_CLEAR_LIN_AS_SLAVE_RESPONSE = 7;  // ���LIN��Ϊ��վʱ��Ӧ������
const uint8_t PACKET_REQUEST_GET_LIN_FIFO_INFO = 8;            // ��ȡ LIN ����FIFO ״̬
const uint8_t PACKET_REQUEST_DO_UDS_REQUEST = 0x0F;            // UDS����
const uint8_t PACKET_REQUEST_GET_UDS_STATUS = 0x10;            // ��ȡUDS״̬
const uint8_t PACKET_REQUEST_DO_UDS_CONTROL = 0x11;            // UDSָ�����

struct AutoSendData
{
    int                         sendcount;  //���ʹ�����0-65535��0��ʾһֱ����
    int                         iscanfd;    //����data������can����canfd
    ZCANFD_AUTO_TRANSMIT_OBJ    data;       //�����û����ö�ʱ�������ݣ����Է�CAN/CANFD֡
};
typedef std::vector<AutoSendData>  VecAutoSendData;

struct LINFIFOInfo
{
    UINT    nChnl;          // ͨ����
    UINT    nFIFOSize;      // ����FIFO�ܴ�С
    UINT    nFIFOAvailable; // ����FIFOʣ������֡��
};

struct LINConfig
{
    UINT    nEnable;        // LIN ͨ��ʹ�ܣ�1 - ���ã�0 - ����
    UINT    IsMaster;       // �Ƿ���Ϊ������1-������0-�ӻ� �ò�����ǰδ������Ӧ����Ϊ 0
    UINT    nBaud;          // �����ʣ�ȡֵ 1000~20000 
    union {
        struct {
            UINT bEnhancedChksum:1;     // Bit0:  ��ǿ��У�飬1-��ǿ��У��ͣ�0-������У���
            UINT bVarDLCDisabled : 1;   // Bit1:  �رտɱ���Ϣ���ȣ�1-�رտɱ� DLC��0-�ɱ� DLC ���رտɱ���Ϣ���Ⱥ���Ϣ���Ƚ�����������Ϣ ID 
            UINT nReserved : 30;        // Bit2~31:  ������Ϊ 0 
        } unionVal;
        UINT rawVal;                    // ע������ LIN2.X��Bit0�� Bit1 ����Ϊ 1,����LIN1.X��Bit0�� Bit1 ����Ϊ 0
    }       stFeature;      // LIN ����λ
};

const int PACKET_HEAD_LEN = sizeof(PacketHead);     //6
const int FRAME_LEN_CAN   = sizeof(PacketDataCAN);  //24
const int FRAME_LEN_CANFD = sizeof(PacketDataCANFD);//80
const int FRAME_LEN_GPS = sizeof(PacketDataGPS);    //32
const int FRAME_LEN_LIN = sizeof(PacketDataLIN);    //24
const int FRAME_LEN_AUTH  = 16;                     //16
const int FRAME_LEN_AUTO_SEND = sizeof(PacketDataAutoSend); //8+80=88
const int FRAME_LEN_BUS_USAGE = sizeof(PacketDataBusUsage); //24
const int PACKET_LEN_MIN = PACKET_HEAD_LEN + 1; //֡ͷ(6Byte)+����(0)+У��(1Byte)
const int PACKET_REQUEST_RESPONSE_DATALEN_MIN = 2;  //const uint8_t PACKET_TYPE_REQUEST_RESPONSE = 0x50 ʱ��Data������̳���Ϊ2������ 1Byte tid �� 1Byte result
const int PACKET_DATA_LEN_DECOMPRESS_MAX = 10000 * FRAME_LEN_CANFD;    //һ��������ѹ�����ݽ�ѹ����󳤶ȣ��ݶ�Ϊ10000֡CANFD

typedef std::vector<const ZCANDataObj*>     VecZCANDataObjPtr;
typedef std::vector<VecZCANDataObjPtr>      VecVecZCANDataObjPtr;

//���λ������ṹ����β���������ݣ���ͷ��ȡ����
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

    // д�����ݵ�����ȥ�У�����ֵ��ʾд���������
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

    // �ӻ���ȥ��ȡ���ݣ�����ֵ��ʾ��ȡ��������
    // ͨ���˽ӿڶ�ȡ���ݺ����ݾͲ��ڻ���������
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

    // �ӻ���ȥ��ȡ���ݣ�requestLen��ʾҪ��ȡ��������������ֵ��ʾ��ȡ�����Ƿ�ɹ�
    // ͨ���˽ӿڶ�ȡ���ݺ������Ծɻ���������,������Ҫ
    // ʹ��pop_front()�ӿ��Ƴ���Ӧ������
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

    //�ӻ�����ͷ���Ƴ�bytes�ֽ�����
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

    //��ȡ������ͷ�����ݣ�Ҫ�󻺳��б���������
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

//typedef std::deque<uint8_t>    PacketBuffer;   //˫�˶��У����ڻ�����յ������ݲ����д������ݴӶ���β�����룬������ϵĴӶ���ͷ��ɾ��
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

	//ͨ��������
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