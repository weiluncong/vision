#include "cyaml_handler.h"
#include <unistd.h>

CYamlHandler::CYamlHandler(const std::string &yaml_path)
{
    if (!yaml_path.empty())
    {
        yaml_path_ = yaml_path;
        LoadYaml();
    }
}

// 加载yaml文件，如果yaml文件不存在，则会提示文件不存在
void CYamlHandler::LoadYaml()
{
    if (access(yaml_path_.c_str(), 0) != -1)
    {
        config_ = YAML::LoadFile(yaml_path_);
    }
    else
    {
        std::cout << yaml_path_ << " yaml file is not exist!" << std::endl;
    }
}

// 存储创建并写入的yaml文件，支持先打开yaml文件，再写入；先打开yaml再写入会导致注释消失
void CYamlHandler::SaveYaml(const std::string &path)
{
    std::lock_guard<std::mutex> lock(mutex_);
    ofs_.open(path);
    if (!ofs_.is_open())
    {
        throw std::runtime_error(path + " open failed!!!");
    }
    ofs_ << config_;
    ofs_.close();
}