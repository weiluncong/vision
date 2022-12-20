#include "cmdi_area.h"
#include "ccamera_widget.h"
#include "cgraphic_widget.h"

CMdiArea::CMdiArea(QWidget *parent)
    : QMdiArea(parent)
{
    data_center_ = CDataCenter::GetCDataCenter();
    data_scheduler_ = CDataScheduler::GetCDataScheduler();
    connect(static_cast<CChartScheduler *>(data_scheduler_->widget_schedulers_["graphic"]), &CChartScheduler::addChartItem, this, &CMdiArea::HandleAddGraphicItem);

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
    act_add_vision_camera_ = new QAction(tr("vision_widget"));
    connect(act_add_vision_camera_, &QAction::triggered, this, &CMdiArea::HandleActAddVisionCamera);
    act_add_vehicle_topview_ = new QAction(tr("vehicle_topview"));
    connect(act_add_vehicle_topview_, &QAction::triggered, this, &CMdiArea::HandleActAddVehicleTopView);
    act_add_graphic_ = new QAction(tr("chart_widget"));
    connect(act_add_graphic_, &QAction::triggered, this, &CMdiArea::HandleActAddGraphic);
}

void CMdiArea::CreateMenus()
{
    menu_ = new QMenu(this);
    QMenu *camera_view = new QMenu("&camera view");
    camera_view->addAction(act_add_camera_);
    camera_view->addAction(act_add_vision_camera_);
    QMenu *view_2d = new QMenu("&2d view");
    view_2d->addAction(act_add_vehicle_topview_);
    view_2d->addAction(act_add_graphic_);

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
    QStringList camera_list = {};
    if (img_data_ptr)
    {
        camera_list = img_data_ptr->Keys();
    }
    CCameraWidget *camera = new CCameraWidget(this);
    camera->SetCameraList(camera_list);
    this->addSubWindow(camera);
    camera->SetSliderRange(data_center_->data_end_time_ - data_center_->data_start_time_);
    static_cast<CCameraScheduler *>(data_scheduler_->widget_schedulers_["camera"])->AddCameraWidget(camera);
    camera->show();
    this->activeSubWindow()->setWindowIcon(QIcon(":/icon/vedio.png"));
    this->activeSubWindow()->resize(400, 300);
    this->activeSubWindow()->setMaximumSize(800, 600);
}

void CMdiArea::HandleActAddVehicleTopView()
{
    CTopViewScheduler *topview_scheduler = static_cast<CTopViewScheduler *>(data_scheduler_->widget_schedulers_["topview"]);
    if (!topview_scheduler->GetVehicleWidgetStatus())
    {
        CVehicleTopViewWidget *topview_widget = new CVehicleTopViewWidget(this);
        topview_scheduler->AddTopViewWidget(topview_widget);
        topview_widget->setWindowTitle("vehicle_topview");
        this->addSubWindow(topview_widget);
        topview_widget->show();
        this->activeSubWindow()->setWindowIcon(QIcon(":/icon/topview.png"));
        this->activeSubWindow()->resize(topview_widget->size());
    }
}

void CMdiArea::HandleActAddVisionCamera()
{
    auto vision_img_ptr = data_center_->GetDataPtr<cav::CImageData>();
    QStringList camera_list = {};
    if (vision_img_ptr)
    {
        camera_list = vision_img_ptr->Keys();
    }
    CVisionCameraWidget *camera = new CVisionCameraWidget(this);
    camera->SetCameraList(camera_list);
    this->addSubWindow(camera);
    camera->SetSliderRange(data_center_->data_end_time_ - data_center_->data_start_time_);
    static_cast<CVisionScheduler *>(data_scheduler_->widget_schedulers_["cvision"])->AddCameraWidget(camera);
    camera->show();
    this->activeSubWindow()->setWindowIcon(QIcon(":/icon/vedio.png"));
    this->activeSubWindow()->resize(400, 300);
    this->activeSubWindow()->setMaximumSize(800, 600);
}

void CMdiArea::HandleActAddGraphic()
{
    CGraphicWidget *graphic = new CGraphicWidget();
    graphic->setWindowTitle("graphic");
    this->addSubWindow(graphic);
    static_cast<CChartScheduler *>(data_scheduler_->widget_schedulers_["graphic"])->AddGraphicWidget(graphic);
    graphic->show();


}

void CMdiArea::HandleAddGraphicItem(const QString &add_signal)
{
    if (subWindowList().size() == 0 ||
        !activeSubWindow()->windowTitle().contains("graphic"))
    {
        HandleActAddGraphic();
    }
    static_cast<CGraphicWidget *>(activeSubWindow()->widget())->AddSignals(add_signal);
}