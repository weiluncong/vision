#ifndef CSDA_VISION_PARSER_H
#define CSDA_VISION_PARSER_H

#include "csda_common_parser.h"
#include "csda_rle_decode.h"
#include "capilot_image_frame.h"
#include <mutex>

#define IMAGE_SIZE 480 * 256 * 3

/**
 * @brief SDA数据独有解析类，包含视觉团队所需数据源
 */
class CSdaVisionParser : public CSdaCommmonParser
{
public:
    CSdaVisionParser();
    ~CSdaVisionParser();

    // 视觉总语义分割解析
    void ParseSemantic(const QString &msg_name, const std::string &data, double time);

    // 视觉freespace解析
    void ParserVisionFreespace(const QString &msg_name, const google::protobuf::Message &msg, double time);

private:
    /**
     * @brief RLE数据解码
     * @param data 原始数据
     * @param name 数据topic名称
     * @param time 时间戳
     * @param id 当前数据帧数
     */
    void RLEDecode(const std::vector<uchar> &data, const QString &name, double time, int id);

    /**
     * @brief 获取类型颜色
     * @param type 颜色类型（range 0-15）
     */
    cav::CPointData GetCVColor(int type);

    /**
     * @brief 解析视觉车道线原图坐标点。
     */
    void ParserVisionLane(const google::protobuf::Message &msg, cav::CSDALineCVBlocks &lines);

    /**
     * @brief 解析视觉单个车道线原图坐标点。
     */
    QVector<cv::Point> ParserContour(const google::protobuf::Message &msg);

    /**
     * @brief 解析视觉语义分割信号，用于获取投影在视频中的语义分割地图区域
     */
    void ParseVisionLine(const google::protobuf::Message &msg, cav::CSDALineRleBlocks &blocks);

    /**
     * @brief 解析视觉语义分割信号，用于获取基础色块组
     */
    CSDARLE ParserRLE(const google::protobuf::Message &msg);

    /**
     * @brief 解析视觉语义分割信号，用于获取投影在视频中的语义分割地图区域
     */
    void ParseVisionSemantic(const QString &msg_name, const google::protobuf::Message &msg, double time);

protected:
    cav::CRLECompressedImage *rle_decode_;
    cav::CapilotParsingFrame *vision_decode_;

    int full_num_ = 0;
    bool online_parsing_img_flag = true; //抽帧
};

#endif // CSDA_VISION_PARSER_H