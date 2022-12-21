#include "camera_parser.h"
#include "raw_data.pb.h"

CameraParser::CameraParser()
{

    decoder_had_init_ = false;
}

CameraParser::~CameraParser()
{
    SafeClear(decoder_vec_);
}

void CameraParser::ParseCamera(const QString &package_msg_name, const std::string &data, double time)
{
    bool raw_data_flag = package_msg_name.contains("RawData-usb");
    raw_data_flag |= package_msg_name.contains("RawData-fc");
    raw_data_flag |= package_msg_name.contains("RawData_fc");
    bool camera_data_flag = package_msg_name.contains("CameraData-usb");
    bool vision_data_flag = package_msg_name.contains("RawImage-fc");

    if (raw_data_flag)
    {
        RawDataProto::RawData raw;
        raw.ParseFromString(data);
        if (raw.has_imagedata())
        {
            auto image_data = raw.imagedata().image();
            CImageData image;
            image.img_.resize(image_data.size());
            memcpy(image.img_.data(), image_data.data(), image_data.size());
            data_center_->InsertValue(package_msg_name, time, image);
            ParseFinished("camera", time);
        }
    }
    else if (camera_data_flag)
    {
        std::vector<unsigned char> d(data.begin(), data.end());
        CImageData image;
        image.img_ = d;
        data_center_->InsertValue(package_msg_name, time, image);
        ParseFinished("camera", time);
    }
    else if (vision_data_flag)
    {
        if (!image_265_frames_.decode_frame((uint8_t *)data.data(), data.size()))
        {
            qDebug() << "frame decode H265 fail, check video is double fc or not";
            return;
        }
        if (!decoder_had_init_)
        {
            DecoderInitail();
            decoder_had_init_ = true;
        }
        for (int i = 0; i < image_265_frames_.frame_count(); i++)
        {
            CImageData vision_image;
            CapilotImageFrame frame = image_265_frames_.get_frame(i);
            ParserH265Image((const uint8_t *)frame.image_data(), frame.image_size(), i, vision_image);
            auto name = package_msg_name + "~" + QString::number(i);
            data_center_->InsertValue(name, time, vision_image);
            ParseFinished("cvision", time);
        }
    }
}

void CameraParser::ParserH265Image(const uint8_t *data, int size, int num, CImageData &vision_image)
{
    int type = (data[4] & 0x7e) >> 1;
    if (type > 0 && type < 9)
    {
        vision_image.type_ = cav::CH265Type::KeyP;
        frame_id_[num].second++;
    }
    else if (type == 32)
    {
        vision_image.type_ = cav::CH265Type::KeyI;
        frame_id_[num].first++;
        frame_id_[num].second = 0;
    }
    vision_image.frame_id_ = frame_id_[num].first;
    vision_image.p_num_ = frame_id_[num].second;
    if (vision_image.p_num_ > 29)
        return;
    cv::Mat src = decoder_vec_.at(num)->Decode(const_cast<uint8_t *>(data), size);
    if (src.empty())
        return;
    cv::imencode(".jpg", src, vision_image.img_, {cv::IMWRITE_JPEG_QUALITY, 30});
}

void CameraParser::DecoderInitail()
{
    for (int i = 0; i < image_265_frames_.frame_count(); i++)
    {
        CH265Decoder *decoder = new cav::CH265Decoder();
        decoder_vec_.push_back(decoder);
    }
}
