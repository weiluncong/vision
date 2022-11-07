#include "cdata_record.h"

CDataRecord::CDataRecord(const std::string &file_name)
    : file_name_(file_name)
{
}

CDataRecord::~CDataRecord()
{
    Close();
}

void CDataRecord::Init()
{
    int res = posix_memalign((void **)&data_write_buf_, 4096, cav::CRawDataBufferMaxSize);
    if (res != 0)
    {
        throw std::runtime_error("init recorder buf failed!");
    }
    fd_ = open(file_name_.c_str(), O_WRONLY | O_CREAT | O_DIRECT, 0777);
    if (fd_ == -1)
    {
        std::cerr << "file open failed! Please check recordFilePos is exist?" << file_name_ << std::endl;
        exit(1);
    }

    cav::CDataFileHead file_head;
    std::string comment_str = "";
    file_head.version = 1.2;
    file_head.comment_length = comment_str.size();
    file_head.proto_content_size = proto_content_.size();

    /** @brief write file head **/
    memcpy(&data_write_buf_[data_buf_cur_size_], (char *)&file_head, sizeof(file_head));
    data_buf_cur_size_ += sizeof(file_head);
    /** @brief write proto content **/
    memcpy(&data_write_buf_[data_buf_cur_size_], proto_content_.c_str(), proto_content_.size());
    data_buf_cur_size_ += proto_content_.size();
    /** @brief write comment **/
    memcpy(&data_write_buf_[data_buf_cur_size_], comment_str.c_str(), comment_str.size());
    data_buf_cur_size_ += comment_str.size();
}

void CDataRecord::Record(const std::string &data)
{
    std::lock_guard<std::mutex> record_lg(record_mutex_);
    int data_size = data.size();
    recorded_size_ += data_size;
    if (data_buf_cur_size_ + data_size <= cav::CRawDataBufferMaxSize)
    {
        memcpy(&data_write_buf_[data_buf_cur_size_], &data[0], data_size);
        data_buf_cur_size_ += data_size;
    }
    else
    {
        size_t copy_len = cav::CRawDataBufferMaxSize - data_buf_cur_size_;
        size_t left_len = data_size - copy_len;
        memcpy(&data_write_buf_[data_buf_cur_size_], &data[0], copy_len);
        int res = write(fd_, data_write_buf_, cav::CRawDataBufferMaxSize);
        if (res == -1)
        {
            throw std::runtime_error("write data met error!!");
        }

        memcpy(&data_write_buf_[0], &data[left_len], left_len);
        data_buf_cur_size_ = left_len;
    }
}

void CDataRecord::Close()
{
    std::lock_guard<std::mutex> record_lg(record_mutex_);
    if (data_buf_cur_size_ != 0)
    {
        // int residues = data_buf_cur_size_ % KPageSize;
        int quotient = data_buf_cur_size_ / cav::CPageSize;

        int write_byte = (quotient + 1) * cav::CPageSize;
        int more_byte = write_byte - data_buf_cur_size_;

        int res = write(fd_, data_write_buf_, write_byte);
        if (res == -1)
        {
            throw std::runtime_error("write data met error");
        }
        //删除多写入的字节
        off_t curpos = lseek(fd_, 0, SEEK_CUR);
        if (ftruncate(fd_, curpos - more_byte) == -1)
        {
            throw std::runtime_error("ftruncate extra byte error");
        }
    }

    close(fd_);
    fd_ = -1;
    delete[] data_write_buf_;
}