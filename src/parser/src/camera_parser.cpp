#include "camera_parser.h"
#include "raw_data.pb.h"

CameraParser::CameraParser() {}

void CameraParser::ParseCamera(const QString &msg_name, const std::string &data, double time)
{
    bool rawDataFlag = msg_name.contains("RawData-usb");
    rawDataFlag |= msg_name.contains("RawData-fc");
    rawDataFlag |= msg_name.contains("RawData_fc");
    bool cameraDataFlag = msg_name.contains("CameraData-usb");
    if (!rawDataFlag && !cameraDataFlag)
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
            image.img.resize(image_data.size());
            memcpy(image.img.data(), image_data.data(), image_data.size());
            data_center_->InsertValue(msg_name, time, image);
        }
    }
    else if (cameraDataFlag)
    {
        std::vector<unsigned char> d(data.begin(), data.end());
        CImageData image;
        image.img = d;
        data_center_->InsertValue(msg_name, time, image);
    }
    ParseFinished("camera", time);
}
