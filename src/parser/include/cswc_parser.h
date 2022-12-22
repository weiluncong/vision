#ifndef CSWC_PARSER_H
#define CSWC_PARSER_H

#include "abstract_parser.h"

class CSwcParser : public AbstractParser
{
public:
    CSwcParser() {}
    ~CSwcParser() {}

    // parser online  swc data
    void ParseOnlineSwcData(const google::protobuf::Message &msg, const QStringList &online_signals, const QString &topic_name, double timestamp);

    // parser offline swc data
    void ParseOfflineSwcData(const google::protobuf::Message &msg, QStringList &source_names, double timestamp, QMap<double, double> &dat_map);

    void GetMsgSignalName(const QString &source_name, const google::protobuf::Message &msg, bool &flag);
};

#endif // CSWC_PARSER_H
