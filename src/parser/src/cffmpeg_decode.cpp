// Copyright 2021 Horizon Robotics

#include "cffmpeg_decode.h"
#include <iostream>
#include <string>

namespace cav
{

    CH265Decoder::CH265Decoder()
    {
        Inital();
    }

    CH265Decoder::~CH265Decoder()
    {
        Destory();
    }

    void CH265Decoder::Inital()
    {
        codec_ = avcodec_find_decoder(AV_CODEC_ID_HEVC);
        codec_context_ = avcodec_alloc_context3(codec_);
        codec_context_->thread_count = 8;
        if (avcodec_open2(codec_context_, codec_, NULL) < 0)
        {
            qDebug() << "open codec_ fail";
        }
        avframe_yuv_ = av_frame_alloc();
        if (!avframe_yuv_)
        {
            qDebug() << " allocate video frame fail";
        }
    }

    void CH265Decoder::Destory()
    {
        avcodec_close(codec_context_);
        av_free(codec_context_);
        av_frame_free(&avframe_yuv_);
    }

    void CH265Decoder::Restart()
    {
        Destory();
        Inital();
    }

    cv::Mat CH265Decoder::Decode(uint8_t *data, uint32_t data_size)
    {
        AVPacket avpkt_h265;
        av_init_packet(&avpkt_h265);
        avpkt_h265.data = data;
        avpkt_h265.size = data_size;
        if (avcodec_send_packet(codec_context_, &avpkt_h265))
        {
            std::cout << "avcodec_send_packet fail" << std::endl;
            return cv::Mat();
        }
        if (avcodec_receive_frame(codec_context_, avframe_yuv_))
        {
            std::cout << "avcodec_receive_frame fail" << std::endl;
            return cv::Mat();
        }
        int h = avframe_yuv_->height;
        int w = avframe_yuv_->width;
        uint8_t *yuv_buf = new uint8_t[h * w * 3];

        cv::Mat mat_rgb;
        mat_rgb.create(avframe_yuv_->height, avframe_yuv_->width, CV_8UC3);

        for (int i = 0; i < h; ++i)
        {
            memcpy(yuv_buf + w * i, avframe_yuv_->data[0] + avframe_yuv_->linesize[0] * i,
                   w);
        }

        for (int i = 0; i < h / 2; ++i)
        {
            memcpy(yuv_buf + w * h + w / 2 * i,
                   avframe_yuv_->data[1] + avframe_yuv_->linesize[1] * i, w / 2);
        }

        for (int i = 0; i < h / 2; ++i)
        {
            memcpy(yuv_buf + w * h + w / 2 * (h / 2) + w / 2 * i,
                   avframe_yuv_->data[2] + avframe_yuv_->linesize[2] * i, w / 2);
        }

        cv::Mat mat_yuv(h + h / 2, w, CV_8UC1, yuv_buf);
        cv::cvtColor(mat_yuv, mat_rgb, cv::COLOR_YUV420p2RGB);

        delete[] yuv_buf;
        cv::Mat dst;
    	cv::resize(mat_rgb,dst,cv::Size(640,480));
    	return dst;
    }

} // namespace matrix_sample
