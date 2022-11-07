#ifndef KZMQ_H
#define KZMQ_H
#include <google/protobuf/message.h>
#include <zmq.hpp>
#include "calog.h"
class KZmq {
public:
    KZmq();
    ~KZmq();
    void open(zmq::socket_type type);
    zmq::socket_t* socket();

    void connect(const std::string& url);
    void bind(const std::string& url);
    void SetSubscribe(const std::string& filter);

    zmq::recv_result_t recv(zmq::message_t& msg,
                            zmq::recv_flags flag = zmq::recv_flags::none);
    zmq::recv_result_t recv(google::protobuf::Message& msg,
                            zmq::recv_flags flag = zmq::recv_flags::none);

    zmq::send_result_t send(zmq::message_t&& msg,
                            zmq::send_flags flags = zmq::send_flags::none);

    zmq::send_result_t send(zmq::message_t& msg,
                            zmq::send_flags flags = zmq::send_flags::none);

    zmq::send_result_t send(const std::string& msg,
                            zmq::send_flags flags = zmq::send_flags::none);
    zmq::send_result_t send(const google::protobuf::Message& msg,
                            zmq::send_flags flags = zmq::send_flags::none);

    bool IsHaveData(long timeout);

private:
    zmq::context_t* context_ = nullptr;
    zmq::socket_t* socket_ = nullptr;
};

#endif  // KZMQ_H