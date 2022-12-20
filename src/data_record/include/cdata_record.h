#ifndef CDATARECORD_H
#define CDATARECORD_H

#include <iostream>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include "cglobal_param.h"
#include "cdat_struct.h"

class CDataRecord
{
public:
    CDataRecord(const std::string &file_name);
    ~CDataRecord();

public:
    void Init();
    void Record(const std::string &data);
    void Close();
    size_t GetRecordedSize() { return recorded_size_; }
    void SetProtoContent(const std::string &proto_content)
    {
        proto_content_ = proto_content;
    }

private:
    int fd_;
    std::string file_name_;
    char *data_write_buf_;
    int data_buf_cur_size_ = 0;
    std::mutex record_mutex_;
    std::string proto_content_;
    size_t recorded_size_ = 0;
};

#endif // CDATARECORD_H