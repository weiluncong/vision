#ifndef ABSTRACT_PARSER_H
#define ABSTRACT_PARSER_H

#include <QObject>

#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/descriptor_database.h>
#include <google/protobuf/compiler/importer.h>
#include "cdata_center.h"

class AbstractParser : public QObject
{
    Q_OBJECT
public:
    explicit AbstractParser()
    {
        data_center_ = CDataCenter::GetCDataCenter();
    }
    virtual ~AbstractParser() {}

    template <typename T>
    void AssignStruct(const google::protobuf::Message &msg,
                      const google::protobuf::Descriptor *descript, T &param, const std::string &name);

    std::string FieldToStr(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field);
    QString FieldToQStr(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field);
    QString GetRepeatedMsg(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field, int index);
    void ParseFinished(const QString &type, double timestamp);

    CDataCenter *data_center_;
};

template <typename T>
void AbstractParser::AssignStruct(const google::protobuf::Message &msg,
                                  const google::protobuf::Descriptor *descript, T &param, const std::string &name)
{
    param = StringToAny<T>(FieldToStr(msg, descript->FindFieldByName(name)));
}

#endif // ABSTRACT_PARSER_H
