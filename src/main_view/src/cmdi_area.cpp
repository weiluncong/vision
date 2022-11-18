#include "cmdi_area.h"
#include "ccamera_widget.h"

CMdiArea::CMdiArea(QWidget *parent)
    : QMdiArea(parent)
{
    data_center_ = CDataCenter::GetCDataCenter();
    data_scheduler_ = CDataScheduler::GetCDataScheduler();
    setOption(QMdiArea::DontMaximizeSubWindowOnActivation);
    setBackground(Qt::gray);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    CreateActions();
    CreateMenus();
}

CMdiArea::~CMdiArea()
{
}

void CMdiArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        menu_->exec(QCursor::pos());
    }
}

void CMdiArea::CreateActions()
{
    act_add_camera_ = new QAction(tr("camera_widget"));
    connect(act_add_camera_, &QAction::triggered, this, &CMdiArea::HandleActAddCamera);
}

void CMdiArea::CreateMenus()
{
    menu_ = new QMenu(this);
    QMenu *camera_view = new QMenu("&camera view");
    camera_view->addAction(act_add_camera_);
    QMenu *view_2d = new QMenu("&2d view");

    QMenu *view_3d = new QMenu("&3d view");

    QMenu *info_view = new QMenu("&info view");

    menu_->addMenu(camera_view);
    menu_->addMenu(view_2d);
    menu_->addMenu(view_3d);
    menu_->addMenu(info_view);
}

void CMdiArea::HandleActAddCamera()
{
    auto img_data_ptr = data_center_->GetDataPtr<CImageData>();
    QStringList cameraList = {};
    if (img_data_ptr)
    {
        cameraList = img_data_ptr->Keys();
    }
    CCameraWidget *camera = new CCameraWidget(this);
    camera->SetCameraList(cameraList);
    this->addSubWindow(camera);
    camera->SetSliderRange(data_center_->data_end_time_ - data_center_->data_start_time_);
    static_cast<CCameraScheduler *>(data_scheduler_->widget_schedulers_["camera"])->AddCameraWidget(camera);
    camera->show();
    this->activeSubWindow()->setWindowIcon(QIcon(":/icon/vedio.png"));
    this->activeSubWindow()->resize(400, 300);
    this->activeSubWindow()->setMaximumSize(800, 600);
}
