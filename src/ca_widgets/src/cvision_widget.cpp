#include "cvision_widget.h"

CVisionCameraWidget::CVisionCameraWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(300, 200);
    setMaximumSize(800, 600);
    InitUi();
}

CVisionCameraWidget::~CVisionCameraWidget()
{
    SAFE_DELETE(vision_tool_);
    SAFE_DELETE(camera_);
    SAFE_DELETE(slider_);
}

void CVisionCameraWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    camera_->UpdateView(last_mat_data_, last_time_str_, last_time_);
}

void CVisionCameraWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    camera_menu_->exec(QCursor::pos());
}

void CVisionCameraWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit SigCameraClose();
}

void CVisionCameraWidget::SetCameraList(const QStringList &camera_list)
{
    camera_menu_->clear();
    for (auto camera_name : camera_list)
    {
        QAction *act_set_name = new QAction(QStringLiteral("切换到%1").arg(camera_name), this);
        act_set_name->setObjectName(camera_name);
        connect(act_set_name, &QAction::triggered, this, &CVisionCameraWidget::HandleActAddCamera);
        camera_menu_->addAction(act_set_name);
    }
}

void CVisionCameraWidget::HandleActAddCamera()
{
    QString new_name = qobject_cast<const QAction *>(sender())->objectName();
    SetName(new_name);
}

void CVisionCameraWidget::InitUi()
{
    QVBoxLayout *widget_layout = new QVBoxLayout(this);
    widget_layout->setSpacing(0);
    widget_layout->setContentsMargins(0, 0, 0, 0);

    semantic_show_ = false;
    act_semantic_ = new QAction("全局语义分割", this);
    act_semantic_->setIcon(QIcon(":/icon/Semantic_.png"));
    act_semantic_->setText(tr("show Semantic"));
    connect(act_semantic_, &QAction::triggered, this,
            &CVisionCameraWidget::HandleSemanticActionChanged);

    lane_line_show_ = false;
    act_lane_line_ = new QAction("车道线显示", this);
    act_lane_line_->setIcon(QIcon(":/icon/LaneLine_.png"));
    act_lane_line_->setText(tr("show laneLine"));
    connect(act_lane_line_, &QAction::triggered, this,
            &CVisionCameraWidget::HandleLaneLineActionChanged);

    lane_semantic_show_ = false;
    act_lane_semantic_ = new QAction("车道线语义分割", this);
    act_lane_semantic_->setIcon(QIcon(":/icon/LaneSemantic_.png"));
    act_lane_semantic_->setText(tr("show laneSemantic"));
    connect(act_lane_semantic_, &QAction::triggered, this,
            &CVisionCameraWidget::HandleLaneSemanticActionChanged);

    obj_boundingbox_show_ = false;
    act_obj_boundingbox_ = new QAction("目标框", this);
    act_obj_boundingbox_->setIcon(QIcon(":/icon/VisionObject_.png"));
    act_obj_boundingbox_->setText(tr("show laneSemantic"));
    connect(act_obj_boundingbox_, &QAction::triggered, this,
            &CVisionCameraWidget::HandleObjectBoxActionChanged);

    vision_tool_ = new QToolBar(this);
    vision_tool_->addAction(act_lane_line_);
    vision_tool_->addAction(act_obj_boundingbox_);
    vision_tool_->addSeparator();
    vision_tool_->addAction(act_semantic_);
    vision_tool_->addAction(act_lane_semantic_);

    camera_ = new CCamera(this);
    widget_layout->addWidget(vision_tool_);
    widget_layout->addWidget(camera_);
    if (!FLAGS_v_online)
    {
        slider_ = new CSlider(this);
        widget_layout->addWidget(slider_);
    }
    camera_menu_ = new QMenu(this);
}

void CVisionCameraWidget::UpdateView(const QString &time_str, double time)
{
    std::vector<unsigned char> img;
    if (name_.contains("panorama_semantic"))
        img = semantic_data_.img_;
    else if (name_.contains("lane_semantic"))
        img = lane_semantic_data_.img_;
    else if (name_.contains("Struct.RawImage-fc"))
        img = raw_data_.img_;

    // 视频数据预处理
    if (img.empty())
        return;

    // 记录当前帧所有输入数据
    last_time_str_ = time_str;
    last_time_ = time;
    last_mat_data_ = cv::imdecode(img, cv::IMREAD_COLOR);
    // 数据分模块更新
    if (semantic_show_ && !semantic_data_.img_.empty())
    {
        UpdateSemantic(last_mat_data_, semantic_data_.img_);
    }
    if (lane_semantic_show_ && !lane_semantic_data_.img_.empty())
    {
        UpdateSemantic(last_mat_data_, lane_semantic_data_.img_);
    }
    if (obj_boundingbox_show_ && !object_data_.empty())
    {
        UpdateObjectBox(last_mat_data_, object_data_);
    }
    // if (lane_line_show_)
    // {
    //     UpdateLinePoints(last_mat_data_, lane_cv_blocks_);
    // }

    camera_->UpdateView(last_mat_data_, time_str, time);

    // 当前模式判断
    if (!FLAGS_v_online)
    {
        slider_->setValue((int)(time * 100));
    }
}

void CVisionCameraWidget::HandleSemanticActionChanged()
{
    if (!semantic_show_)
    {
        act_semantic_->setIcon(QIcon(":/icon/Semantic.png"));
        act_semantic_->setText(tr("show Semantic"));
        if (lane_semantic_show_)
        {
            act_lane_semantic_->setIcon(QIcon(":/icon/LaneSemantic_.png"));
            act_lane_semantic_->setText(tr("hide laneSemantic"));
            lane_semantic_show_ = false;
        }
    }
    else
    {
        act_semantic_->setIcon(QIcon(":/icon/Semantic_.png"));
        act_semantic_->setText(tr("hide Semantic"));
    }
    semantic_show_ = !semantic_show_;
    UpdateView(last_time_str_, last_time_);
}

void CVisionCameraWidget::HandleLaneSemanticActionChanged()
{
    if (!lane_semantic_show_)
    {
        act_lane_semantic_->setIcon(QIcon(":/icon/LaneSemantic.png"));
        act_lane_semantic_->setText(tr("show laneSemantic"));

        if (semantic_show_)
        {
            act_semantic_->setIcon(QIcon(":/icon/Semantic_.png"));
            act_semantic_->setText(tr("hide Semantic"));
            semantic_show_ = false;
        }
    }
    else
    {
        act_lane_semantic_->setIcon(QIcon(":/icon/LaneSemantic_.png"));
        act_lane_semantic_->setText(tr("hide laneSemantic"));
    }
    lane_semantic_show_ = !lane_semantic_show_;
    UpdateView(last_time_str_, last_time_);
}

void CVisionCameraWidget::HandleObjectBoxActionChanged()
{
    if (!obj_boundingbox_show_)
    {
        act_obj_boundingbox_->setIcon(QIcon(":/icon/VisionObject.png"));
        act_obj_boundingbox_->setText(tr("show VisionObject"));
    }
    else
    {
        act_obj_boundingbox_->setIcon(QIcon(":/icon/VisionObject_.png"));
        act_obj_boundingbox_->setText(tr("hide VisionObject"));
    }
    obj_boundingbox_show_ = !obj_boundingbox_show_;
    UpdateView(last_time_str_, last_time_);
}

void CVisionCameraWidget::HandleLaneLineActionChanged()
{
    lane_line_show_ = !lane_line_show_;
    if (!lane_line_show_)
    {
        act_lane_line_->setIcon(QIcon(":/icon/LaneLine.png"));
        act_lane_line_->setText(tr("show laneLine"));
    }
    else
    {
        act_lane_line_->setIcon(QIcon(":/icon/LaneLine_.png"));
        act_lane_line_->setText(tr("hide laneLine"));
    }
    UpdateView(last_time_str_, last_time_);
}

void CVisionCameraWidget::UpdateSemantic(cv::Mat &img, std::vector<unsigned char> &img_data)
{
    cv::Mat sematic = cv::imdecode(img_data, cv::IMREAD_COLOR);   
    if (sematic.empty())
        return;
    cv::resize(img, img, cv::Size(480, 270));
    cv::Mat roi = img(cv::Rect(0, 0, 480, 256));
    cv::addWeighted(sematic, 1, roi, 0.8, 0, roi);
}

void CVisionCameraWidget::UpdateObjectBox(cv::Mat &img, QVector<cav::CObjBoxCV> &blocks)
{
    cv::resize(img, img, cv::Size(3840, 2160));
    for (auto obj : blocks)
    {
        cv::rectangle(img, cv::Point(obj.top_left_.x_, obj.top_left_.y_),
                      cv::Point(obj.bottom_right_.x_, obj.bottom_right_.y_),
                      cv::Scalar(0, 0, 255), 3, cv::LINE_4, 0);
    }
}

void CVisionCameraWidget::UpdateLinePoints(cv::Mat &img, QVector<cav::CSDALineCVBlocks> &blocks)
{
    for (auto block : blocks)
    {
        for (auto line : block)
        {
            cv::Scalar color;
            if (line.type_ == cav::CLineType::LaneLine)
                color = cv::Scalar(0, 0, 255);
            else
                color = cv::Scalar(0, 255, 0);
            PaintPoint(img, line.points_, color);
            PaintLine(img, line, color);
        }
    }
}

void CVisionCameraWidget::UpdateLineRLE(cv::Mat &img, QVector<cav::CSDALineRleBlocks> &blocks)
{
    cv::Mat rm;
    cv::resize(img, rm, cv::Size(480, 270));
    for (auto block : blocks)
    {
        cv::Mat src(rm.rows, rm.cols, CV_8UC3, cv::Scalar(40, 40, 40));
        for (auto lineRle : block)
        {
            cv::Scalar color;
            if (lineRle.type_ == cav::CLineType::Unknown)
                color = cv::Scalar(240, 255, 240);
            else if (lineRle.type_ == cav::CLineType::LaneLine)
                color = cv::Scalar(65, 105, 225);
            else if (lineRle.type_ == cav::CLineType::CurbLine)
                color = cv::Scalar(0, 255, 0);
            else if (lineRle.type_ == cav::CLineType::CenterLine)
                color = cv::Scalar(227, 207, 87);
            else if (lineRle.type_ == cav::CLineType::GuardBar)
                color = cv::Scalar(255, 97, 3);
            else if (lineRle.type_ == cav::CLineType::ConcreteBarrier)
                color = cv::Scalar(247, 145, 33);
            else if (lineRle.type_ == cav::CLineType::Fence)
                color = cv::Scalar(255, 128, 0);
            else if (lineRle.type_ == cav::CLineType::Wall)
                color = cv::Scalar(255, 0, 0);
            else if (lineRle.type_ == cav::CLineType::Canopy)
                color = cv::Scalar(50, 205, 50);
            else if (lineRle.type_ == cav::CLineType::Virtual)
                color = cv::Scalar(163, 128, 148);
            else if (lineRle.type_ == cav::CLineType::Cone)
                color = cv::Scalar(255, 255, 0);
            for (auto line : lineRle.line_)
            {
                cv::rectangle(src, cv::Point(line.start_col_, line.row_),
                              cv::Point(line.end_col_, line.row_), color, -1);
            }
        }
        cv::Mat out;
        cv::addWeighted(rm, 1, src, 0.8, 0, out);
        rm = out;
    }
    cv::resize(rm, img, cv::Size(img.cols, img.rows));
}

void CVisionCameraWidget::PaintPoint(cv::Mat &img, const QVector<cv::Point> &points, cv::Scalar color)
{
    for (cv::Point p : points)
    {
        cv::circle(img, p, 5, color, 4);
    }
}

void CVisionCameraWidget::PaintLine(cv::Mat &img, const cav::CSDALineCV &line, cv::Scalar color)
{
    if (line.type_ == cav::CLineType::LaneLine)
    {
        for (int i = 0; i < line.points_.size() - 1; i = i + 2)
        {
            cv::Point stPoint(line.points_[i].x, line.points_[i].y);
            cv::Point edPoint(line.points_[i + 1].x, line.points_[i + 1].y);
            cv::line(img, stPoint, edPoint, color, 4);
        }
    }
    else if (line.type_ == cav::CLineType::CurbLine)
    {
        for (int i = 0; i < line.points_.size() - 1; i++)
        {
            cv::Point stPoint(line.points_[i].x, line.points_[i].y);
            cv::Point edPoint(line.points_[i + 1].x, line.points_[i + 1].y);
            cv::line(img, stPoint, edPoint, color, 4);
        }
    }
}
