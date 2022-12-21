#ifndef CAMERA_PARSER_H
#define CAMERA_PARSER_H

#define CAMERA_NUM_ 2

#include "abstract_parser.h"
#include "cdata_center.h"
#include "cvision_obj_struct.h"
#include "cffmpeg_decode.h"
#include "capilot_image_frame.h"

class CameraParser : public AbstractParser
{
public:
    CameraParser();
    ~CameraParser();

    void ParseCamera(const QString &package_msg_name, const std::string &data, double time);

private:
    // int frame_id_ = 0;
    // int p_num_ = 0;
    QMap<int, std::pair<int, int>> frame_id_; //first key is frame, second key is I-key,value is P-Kye;
    CapilotMultiImageFrame image_265_frames_; //视觉多路H265格式视频集合
    QVector<cav::CH265Decoder *> decoder_vec_;
    bool decoder_had_init_ = false;

    /**
     * @brief 初始化H265解码器
     */
    void DecoderInitail();

    /**
     * @brief 解析H265数据
     * 
     * @param data 一帧视频数据原始数据
     * @param size 一帧视频数据原始数据大小
     * @param num 摄像头序号
     * @param vision_image CImageData数据，用于填值
     */
    void ParserH265Image(const uint8_t *data, int size, int num, CImageData &vision_image); 
};

#endif // CAMERA_PARSER_H
