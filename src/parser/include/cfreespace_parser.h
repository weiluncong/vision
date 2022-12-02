#ifndef FREESPACE_PARSER_H
#define FREESPACE_PARSER_H

#include "abstract_parser.h"
#include "cpoint_struct.h"

class CFreeSpaceParser : public AbstractParser
{
public:
    CFreeSpaceParser();
    ~CFreeSpaceParser() {}

    void ParseFreeSpace(const QString &msg_name, const google::protobuf::Message &msg, const double time);
};

#endif // FREESPACE_PARSER_H
