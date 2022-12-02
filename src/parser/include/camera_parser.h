#ifndef CAMERA_PARSER_H
#define CAMERA_PARSER_H

#include "abstract_parser.h"
#include "cdata_center.h"
#include "cvision_obj_struct.h"
#include "cffmpeg_decode.h"

class CameraParser : public AbstractParser
{
public:
    CameraParser();

    void ParseCamera(const QString &msg_name, const std::string &data, double time);

private:
    CDataCenter *data_center_;
    int frame_id_ = 0;
    int p_num_ = 0;
    cav::CH265Decoder *decoder_;
};

#endif // CAMERA_PARSER_H
