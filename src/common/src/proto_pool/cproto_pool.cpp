#include "cproto_pool.h"

CProtoPool *CProtoPool::proto_pool_ = GetCProtoPool();

//第二次 映射，将框架proto加入impoter中
void CProtoPool::LoadProtoContent(const std::string &content)
{
    std::vector<std::string> pcontent;
    std::ofstream ofs;
    task_interface_content_.ParseFromString(content);

    int res = access("./dbc/proto/", 0);
    if (res == -1)
    {
        int r = mkdir("./dbc/proto/", 0777);
    }

    for (auto proto_it = task_interface_content_.content().cbegin();
         proto_it != task_interface_content_.content().cend(); ++proto_it)
    {
        pcontent.push_back(proto_it->first);
        proto_content_map_[QString::fromStdString(proto_it->first)] =
            QString::fromStdString(proto_it->second);
        ofs.clear();
        ofs.open("./dbc/proto/" + proto_it->first);
        if (!ofs.is_open())
        {
            std::cout << "open file " << proto_it->first << " error" << std::endl;
        }
        ofs << proto_it->second;
        ofs.close();
    }

    google::protobuf::compiler::DiskSourceTree source_tree;
    source_tree.MapPath("", "./dbc/proto/");
    if (!importer_)
        importer_ = new google::protobuf::compiler::Importer(&source_tree, NULL);
    for (auto i : pcontent)
    {
        auto file_des = importer_->Import(i);
        if (file_des == NULL)
        {
            std::cout << "import file " << i << " error" << std::endl;
        }
    }
}

google::protobuf::Message *CProtoPool::GetProtoMessage(const std::string &topic_name,
                                                       const std::string &package_msg_name, const std::string &data)
{
    auto it = msgs_.find(package_msg_name);
    if (it == msgs_.end())
    {
        msgs_[package_msg_name] = CreateMessage(package_msg_name);
    }
    google::protobuf::Message *msg = msgs_[package_msg_name]->New();
    if (!msg)
    {
        qDebug() << "CProtoPool::GetProtoMessage message_name:"
                 << QString::fromStdString(package_msg_name)
                 << "topic_name:" << QString::fromStdString(topic_name);
    }
    else
    {
        msg->Clear();
        msg->ParseFromString(data);
    }
    return msg;
}

google::protobuf::Message *CProtoPool::CreateMessage(const std::string &package_msg_name)
{
    auto descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(package_msg_name);
    if (descriptor)
    {
        const google::protobuf::Message *protoType = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (protoType)
        {
            return protoType->New();
        }
        else
        {
            qDebug() << "create message failed "
                     << QString::fromStdString(package_msg_name);
        }
    }
    else
    {
        if (importer_ != nullptr)
        {
            descriptor = importer_->pool()->FindMessageTypeByName(package_msg_name);
            if (descriptor)
            {
                return factory_.GetPrototype(descriptor)->New();
            }
        }
        qDebug() << "can't create proto msg "
                 << QString::fromStdString(package_msg_name);
    }
    return nullptr;
}

void CProtoPool::UpdateProtoContent(const std::string &serviceAddress)
{
    KZmq socket;
    socket.open(zmq::socket_type::req);
    socket.connect(serviceAddress);
    InterfaceRequest request;
    InterfaceReply reply;
    request.set_mode(1);
    socket.send(request);
    if (socket.IsHaveData(5000))
    {
        socket.recv(reply);
        if (reply.res())
        {
            for (auto it = reply.all_content().contents().cbegin();
                 it != reply.all_content().contents().cend(); ++it)
            {
                std::lock_guard<std::mutex> proto_lock(proto_mutex_);
                proto_content_ += it->second.SerializeAsString();
            }
            reply.all_content().SerializeToString(&all_proto_content_);
            std::cout << "Init pb content success......" << std::endl;
        }
        else
        {
            std::cout << reply.error_info() << ", try again -proto pool....." << std::endl;
        }
    }
    else
    {
        std::cout << "get proto timeout ...." << std::endl;
    }
    LoadProtoContent(proto_content_);
}
