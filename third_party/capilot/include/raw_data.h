#ifndef RAW_DATA_H
#define RAW_DATA_H

#include <string>
#include <cstring>

#define MAX_NAME_SIZE 100

#pragma pack(1)
struct RawDataHead
{
    char name[MAX_NAME_SIZE];
    size_t size;
    double timestamp;
};
#pragma pack()

/**
 * @brief RawData 是在 MetaData的基础上添加了数据头，用以数据存储、读取
 *
 */
class RawData
{
public:
    RawData(const std::string &rawData);
    RawData(const std::string &name, double timestamp, const std::string &metaData);
    ~RawData() {}
    std::string metaData() { return metaData_; }
    std::string name() { return name_; }
    std::string rawData() { return rawData_; }

private:
    std::string name_;
    std::string rawData_;  //加了头的序列化数据
    std::string metaData_; //原始的序列化数据
    RawDataHead head_;

    int headSize_ = sizeof(RawDataHead);
};

#endif // RAW_DATA_H