#ifndef FFMPEG_DECODE_H
#define FFMPEG_DECODE_H

#include <opencv2/opencv.hpp>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}
#include <mutex>
#include <QDebug>

namespace cav
{
    /**
     * @brief H265解析器，用于将H265数据中转为YUA后，生成可用cv:Mat数据
     */
    class CH265Decoder
    {
    public:
        CH265Decoder();
        ~CH265Decoder();
        void Restart();
        cv::Mat Decode(uint8_t *data, uint32_t data_size);

    private:
        void Inital();
        void Destory();
        const AVCodec *codec_;
        AVCodecContext *codec_context_;
        AVFrame *avframe_yuv_;
        AVPacket avpkt_h265_;
        std::mutex decode_mutex_;
    };
} // namespace cav

#endif // FFMPEG_DECODE_H