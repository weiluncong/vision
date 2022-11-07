#ifndef RECORDER_H
#define RECORDER_H

#include <fstream>
#include <map>
#include <mutex>
#include <zmq.hpp>

#include "kinterface_content.pb.h"
#include "node.h"
#include "proto_reflector.h"

const int KPageSize = 4096;
const int KRawDataBufferMaxSize = KPageSize * 1e4;  // 40M

namespace capilot {
// 4096=getpagessize() 系统内存页大小
struct RecordData {
    std::string topicName;
    std::string data;
    double timestamp;
};

class KRecorder {
public:
    KRecorder(const std::string& file_name = "");
    ~KRecorder();
    void Init(const std::string& file_name = "");
    void Record(const std::string& data, const std::string& topic,
                double timestamp);
    void Close();
    size_t RecordSize() { return record_size_; }
    void Flush();

private:
    void MoveToRawdataBufAndWriteWhenBufFull(const std::string& rdata);
    static void InitPbContent();

private:
    int fd_ = -1;
    bool is_close_ = false;
    std::string file_name_;
    int rawdata_buf_cur_size_ = 0;
    char* rawdata_write_buf_ = nullptr;
    std::mutex write_lock_;
    static std::string proto_content_;
    static std::mutex pb_mutex_;

    size_t record_size_ = 0;
};
}  // namespace capilot
#endif  // RECORDER_H