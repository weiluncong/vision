#include "camera_parser.h"
#include "raw_data.pb.h"

CameraParser::CameraParser()
{
    data_center_ = CDataCenter::GetCDataCenter();
    decoder_ = new cav::CH265Decoder();
}

void CameraParser::ParseCamera(const QString &msg_name, const std::string &data, double time)
{
    bool rawDataFlag = msg_name.contains("RawData-usb");
    rawDataFlag |= msg_name.contains("RawData-fc");
    rawDataFlag |= msg_name.contains("RawData_fc");
    bool cameraDataFlag = msg_name.contains("CameraData-usb");
    bool visionDattaFlag = msg_name.contains("RawImage-fc");

    if (!rawDataFlag && !cameraDataFlag && !visionDattaFlag)
    {
        return;
    }
    if (rawDataFlag)
    {
        RawDataProto::RawData raw;
        raw.ParseFromString(data);
        if (raw.has_imagedata())
        {
            auto image_data = raw.imagedata().image();
            CImageData image;
            image.img_.resize(image_data.size());
            memcpy(image.img_.data(), image_data.data(), image_data.size());
            data_center_->InsertValue(msg_name, time, image);
            ParseFinished("camera", time);
        }
    }
    else if (cameraDataFlag)
    {
        std::vector<unsigned char> d(data.begin(), data.end());
        CImageData image;
        image.img_ = d;
        data_center_->InsertValue(msg_name, time, image);
        ParseFinished("camera", time);
    }
    else if (visionDattaFlag)
    {
        int headSize = sizeof(int32_t) + sizeof(int64_t);
        CImageData vision_image;
        vision_image.img_.resize(data.size() - headSize);
        memcpy(vision_image.img_.data(), (char *)(data.data() + headSize), data.size() - headSize);
        int type = (vision_image.img_[4] & 0x7e) >> 1;
        if (type > 0 && type < 9)
        {
            vision_image.type_ = cav::CH265Type::KeyP;
            p_num_++;
        }
        else if (type == 32)
        {
            vision_image.type_ = cav::CH265Type::KeyI;
            frame_id_++;
            p_num_ = 0;
        }
        vision_image.frame_id_ = frame_id_;
        vision_image.p_num_ = p_num_;
        if (vision_image.p_num_ > 29)
            return;
        cv::Mat src = decoder_->Decode(const_cast<uint8_t *>(vision_image.img_.data()), vision_image.img_.size());
        if (src.empty())
            return;
        std::vector<unsigned char> data_encode;
        cv::imencode(".jpg", src, data_encode, {cv::IMWRITE_JPEG_QUALITY, 30});
        vision_image.img_ = data_encode;

        data_center_->InsertValue(msg_name, time, vision_image);
        ParseFinished("cvision", time);
    }
}
