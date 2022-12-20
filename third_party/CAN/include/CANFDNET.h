#ifndef CANFDNET_H_
#define CANFDNET_H_

#include "zlgcan/zlgcan.h"


#define SETGETREF_VIRIFY_DEVICE             9
#define SETGETREF_UDP_MULTICAST_LOOPBACK    10
#define SETGETREF_VIRIFY_DEVICE_BY_PASS     20  //��֤�豸��reftype��pData��������Ϊָ��VerifyDeviceData��ָ��

//SETREF_ADD_TIMER_SEND_CAN_DIRECT SETREF_ADD_TIMER_SEND_CANFD_DIRECT ֱ�ӷ��Ͷ�ʱ����
#define SETREF_ADD_TIMER_SEND_CAN_DIRECT    11  //pData Pointer to ZCAN_AUTO_TRANSMIT_OBJ
#define SETREF_ADD_TIMER_SEND_CANFD_DIRECT  12  //pData Pointer to ZCANFD_AUTO_TRANSMIT_OBJ

//���¶�ʱ����ʹ�����̣�Add CAN/CANFD����뻺�棬����Apply�Ž��ж�ʱ���ͣ�Clear��ֹ֮ͣǰApply�����ݲ���ջ���
#define SETREF_ADD_TIMER_SEND_CAN           21  //pData Pointer to ZCAN_AUTO_TRANSMIT_OBJ
#define SETREF_ADD_TIMER_SEND_CANFD         22  //pData Pointer to ZCANFD_AUTO_TRANSMIT_OBJ
#define SETREF_APPLY_TIMER_SEND             23  //9 Start Timer Send
#define SETREF_CLEAR_TIMER_SEND             24  //10 Stop Timer Send & Clear Send List

//�����豸ʱ���
#define SETREF_SET_DEV_TIMESTAMP            25  //pData Pointer to UINT64, Timestamp in us
//�����Ƿ��ͻ���
#define SETREF_SET_TX_ECHO_ENABLE           26  //�����豸�����Ƿ������ԣ�pData Pointer to UINT, 0:�رշ��ͻ��ԣ�1���������ͻ���
#define GETREF_GET_TX_TIMESTAMP             27  //������������, pData ָ�� TxTimeStamp
#define GETREF_GET_BUS_USAGE                28  //��ȡ����������, pDataָ�� BusUsage

//CANFDNET���ݷ���֡�е�TX_DELAY_SEND_FLAG��־��ʶ��ǰ֡�Ƿ�ʹ����ʱ����
//TX_DELAY_SEND_FLAG 0��ʾ�������͵����ߣ�1��ʾ�豸�ӳٷ���ģʽ��֧��֡������ã�֡�����λms��2�ֽ�����can/canfd֡�е�__res0(��8λ)��__res1(��8λ)�ֶ���
//���͵�����ʹ��ZCAN_Transmit/ZCAN_TransmitFD�ӿڣ�����ֵ��ʾ�ж���֡�Ѿ����뵽�豸�ķ��Ͷ����С�
#define GETREF_GET_DELAY_SEND_AVALIABLE_COUNT   29  //��ȡ�豸���ӳٷ��Ϳ������� pData Pointer to uint32_t
#define SETREF_CLEAR_DELAY_SEND_QUEUE           30  //������з�������������Ϊʱ��δ��δ���ͣ�ȡ���豸��ǰ�Ķ��з���

//��ȡ�豸��ʱ�����������豸���û�ȡCAN����CANFD��ʱ��������ʱ(REFΪ31,33)������豸����ͨѶ����ȡ�豸�˶�ʱ�����б�Ȼ��ͨ��(REF 32,34)�ֱ��ȡCAN,CANFD��ʱ����֡����
#define GETREF_GET_DEV_CAN_AUTO_SEND_COUNT      31  //��ȡ�豸�˶�ʱ����CAN֡��������pDataָ��UINT,��ʾ�豸�˶�ʱ����CAN֡����
#define GETREF_GET_DEV_CAN_AUTO_SEND_DATA       32  //��ȡ�豸�˶�ʱ����CAN֡�����ݣ��û����ݲ�ѯ����CAN֡���������ڴ� sizeof(ZCAN_AUTO_TRANSMIT_OBJ) * N�������뵽���ڴ��ַ����pData
#define GETREF_GET_DEV_CANFD_AUTO_SEND_COUNT    33  //��ȡ�豸�˶�ʱ����CANFD֡��������pDataָ��UINT,��ʾ�豸�˶�ʱ����CANFD֡����
#define GETREF_GET_DEV_CANFD_AUTO_SEND_DATA     34  //��ȡ�豸�˶�ʱ����CANFD֡�����ݣ��û����ݲ�ѯ����CAN֡���������ڴ� sizeof(ZCANFD_AUTO_TRANSMIT_OBJ) * N�������뵽���ڴ��ַ����pData

//��ȡ�豸״̬��Ϣ
#define GETREF_GET_DEV_STATE_SYS_INFO           35  //�����豸����ʱ�䣬���кţ��豸���Ƶ�json�ַ�����pDataָ��ZCAN_RAW_DATA, �û���Ҫ�����ڴ�(1k�ֽڼ���)�����ڴ��ַ���ڴ��С������ZCAN_RAW_DATA��Ӧ���ֶ���
#define GETREF_GET_DEV_STATE_CAN_INFO           36  //�����豸ͨ���Ƿ�ʹ�ܣ������ʣ�ͨ������������������Ϣ��json�ַ�����pDataָ��ZCAN_RAW_DATA, �û���Ҫ�����ڴ�(4k�ֽڼ���)�����ڴ��ַ���ڴ��С������ZCAN_RAW_DATA��Ӧ���ֶ���
#define GETREF_GET_DEV_STATE_RECORDER_INFO      37  //�����豸�Ƿ��¼����¼ģʽ����¼�ļ���С��sd��״̬����Ϣ��json�ַ�����pDataָ��ZCAN_RAW_DATA, �û���Ҫ�����ڴ�(1k�ֽڼ���)�����ڴ��ַ���ڴ��С������ZCAN_RAW_DATA��Ӧ���ֶ���
#define GETREF_GET_DEV_STATE_NET_INFO           38  //�����豸����ip�����ص���Ϣ��json�ַ�����pDataָ��ZCAN_RAW_DATA, �û���Ҫ�����ڴ�(1k�ֽڼ���)�����ڴ��ַ���ڴ��С������ZCAN_RAW_DATA��Ӧ���ֶ���

//��ȡ�豸���͵�GPS���ݵ�����
#define GETREF_GET_DEV_GPS_COUNT                39  //��ȡ�豸�˴����GPS���ݵ��������pDataָ��UINT,��ʾ����յ���GPS���ݵ�����
#define GETREF_GET_DEV_GPS_DATA                 40  //��ȡ�豸�˴����GPS���ݵ�����ݣ�pDataָ�� ZCAN_GPS_DATA
                                                    //�û������ڴ�ռ佫��ַ���� ZCAN_GPS_DATA.pData����������GPS���ݵ���������ZCAN_GPS_DATA.nFrameCount

//��ȡLIN ����Fifo��С
#define GETREF_GET_LIN_TX_FIFO_TOTAL            41  //��ȡLIN���ͻ�������С
#define GETREF_GET_LIN_TX_FIFO_AVAILABLE        42  //��ȡLIN���ͻ��������ô�С

//�����رպϲ�����
#define SETREF_SET_DATA_RECV_MERGE              43  //���úϲ��������ݣ�CAN/LIN/GPS�Լ���ͬͨ�������ݺϲ�����,pData Pointer to UINT, 0:�رպϲ����գ�1�������ϲ�����
#define GETREF_GET_DATA_RECV_MERGE              44  //��ȡ�ϲ���������״̬��pData Pointer to UINT, 0:�ϲ����չرգ�1���ϲ����մ��ڿ���״̬

//��ѯ�Ƿ������ͻ���
#define GETREF_GET_TX_ECHO_ENABLE               45  //��ѯ�豸�����Ƿ������ԣ�pData Pointer to UINT, 0:���ͻ����ѹرգ�1�����ͻ����ѿ���

#pragma pack(push ,1)

//verify device
typedef struct tagVerifyDeviceData
{
    char     inData[16];    //��������
    char     OutData[16];   //�豸��������
}VerifyDeviceData;

#if 0
//TX timestamp
typedef struct tagTxTimeStamp
{
    UINT64* pTxTimeStampBuffer;       //allocated by user, size:nBufferTimeStampCount * 8,unit:1us
    UINT    nBufferTimeStampCount;    //buffer timestamp count
    int     nWaitTime;                //Wait Time ms, -1��ʾ�ȵ������ݲŷ���
}TxTimeStamp;

// Bus usage
typedef struct tagBusUsage
{
    UINT64  nTimeStampBegin;//������ʼʱ�������λus
    UINT64  nTimeStampEnd;  //��������ʱ�������λus
    BYTE    nChnl;          //ͨ��
    BYTE    nReserved;      //����
    USHORT  nBusUsage;      //����������(%),����������*100չʾ��ȡֵ0~10000����8050��ʾ80.50%
    UINT    nFrameCount;    //֡����
}BusUsage;
#endif

typedef struct tagRemoteClient{
    int      iIndex;
    UINT     port;
    int      hClient;
    char     szip[32];
}REMOTE_CLIENT;

// GPS
typedef struct tagZCAN_GPS_FRAME
{
    union{
        struct{
            UINT btm : 1;           //Bit0 ��ʾʱ���Ƿ���Ч
            UINT blatlong : 1;      //Bit1 ��ʾ��γ����Ч
            UINT bspd : 1;          //Bit2 ��ʾ�ٶ��Ƿ���Ч
            UINT bcouseangle : 1;   //Bit3 ��ʾ������Ƿ���Ч
            UINT baltitude : 1;     //Bit4 ��ʾ�����Ƿ���Ч
            UINT breserved : 27;    //Bit5-32 ����
        }unionValue;
        UINT rawValue;
    } validflag;                    //��ʶ���ݵ���Ч��
    struct __gps_time {
        USHORT    year;
        USHORT    mon;
        USHORT    day;
        USHORT    hour;
        USHORT    min;
        USHORT    sec;
    }tm;                    // ʱ��
    float latitude;         // γ�� + north latitude, - south latitude
    float longitude;        // ���� + east longitude, - west longitude           
    float speed;            // km/h
    float courseangle;      // �����
    float altitude;         // ����
    UINT  reserved;         // ����
} ZCAN_GPS_FRAME;

typedef struct tagZCAN_GPS_DATA
{
    ZCAN_GPS_FRAME* pData;          // �û�������GPS���ݵĻ�������ַ,��Ҫ����Ŀռ��СΪ sizeof(ZCAN_GPS_FRAME) * nFrameCount
    UINT            nFrameCount;    // �û�������GPS���ݵĻ��������Դ��GPS����֡�ĸ���
    UINT            nRet;           // ʵ�ʻ�ȡ����GPS��������
    int             nWaitTime;      // ��ȡgps����ʱ�ĵȴ�ʱ�䣬��λms, -1��ʾ�ȵ������ݲŷ���
}ZCAN_GPS_DATA;

typedef struct tagZCAN_RAW_DATA
{
    void*           pData;          // �û����������ݵĻ�������ַ
    UINT            nDataLen;       // �û����������ݵĻ�������С
    UINT            nResultLen;     // �����������ʾʵ�����ݳ���,�û���Ҫ��鷵�ص����ݴ�С�����nResultLen > nDataLen ˵���û�����Ŀռ䲻���Դ�����ݣ��û���Ҫ���������ڴ�ռ����������ݽ����
    int             nWaitTime;      // ��ȡ����ʱ�ĵȴ�ʱ�䣬���ݲ�ͬ����˲������ܻᱻ����
}ZCAN_RAW_DATA;

//zlgcan��canfd_frame.__res0,��ZCANPRO�ڲ�ʹ�ã�����ͳһ��������,Ϊ�˶�ͨ����������������
//Flag����ͨ����Ϣ���Ƿ����֡��CAN/CANFD/Err��Ϣͳһʹ��canfd_frame�ṹ��װ
#define CANFD_FLAG_RES0_FRAME_TYPE_CAN      0
#define CANFD_FLAG_RES0_FRAME_TYPE_CANFD    1
#define CANFD_FLAG_RES0_FRAME_TYPE_ERROR    2
typedef union
{
    struct {
        BYTE nChnl : 4;      //ͨ��
        BYTE nType : 3;      //�������ͣ� 0��CAN;1:CANFD;2:Err
        BYTE nFlagValid : 1; //��ʶ��ǰFlag�ֽ��Ƿ���Ч��0����Ч��1����Ч
    } unionValue;
    BYTE nRawValue;
} CANFD_FLAG_RES0;

#pragma pack(pop)

#ifdef __cplusplus
extern "C"
{
#endif
UINT FUNC_CALL ZCAN_GetReference(UINT DeviceType, UINT nDevIndex, UINT nChnlIndex, UINT nRefType, void* pData);
UINT FUNC_CALL ZCAN_SetReference(UINT DeviceType, UINT nDevIndex, UINT nChnlIndex, UINT nRefType, void* pData);

#ifdef __cplusplus
}
#endif

#endif //CANFDNET_H_
