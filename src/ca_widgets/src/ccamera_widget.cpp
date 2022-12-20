#include "ccamera_widget.h"

CCameraWidget::CCameraWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(300, 200);
    setMaximumSize(800, 600);
    InitUi();
}

CCameraWidget::~CCameraWidget()
{
    SAFE_DELETE(camera_);
    SAFE_DELETE(slider_);
}

void CCameraWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!FLAGS_v_online)
        camera_->UpdateView(last_img_data_, last_time_str_, last_time_);
}

void CCameraWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    camera_menu_->exec(QCursor::pos());
}

void CCameraWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit SigCameraClose();
}

void CCameraWidget::InitUi()
{
    QVBoxLayout *widget_layout = new QVBoxLayout(this);
    widget_layout->setSpacing(0);
    widget_layout->setContentsMargins(0, 0, 0, 0);
    camera_ = new CCamera(this);
    widget_layout->addWidget(camera_);
    if (!FLAGS_v_online)
    {
        slider_ = new CSlider(this);
        widget_layout->addWidget(slider_);
    }
    camera_menu_ = new QMenu(this);
}

void CCameraWidget::SetCameraList(const QStringList &camera_list)
{
    camera_menu_->clear();
    for (auto camera_name : camera_list)
    {
        QAction *act_set_name = new QAction(QStringLiteral("切换到%1").arg(camera_name), this);
        act_set_name->setObjectName(camera_name);
        connect(act_set_name, &QAction::triggered, this, &CCameraWidget::HandleActAddCamera);
        camera_menu_->addAction(act_set_name);
    }
}

void CCameraWidget::HandleActAddCamera()
{
    QString new_name = qobject_cast<const QAction *>(sender())->objectName();
    SetName(new_name);
}

void CCameraWidget::UpdateView(const std::vector<unsigned char> &img_data, const QString &time_str, double time)
{
    last_img_data_ = img_data;
    last_time_str_ = time_str;
    last_time_ = time;
    camera_->UpdateView(img_data, time_str, time);
    if (!FLAGS_v_online)
    {
        slider_->setValue((int)(time * 100));
    }
}
