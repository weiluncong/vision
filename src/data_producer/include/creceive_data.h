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
    /** @brief dds数据接收*/
    void ReceiveDDSData(const std::string &config_path = "./dbc/dds-config.xml", const std::string &topic_name = "C2_SOC_TO_MCU");
    void DDSMsgToString(DDSData_Msg *src_data, std::string &desc_data);

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