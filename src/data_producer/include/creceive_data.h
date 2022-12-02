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
#include "DDSData.h"
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

   void DDSMsgToQueue(DDSData_Msg *src_data);

   static bool IsContainsReader(dds_entity_t reader);

   static void DDSMsgToQueue(dds_entity_t reader, DDSData_Msg *src_data);

   QString DDS2SourceTopic(const QString &dds_topic);


private:
    void InitReceive();
    void ReceiveData(const std::string &data_address, const std::string &proto_address);
    void SplitRecvData(const char *data, size_t size,
                       std::string &topic_name, double &timestamp,
                       size_t &batch, std::string &msg_data);
    /** @brief 数据记录*/
    void RecordData(const std::string &topic_name, double timestamp, const std::string &msg_data);
    /** @brief usb摄像头数据接收*/
    void ReceiveCameraData();
    bool CreateCameraCapture();

    void DDSMsgToString(DDSData_Msg *src_data, std::string &desc_data);

    void ReceiveDdsData(const std::string &config_path, const QStringList &topics);


    bool CreateDDSTopic(dds_entity_t participant, const std::string &topic,
                        dds_entity_t *res_topic);

    bool CreateDDSReader(dds_entity_t participant, dds_entity_t dds_topic,
                         dds_entity_t *res_reader, dds_listener_t *listener = nullptr);

    bool CreateDDSParticiPant(const dds_domainid_t domain);

    bool CreateDDSLister(dds_listener_t **listener);

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

    /// DDS receive data
    dds_entity_t participant_;
    static QMap<QString, QString> topic_map_;
    static QMap<dds_entity_t, CReceiveData *> readers_;
};

#endif // CRECEIVE_H
