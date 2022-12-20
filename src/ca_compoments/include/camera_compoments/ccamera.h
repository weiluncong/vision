#ifndef CCAMERA_H
#define CCAMERA_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <iostream>
#include "opencv2/opencv.hpp"

class CCamera : public QWidget
{
    Q_OBJECT
public:
    explicit CCamera(QWidget *parent = nullptr);
    virtual ~CCamera() {}

public:
    void UpdateView(const std::vector<unsigned char> &img, const QString &time_str, double timestamp);
    void UpdateView(const cv::Mat &img, const QString &time_str, double timestamp);

private:
    void InitCamera();

private:
    QLabel *camera_label_ = nullptr;
};

#endif // KCAMERA_H
