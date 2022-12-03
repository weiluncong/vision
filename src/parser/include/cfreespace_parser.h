#ifndef CFREESPACE_PARSER_H
#define CFREESPACE_PARSER_H

#include "abstract_parser.h"
#include "cpoint_struct.h"

class CFreeSpaceParser : public AbstractParser
{
public:
    CFreeSpaceParser();
    ~CFreeSpaceParser() {}

    void ParseFreeSpace(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
};

#endif // CFREESPACE_PARSER_H
