#ifndef CAMERA_PARSER_H
#define CAMERA_PARSER_H

#include "abstract_parser.h"
#include "cdata_center.h"

class CameraParser : public AbstractParser
{
public:
    CameraParser();

    void ParseCamera(const QString &msg_name, const std::string &data, double time);

private:
    CDataCenter *data_center_;
};

#endif // CAMERA_PARSER_H
