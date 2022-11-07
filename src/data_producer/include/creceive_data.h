#ifndef CRECEIVEDATA_H
#define CRECEIVEDATA_H

#include <iostream>
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

const size_t kTopicNameMaxLen = 100;
const size_t kTimestampLen = sizeof(double);
const size_t kBatchLen = sizeof(size_t);

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
    /** @brief usb摄像头数据接收*/
    void ReceiveCameraData();
    bool CreateCameraCapture();
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