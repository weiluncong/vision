#ifndef CCAMERAWIDGET_H
#define CCAMERAWIDGET_H

#include <QtWidgets>
#include <QCloseEvent>
#include <iostream>
#include "cglobal_param.h"
#include "ccamera.h"
#include "cslider.h"

class CCameraWidget : public QWidget
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

public:
    CCameraWidget(QWidget *parent = nullptr);
    ~CCameraWidget();

    QString Name() const { return name_; }
    void SetName(const QString &name)
    {
        name_ = name;
        setWindowTitle(name_);
    }
    void SetCameraList(const QStringList &camera_list);
    void SetSliderRange(double max)
    {
        if (!FLAGS_v_online)
            slider_->setRange(0, max * 100);
    }
    void SetSliderStep(int value)
    {
        if (!FLAGS_v_online)
            slider_->setSingleStep(value);
    }

    void UpdateView(const std::vector<unsigned char> &img_data, const QString &time_str, double time);

private slots:
    void HandleActAddCamera();

private:
    void InitUi();

private:
    CCamera *camera_;
    CSlider *slider_;
    QMenu *camera_menu_;
    QString name_;
    std::vector<unsigned char> last_img_data_;
    QString last_time_str_;
    double last_time_;

signals:
    void SigCameraClose();
};

#endif // CCAMERAWIDGET_H
