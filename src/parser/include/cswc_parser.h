#ifndef CSWC_PARSER_H
#define CSWC_PARSER_H

#include "abstract_parser.h"

class CSwcParser : public AbstractParser
{
public:
    CSwcParser();
    ~CSwcParser() {}

    void ParseSwcData(const google::protobuf::Message &msg, const QString &package_msg_name, double timestamp, QMap<double, double> &dat_map);

    void ParseAllSignalName(const QString &topic_name, const google::protobuf::Message &msg, bool &flag);
};

#endif // CSWC_PARSER_H
