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
    explicit AbstractParser() {}
    virtual ~AbstractParser() {}

    std::string FieldTypeConvert(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field);
    QString GetRepeatedMsg(const google::protobuf::Message &msg, const google::protobuf::FieldDescriptor *field, int index);
    void ParseFinished(const QString &type, double timestamp);
};

#endif // ABSTRACT_PARSER_H
