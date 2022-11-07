#include "ccamera.h"
#include "cglobal_param.h"
#include "opencv2/opencv.hpp"

CCamera::CCamera(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    InitCamera();
}

void CCamera::InitCamera()
{
    QVBoxLayout *camera_layout = new QVBoxLayout(this);
    camera_layout->setSpacing(0);
    camera_layout->setContentsMargins(0, 0, 0, 0);
    camera_label_ = new QLabel();
    camera_label_->setStyleSheet("QLabel{background-color:rgb(0, 0, 0);}");
    camera_layout->addWidget(camera_label_);
}

void CCamera::UpdateView(const std::vector<unsigned char> &img, const QString &time_str, double timestamp)
{
    if (img.empty())
        return;

    cv::Mat m = cv::imdecode(img, cv::IMREAD_COLOR);
    if (!m.empty())
    {
        cv::Mat rgb_, dst;
        cv::cvtColor(m, rgb_, CV_BGR2RGB);
        cv::resize(rgb_, dst, cv::Size(camera_label_->width(), camera_label_->height()));
        cv::putText(dst, time_str.toStdString(), cv::Point(0, 20), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(214, 114, 113), 1);
        QImage image(dst.data, dst.cols, dst.rows, dst.cols * dst.channels(), QImage::Format_RGB888);
        camera_label_->setPixmap(QPixmap::fromImage(image));
    }
}
