#include "creceive_data.h"

CReceiveData::CReceiveData()
{
    parser_manager_ = new CParserManager();
    proto_pool_ = CProtoPool::GetCProtoPool();
    data_center_ = CDataCenter::GetCDataCenter();
    signal_manager_ = CSignalManager::GetCSignalManager();
    InitReceive();
}

CReceiveData::~CReceiveData()
{
    receive_flag_ = false;
    condition_var_.notify_one();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    for (auto i : camera_captures_)
    {
        SAFE_DELETE(i);
    }
    SafeClear(camera_captures_);
    SAFE_DELETE(parser_manager_);
}

/*************************************zmq data receive*********************************/
void CReceiveData::InitReceive()
{
    switch (FLAGS_v_model_setting)
    {
    case 0:
        ReceiveData(FLAGS_v_data_address_pc, FLAGS_v_proto_address_pc);
        // ReceiveCameraData();
        DeliverData();
        break;
    case 1:
        ReceiveData(FLAGS_v_data_address_soc1, FLAGS_v_proto_address_soc1);
        // ReceiveCameraData();
        DeliverData();
        break;
    case 2:
        ReceiveData(FLAGS_v_data_address_soc1, FLAGS_v_proto_address_soc1);
        ReceiveData(FLAGS_v_data_address_soc2, FLAGS_v_proto_address_soc2);
        DeliverData();
        break;
    default:
        break;
    }
}

void CReceiveData::ReceiveData(const std::string &data_address, const std::string &proto_address)
{
    std::thread t([&] {
                    bool proto_flag = true;
                    zmq::message_t msg;
                    zmq::context_t *context = new zmq::context_t();
                    std::shared_ptr<zmq::socket_t> socket = std::make_shared<zmq::socket_t>(*context, zmq::socket_type::sub);
                    socket->set(zmq::sockopt::subscribe, "");
                    socket->connect(data_address);
                    while (receive_flag_)
                    {
                        if (switch_flag_)
                        {
                            if(proto_flag)
                            {
                                proto_pool_->UpdateProtoContent(proto_address);
                                proto_flag = false;
                            }
                            if(FLAGS_v_capilot_version >= 3.2)
                            {
                                socket->recv(msg);
                                this->msgs_queue_.Enqueue(msg.to_string());
                            }
                            else
                            {
                                socket->recv(msg);
                                char topic_name[kTopicNameMaxLen];
                                memset(topic_name, 0, kTopicNameMaxLen);
                                memcpy(topic_name, msg.to_string().c_str(), msg.to_string().size());
                                socket->recv(msg);
                                double timestamp;
                                std::istringstream is(msg.to_string());
                                is >> timestamp;
                                size_t batch = 1;
                                // std::cout << "origin: " << topic_name << "----" << std::fixed << timestamp << std::endl;
                                socket->recv(msg);
                                std::string msg_data = msg.to_string();

                                std::string msg_str;
                                msg_str.resize(kTopicNameMaxLen + kTimestampLen + kBatchLen + msg_data.size());
                                memcpy(&msg_str[0], &topic_name, kTopicNameMaxLen);
                                memcpy(&msg_str[kTopicNameMaxLen], &timestamp, kTimestampLen);
                                memcpy(&msg_str[kTopicNameMaxLen + kTimestampLen], &batch, kBatchLen);
                                memcpy(&msg_str[kTopicNameMaxLen + kTimestampLen + kBatchLen], msg_data.c_str(), msg_data.size());
                                this->msgs_queue_.Enqueue(msg_str);    
                            }
                            this->condition_var_.notify_one();
                        }
                        else
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        }                   
                    }
                    zmq_close(&socket); });
    t.detach();
}

void CReceiveData::DeliverData()
{
    std::thread t([this] {
                      std::string msg;
                      std::string topic_name;
                      double timestamp;
                      size_t batch = 0;
                      std::string msg_data;
                      while (this->receive_flag_)
                      {
                        {
                            std::unique_lock<std::mutex> recv_lock(this->recv_mutex_);
                            if(this->msgs_queue_.Empty())
                                this->condition_var_.wait(recv_lock);
                        }
                        if(this->msgs_queue_.Dequeue(msg))
                        {
                            /** @brief parse data*/
                            this->SplitRecvData(msg.data(), msg.size(), topic_name, timestamp, batch, msg_data);
                            if(FLAGS_v_parser)
                            {
                                // std::cout << "split: " << topic_name << ">>>" << std::fixed  << timestamp << ">>>" << batch << std::endl;
                                this->parser_manager_->Parser(timestamp, topic_name, msg_data);
                            }
                            /** @brief save data for record*/
                            if(FLAGS_v_total_record || FLAGS_v_point_record)
                            {
                                RawData rdata(topic_name, timestamp, msg_data);
                                {
                                    std::lock_guard<std::mutex> save_lg(this->data_center_->record_mutex_);
                                    this->data_center_->data_buf_.insert({timestamp, rdata.rawData()});
                                }
                                if(FLAGS_v_point_record)
                                {
                                    emit this->signal_manager_->SigUpdateDataBufSize();
                                    if(this->data_center_->data_buf_.size() >= FLAGS_v_point_record_size)
                                    {
                                        auto it = this->data_center_->data_buf_.begin();
                                        this->data_center_->data_buf_.erase(it);
                                    }
                                }
                            }
                            
                        }
                      } });
    t.detach();
}

void CReceiveData::SplitRecvData(const char *data, size_t size,
                                 std::string &topic_name, double &timestamp,
                                 size_t &batch, std::string &msg_data)
{
    assert(size > kTopicNameMaxLen + kTimestampLen);

    /// get topic name
    char topic_name_array[kTopicNameMaxLen];
    memcpy(topic_name_array, data, kTopicNameMaxLen);
    topic_name = topic_name_array;
    // int pos = topic_name.find_first_of("-");
    // if (pos != topic_name.npos)
    // {
    //     int length = topic_name.length() - pos;
    //     topic_name = topic_name.substr(pos + 1, length);
    // }
    /// get timestamp
    memcpy(&timestamp, &data[kTopicNameMaxLen], kTimestampLen);
    /// get batch
    memcpy(&batch, &data[kTopicNameMaxLen + kTimestampLen], kBatchLen);
    /// get msg data
    auto ptr = &data[kTopicNameMaxLen + kTimestampLen + kBatchLen];
    size_t len = size - kTopicNameMaxLen - kTimestampLen - kBatchLen;
    msg_data = std::string(ptr, len);
}

/***************************************camera data receive*********************/
void CReceiveData::ReceiveCameraData()
{
    if (CreateCameraCapture())
    {
        qDebug() << "create camera capture failed, no usb camera!!";
        return;
    }
    std::thread t([=] {
        while (receive_flag_)
        {
            if (switch_flag_)
            {
                for (size_t i = 0; i < camera_captures_.size(); i++)
                {
                    cv::Mat m;
                    *camera_captures_.at(i) >> m;
                    if (!m.empty())
                    {
                        double timestamp = KTime().getTime();
                        std::vector<unsigned char> data;
                        cv::imencode(".jpg", m, data, {cv::IMWRITE_JPEG_QUALITY, 30});
                        std::string raw_data;
                        raw_data.resize(data.size());
                        memcpy(&raw_data[0], &data[0], data.size());
                        std::string topic_name = "Idraw-Struct.CameraData-usb%1" + std::to_string(i);
                        if(FLAGS_v_parser)
                        {
                            parser_manager_->Parser(timestamp, topic_name, raw_data);
                        }
                        /** @brief save data for record*/
                        if (FLAGS_v_total_record || FLAGS_v_point_record)
                        {
                            RawData rdata(topic_name, timestamp, raw_data);
                            {
                                std::lock_guard<std::mutex> save_lg(this->data_center_->record_mutex_);
                                this->data_center_->data_buf_.insert({timestamp, rdata.rawData()});
                            }
                            if(FLAGS_v_point_record)
                            {
                                emit this->signal_manager_->SigUpdateDataBufSize();
                                if(this->data_center_->data_buf_.size() >= FLAGS_v_point_record_size)
                                {
                                    auto it = this->data_center_->data_buf_.begin();
                                    this->data_center_->data_buf_.erase(it);
                                }
                            }
                        }
                    }
                    else
                    {
                        qDebug() << "usb camera " << i << " no data to receive";
                    }
                }
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        } });
    t.detach();
}

bool CReceiveData::CreateCameraCapture()
{
    for (auto i : camera_captures_)
    {
        delete i;
    }
    SafeClear(camera_captures_);
    for (int i = 0; i < FLAGS_v_camera_num; ++i)
    {
        cv::VideoCapture *capture = new cv::VideoCapture(i);
        if (!capture->isOpened())
        {
            qDebug() << "camera open failed, camera num =" << i;
            continue;
        }
        capture->set(cv::CAP_PROP_FPS, 15);
        camera_captures_.push_back(capture);
    }

    return camera_captures_.empty();
}

void CReceiveData::ReceiveDDSData(const std::string &config_path, const std::string &topic_name)
{
    std::thread t([=] {
                      ddsrt_setenv("AUTOCOREDDS_URI", config_path.data());
                      dds_entity_t participant;
                      dds_entity_t topic;
                      dds_entity_t reader;
                      DDSData_Msg *msg;
                      void *samples[MAX_SAMPLES];
                      dds_sample_info_t infos[MAX_SAMPLES];
                      dds_return_t rc;
                      dds_qos_t *qos;

                      participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
                      if (participant < 0)
                      {
                          std::cout << "dds创建Participant失败, 错误: " << dds_strretcode(-participant) << std::endl;
                          return;
                      }

                      topic = dds_create_topic(participant, &DDSData_Msg_desc,
                                               topic_name.c_str(), NULL, NULL);
                      if (topic < 0)
                      {
                          std::cout << "dds_create_topic:" << dds_strretcode(-topic) << std::endl;
                          return;
                      }

                      qos = dds_create_qos();
                      dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS(10));
                      reader = dds_create_reader(participant, topic, qos, NULL);
                      if (reader < 0)
                      {
                          std::cout << "dds_create_reader:" << dds_strretcode(-reader) << std::endl;
                          return;
                      }
                      dds_delete_qos(qos);

                      samples[0] = DDSData_Msg__alloc();
                      std::string data;

                      while (receive_flag_)
                      {
                          if (!switch_flag_)
                          {
                              std::this_thread::sleep_for(std::chrono::milliseconds(100));
                              continue;
                          }

                          rc = dds_read(reader, samples, infos, MAX_SAMPLES, MAX_SAMPLES);
                          if (rc < 0)
                          {
                              std::cout << "dds_read:" << dds_strretcode(-rc) << std::endl;
                              dds_sleepfor(DDS_MSECS(20));
                              continue;
                          }

                          if (infos[0].valid_data)
                          {
                            msg = static_cast<DDSData_Msg *>(samples[0]);
#if 0
                            std::cout << "=== [Subscriber] Received : \n" << std::endl;
                            std::cout << "topic:" << msg->topic << std::endl;
                            std::cout << "time:" << msg->timestamp << std::endl;
                            std::cout << "data max size:" << msg->payload._maximum << std::endl;
                            std::cout << "data size:" << msg->payload._length << std::endl;
                            std::cout << "Message:" << msg->payload._buffer << std::endl;
#endif
                            if(msg)
                            {
                                DDSMsgToString(msg, data);
                                msgs_queue_.Enqueue(data);
                                condition_var_.notify_one();
                            }
                          }
                          else
                          {
                            dds_sleepfor(DDS_MSECS(20));
                          }
                      }
                      DDSData_Msg_free(samples[0], DDS_FREE_ALL);
                      rc = dds_delete(participant);
                      if (rc != DDS_RETCODE_OK)
                          std::cout << "dds_delete:" << dds_strretcode(-rc) << std::endl; });
    t.detach();
}

void CReceiveData::DDSMsgToString(DDSData_Msg *msg, std::string &desc_data)
{
    desc_data.resize(kTopicNameMaxLen + kTimestampLen + kBatchLen + msg->payload._length);
    char topic_name[kTopicNameMaxLen];
    memset(topic_name, 0, kTopicNameMaxLen);
    memcpy(&topic_name, msg->topic, std::string(msg->topic).size());
    memcpy(&desc_data[0], &topic_name, kTopicNameMaxLen);
    memcpy(&desc_data[kTopicNameMaxLen], &msg->timestamp, kTimestampLen);

    size_t batch = 1;
    memcpy(&desc_data[kTopicNameMaxLen + kTimestampLen], &batch, kBatchLen);

    memcpy(&desc_data[kTopicNameMaxLen + kTimestampLen + kBatchLen],
           msg->payload._buffer, msg->payload._length);
}