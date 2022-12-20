#ifndef CYAML_HANDLER_H
#define CYAML_HANDLER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>
#include "yaml-cpp/yaml.h"

class CYamlHandler
{
public:
    CYamlHandler(const std::string &yaml_path = "");
    ~CYamlHandler() {}
    void SaveYaml(const std::string &path);

    // read yaml
    template <typename T>
    T GetScalarItem(const std::string &key_name);
    template <typename T>
    T GetMapItem(const std::string &key_name, const std::string &item_name);
    template <typename T>
    std::vector<T> GetSequenceItem(const std::string &key_name);

    // write yaml
    template <typename T>
    void SetScalarItem(const std::string &key_name, const T &val);
    template <typename T>
    void SetMapItem(const std::string &key_name, const std::string &val_name, const T &val);

    YAML::Node &getNode()
    {
        return config_;
    }

protected:
    void LoadYaml();

protected:
    YAML::Node config_;
    std::string yaml_path_;
    std::ofstream ofs_;
    std::mutex mutex_;
};

// 获取纯量类型的数据
template <typename T>
inline T CYamlHandler::GetScalarItem(const std::string &key_name)
{
    try
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return config_[key_name].as<T>();
    }
    catch (...)
    {
        std::cout << "key:" << key_name << " failed!!" << std::endl;
    }

    return {};
}
// 获取对象map类型的数据
template <typename T>
inline T CYamlHandler::GetMapItem(const std::string &key_name, const std::string &item_name)
{
    try
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return config_[key_name][item_name].as<T>();
    }
    catch (...)
    {
        std::cout << "key:" << key_name << " item:" << item_name
                  << " failed!!" << std::endl;
    }

    {};
}
// 获取数组(序列)sequence类型的数据
template <typename T>
inline std::vector<T> CYamlHandler::GetSequenceItem(const std::string &key_name)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<T> res;
    for (int i = 0; i < config_[key_name].size(); ++i)
    {
        res.push_back(config_[key_name][i].as<T>());
    }
    return res;
}
// 写入纯量类型数据，写入的val可以是基本类型，也可以是map、vector等
template <typename T>
inline void CYamlHandler::SetScalarItem(const std::string &key_name, const T &val)
{
    std::lock_guard<std::mutex> lock(mutex_);
    config_[key_name] = val;
}
// 写入map类型数据，写入的val可以是基本类型，也可以是map、vector等
template <typename T>
inline void CYamlHandler::SetMapItem(const std::string &key_name, const std::string &val_name, const T &val)
{
    std::lock_guard<std::mutex> lock(mutex_);
    config_[key_name][val_name] = val;
}
#endif // YAML_HANDLER_H
