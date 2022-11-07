#ifndef CAPILOT_UTILITY_H
#define CAPILOT_UTILITY_H

#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>
namespace capilot {

void createFolderIfNotExist(const std::string& name);

std::string toLowerStr(const std::string& str);

template <typename T>
T str2any(const std::string& str) {
    std::istringstream is(str);
    T res;
    is >> res;
    return res;
}

std::string addSlashToFilePathEnd(std::string& path);

std::string replaceAllDistinct(const std::string& str,
                               const std::string& old_value,
                               const std::string& new_value);

bool IsMatchWildcard(const std::string& str, const std::string& wildcard);

bool IsMatchRegex(const std::string& str, const std::string& regex_str);

void SplitRecvData(const void* data, size_t size, std::string& topic_name,
                   double& timestamp, size_t& batch, std::string& msg_data);

/**
 * @brief 将topic_name,timestamp,batch,和要发送的数据一起合并为连续数据流
 *
 * @param buffer 保存合并后数据的缓存空间，函数内部自动根据需求可自动增大空间
 * @param topic_name topic名称
 * @param timestamp 发送时间戳
 * @param batch 发送批次
 * @param data 数据首地址指针
 * @param len 数据长度
 * @return size_t 合并后数据长度
 */
size_t MergeSendData(std::string& buffer, const std::string& topic_name,
                     double timestamp, size_t batch, const void* data,
                     size_t len);
bool ChangeProcessName(const std::string& name);

template <class T>
std::string ListToString(T&& l) {
    if (l.empty()) {
        return "[]";
    } else {
        std::ostringstream ostr;
        ostr << "[";
        auto it = l.begin(), itend = --l.end();
        for (; it != itend; ++it) {
            ostr << *it << ",";
        }

        ostr << *it << "]";
        return ostr.str();
    }
}

int StringToList(const std::string& str, std::vector<std::string>* mylist);

std::string GetFileContent(const std::string& file);

inline std::string GetFullTopicName(const std::string& topic_name,
                                    const std::string& task_name) {
    return task_name.empty() ? topic_name : task_name + "-" + topic_name;
}

std::string GetMessageNameFromTopicName(const std::string& topic_name);
std::string GetMessageNameFromFullTopicName(const std::string& full_topic_name);
std::string GetTopicNameFromFullTopicName(const std::string& full_topic_name);

}  // namespace capilot

#endif  // CAPILOT_UTILITY_H
