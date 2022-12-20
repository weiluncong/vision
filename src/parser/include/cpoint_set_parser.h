#ifndef CPOINT_SET_PARSER_H
#define CPOINT_SET_PARSER_H

#include "abstract_parser.h"
#include "cpoint_struct.h"

using namespace cav;

class CPointSetParser : public AbstractParser
{
public:
    CPointSetParser() {}
    ~CPointSetParser() {}

    void ParseFreeSpace(const QString &package_msg_name, const google::protobuf::Message &msg, const double time);
};

#endif // CPOINT_SET_PARSER_H