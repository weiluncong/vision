#ifndef CPROTO_POOL_H
#define CPROTO_POOL_H

#include <QObject>
#include <fstream>
#include <mutex>
#include <unistd.h>
#include <sys/stat.h>
#include <QDebug>
#include "cglobal_param.h"
#include "google/protobuf/descriptor_database.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/dynamic_message.h"
#include "kzmq.h"
#include "kinterface_content.pb.h"

class CProtoPool : public QObject
{
    Q_OBJECT
private:
    explicit CProtoPool()
    {
    }
    ~CProtoPool() {}
    CProtoPool(const CProtoPool &) = delete;
    CProtoPool &operator=(const CProtoPool &) = delete;
    static CProtoPool *proto_pool_;

public:
    static CProtoPool *GetCProtoPool()
    {
        if (proto_pool_ == NULL)
        {
            proto_pool_ = new CProtoPool;
        }
        return proto_pool_;
    }
    std::string GetProtoContent()
    {
        return proto_content_;
    }
    std::string GetAllProtoContent()
    {
        return all_proto_content_;
    }
    void LoadProtoContent(const std::string &content);
    void UpdateProtoContent(const std::string &serviceAddress);
    google::protobuf::Message *GetProtoMessage(const std::string &topic_name,
                                               const std::string &package_msg_name, const std::string &data);
    google::protobuf::Message *CreateMessage(const std::string &package_msg_name);

public:
    google::protobuf::compiler::Importer *importer_ = nullptr;
    QMap<QString, QString> proto_content_map_;

private:
    google::protobuf::DynamicMessageFactory factory_;
    std::map<std::string, google::protobuf::Message *> msgs_;
    TaskInterfaceContent task_interface_content_;
    std::string proto_content_;
    std::string all_proto_content_;
    std::mutex proto_mutex_;
};

#endif // CPROTO_POOL_H
