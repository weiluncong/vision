#ifndef COBJECT_PARSER_H
#define COBJECT_PARSER_H

#include "abstract_parser.h"
#include "cobj_struct.h"

class CObjectParser : public AbstractParser
{
public:
    CObjectParser() {}
    ~CObjectParser() {}

    void ParseObjects(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    void ParseObject(const google::protobuf::Message &msg, const double &timestamp, CObjectData &obj);
};

#endif //COBJECT_PARSER_H