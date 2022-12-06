#ifndef CCAMLINE_PARSER_H
#define CCAMLINE_PARSER_H

#include "abstract_parser.h"
#include "cline_struct.h"

class CCamLineParser : public AbstractParser
{
public:
    CCamLineParser() {}
    ~CCamLineParser() {}

    void ParseCamLines(const QString &package_msg_name, const google::protobuf::Message &msg, double time);
    void ParseLine(const google::protobuf::Message &msg, CLineData &line);
};

#endif // CCAMLINE_PARSER_H