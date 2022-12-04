#ifndef CRECEIVEDATA_H
#define CRECEIVEDATA_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <functional>
#include <memory>
#include "cglobal_param.h"
#include "csafe_queue.h"
#include "kzmq.h"
#include "cparser_manager.h"
#include "cproto_pool.h"
#include "opencv2/opencv.hpp"
#include "ktime.h"
#include "raw_data.h"
#include "dds_data.h"
#include "dds/dds.h"
#include "dds/ddsrt/environ.h"

#define MAX_SAMPLES 1

const size_t kTopicNameMaxLen = 100;
const size_t kTimestampLen = sizeof(double);
const size_t kBatchLen = sizeof(size_t);
const QStringList lidar_list_ = {"lidar_service-Struct.string-left",
                                 "lidar_service-Struct.string-right",
                                 "lidar_perception-Struct.string-origin_points",
                                 "lidar_perception-Struct.string-seg_points",
                                 "lidar_perception-Struct.string-map_points",
                                 "lidar_perception-Struct.string-ground_points",
                                 "lidar_perception-Struct.string-non_ground_points",
                                 "lidar_perception-LidarObjectProto.Objects-Ai",
                                 "lidar_perception-LidarObjectProto.Objects-Seg",
                                 "lidar_perception-LidarObjectProto.Objects-Fus",
                                 "lidar_perception-LidarObjectProto.Objects-Tracked",
                                 "lidar_perception-LidarFreeSpaceProto.FreeSpaceData",
                                 "sab-CameraProto.CamLines-fc_selected",
                                 "sab-CameraProto.CamLines-curb"};

class CReceiveData : public QObject
{
    Q_OBJECT
public:
    CReceiveData();
    ~CReceiveData();

public:
    bool receive_flag_ = true;
    bool switch_flag_ = false;

private:
    void InitReceive();
    void ReceiveData(const std::string &data_address, const std::string &proto_address);
    void SplitRecvData(const char *data, size_t size,
                       std::string &topic_name, double &timestamp,
                       size_t &batch, std::string &msg_data);
    /** @brief 数据记录*/
    void RecordData(const std::string &topic_name, double timestamp, const std::string &msg_data);
    double timestamp_gap_ = 0;
    /** @brief usb摄像头数据接收*/
    void ReceiveCameraData();
    bool CreateCameraCapture();
    /** @brief dds数据接收*/
    void ReceiveDDSData(const QStringList &topics, const std::string &config_path = "./dbc/dds-config.xml");
    bool CreateDDSListener(dds_listener_t **listener);
    static void DDSDataCallBack(dds_entity_t reader, void *arg);
    bool CreateDDSTopic(dds_entity_t participant, const std::string &topic, dds_entity_t *res_topic);
    bool CreateDDSReader(dds_entity_t participant, dds_entity_t dds_topic,
                         dds_entity_t *res_reader, dds_listener_t *listener);
    QString ConvertDDSTopic(const QString &dds_topic);
    void DDSMsgToString(DDSData_Msg *src_data);

    dds_entity_t participant_ = 0;
    const dds_domainid_t domain_id_ = 0;
    QMap<QString, QString> dds_topic_map_ = {
        {"sab____RadarProto__RadarDetections____fr", ""},
        {"sab____RadarProto__RadarFreeSpace____fr", ""},
        {"sab____RadarProto__RadarObjects____fr", ""},
        {"sab____RadarProto__RadarObjects____fcr", ""},
        {"sab____RadarProto__RadarDetections____fcr", ""},
        {"sab____RadarProto__RadarFreeSpace____fcr", ""},
        {"sab____RadarProto__RadarObjects____rcr", ""},
        {"sab____RadarProto__RadarFreeSpace____rcr", ""},
        {"sab____RadarProto__RadarDetections____rcr", ""},
        {"lidar_service____Struct__string____left", ""},
        {"lidar_service____Struct__string____right", ""},
        {"sab____BusepsProto__BusEpsMotionInfo", ""},
        {"sab____BusepsProto__BusEpsSts", ""},
        {"sab____BusespProto__BusEspFbSts", ""},
        {"sab____BusespProto__BusEspFuncSts", ""},
        {"sab____BusespProto__BusEspMbr", ""},
        {"sab____BusespProto__BusEspMotionInfo", ""},
        {"sab____BusespProto__BusEspWhlSpd", ""},
        {"sab____BusespProto__BusBrkPressure", ""},
        {"sab____BusepsProto__BusSAS", ""},
        {"sab____BusespProto__BusEpbFunInfo", ""},
        {"sab____BusespProto__BusEspFbStsforAPA", ""},
        {"sab____BusrbmProto__BusRbmFbSts", ""},
        {"sab____BusrbmProto__BusRbmFunSts", ""},
        {"sab____BusrbmProto__BusRbmMotionInfo", ""},
        {"sab____VcuProto__BusVcuEngine", ""},
        {"sab____VcuProto__BusVcuPedl", ""},
        {"sab____VcuProto__BusVcuTransmission", ""},
        {"sab____EmsProto__BusEmsMotionInfo", ""},
        {"sab____EmsProto__BusEmsSts", ""},
        {"sab____TcuProto__BusTcuSts", ""},
        {"sab____BusbcmProto__BusSRS", ""},
        {"sab____TcuProto__BusAcmInfo", ""},
        {"sab____BusbcmProto__BusBcmDoorSts", ""},
        {"sab____BusbcmProto__BusBcmLightSts", ""},
        {"sab____BusbcmProto__BusBcmSts", ""},
        {"sab____BusbcmProto__BusBcmWinSts", ""},
        {"sab____BusmfsProto__BusMfsHod", ""},
        {"sab____BusbcmProto__BusPEPS", ""},
        {"sab____BusmfsProto__BusMfsSwitch", ""},
        {"sab____BushusetProto__BusHuAvmSet", ""},
        {"sab____BushusetProto__BusHuScreenOper", ""},
        {"sab____BushusetProto__BusHuVcReq", ""},
        {"sab____BusipProto__BusIP", ""},
        {"sab____BusadasmapProto__BusAdasMapCurInfo", ""},
        {"sab____BusadasmapProto__BusAdasMapRoadInfo", ""},
        {"sab____BusadasmapProto__BusAdasMapSlopeInfo", ""},
        {"sab____BusadasmapProto__BusAdasMapSts", ""},
        {"sab____BusbcmProto__BusBcmDsmSts", ""},
        {"sab____BusbcmProto__BusBcmTMS", ""},
        {"sab____BushusetProto__BusHuAdsFunSet", ""},
        {"sab____BushusetProto__BusHuApaSet", ""},
        {"sab____BushusetProto__BusHuDaFunSet", ""},
        {"sab____BushusetProto__BusHuDvrSet", ""},
        {"sab____BushusetProto__BusHuScreenSet", ""},
        {"sab____BusimsProto__BusIMS", ""},
        {"sab____PhonesetProto__BusRFBT", ""},
        {"sab____TboxProto__BusTboxSts", ""},
        {"sab____TboxProto__BusTboxTime", ""},
        {"sab____VcuProto__BusVcuSwitchSts", ""},
        {"sab____BusbcmProto__BusBcmTPMS", ""}};
    /** @brief 实时接收到的数据处理*/
    void DeliverData();

    CParserManager *parser_manager_ = nullptr;
    CProtoPool *proto_pool_ = nullptr;
    CDataCenter *data_center_ = nullptr;
    CSignalManager *signal_manager_ = nullptr;
    std::vector<cv::VideoCapture *> camera_captures_;

    CSafeQueue<std::string> msgs_queue_;
    std::mutex recv_mutex_;
    std::condition_variable condition_var_;
};

#endif // CRECEIVE_H