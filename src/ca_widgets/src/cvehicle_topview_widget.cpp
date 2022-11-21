#include "cvehicle_topview_widget.h"

CVehicleTopViewWidget::CVehicleTopViewWidget(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    CreateMenu();

    graphics_view_ = new CGraphicsView();
    graphics_scene_ = new CGraphicsScene();
    background_item_ = new CVehicleBackGroundItem(QRectF(-8.0 * 30 + 50, -120 * 0.3 + 20, 16 * 30 - 80, 180 * 3 - 50));

    graphics_scene_->addItem(background_item_);
    graphics_view_->setScene(graphics_scene_);
    graphics_view_->resize(graphics_scene_->width(), graphics_scene_->height());

    setter_btn_ = new QToolButton(this);
    setter_btn_->setMaximumWidth(15);
    setter_btn_->setArrowType(Qt::LeftArrow);
    connect(setter_btn_, &QToolButton::clicked, this, &CVehicleTopViewWidget::HandleActBtnClicked);

    setter_tab_widget_ = new CSetterTabWidget(this);
    connect(setter_tab_widget_, &CSetterTabWidget::SigColorChanged,
            this, &CVehicleTopViewWidget::HandleActColorChanged);
    connect(setter_tab_widget_, &CSetterTabWidget::SigCheckChanged,
            this, &CVehicleTopViewWidget::HandleActCheckStatusChanged);

    main_splitter_ = new QSplitter(Qt::Horizontal);
    main_splitter_->addWidget(graphics_view_);
    main_splitter_->addWidget(setter_btn_);
    main_splitter_->addWidget(setter_tab_widget_);
    main_splitter_->setSizes({600, 15, 200});
    setCentralWidget(main_splitter_);
}

CVehicleTopViewWidget::~CVehicleTopViewWidget()
{
    SAFE_DELETE(graphics_view_);
    SAFE_DELETE(graphics_scene_);
    SAFE_DELETE(background_item_);
    SAFE_DELETE(setter_tab_widget_);
}

/** @brief 定义发送事件，当窗口关闭时，可以重新创建*/
void CVehicleTopViewWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit SigTopViewClosed();
}

void CVehicleTopViewWidget::CreateMenu()
{
    QMenu *menu = this->menuBar()->addMenu("&Zoom");
    QAction *act_zoom_x = menu->addAction("Zoom &X");
    act_zoom_x->setObjectName("ZoomX");
    act_zoom_x->setShortcut(QKeySequence("Ctrl+X"));
    QAction *act_zoom_y = menu->addAction("Zoom &Y");
    act_zoom_y->setObjectName("ZoomY");
    act_zoom_y->setShortcut(QKeySequence("Ctrl+Y"));
    QAction *act_zoom_xy = menu->addAction("Zoom &XY");
    act_zoom_xy->setObjectName("ZoomXY");
    act_zoom_xy->setShortcut(QKeySequence("Ctrl+Z"));
    connect(act_zoom_x, &QAction::triggered, this, &CVehicleTopViewWidget::HandleActZoom);
    connect(act_zoom_y, &QAction::triggered, this, &CVehicleTopViewWidget::HandleActZoom);
    connect(act_zoom_xy, &QAction::triggered, this, &CVehicleTopViewWidget::HandleActZoom);
}

/** @brief 响应颜色变化时，更新图像颜色*/
void CVehicleTopViewWidget::HandleActColorChanged(const QString &name, const QColor &color)
{
}

/** @brief 响应复选框选择事件，更改item是否显示的状态*/
void CVehicleTopViewWidget::HandleActCheckStatusChanged(const QString &name, bool status)
{
}

void CVehicleTopViewWidget::HandleActBtnClicked()
{
    if (setter_tab_widget_->isVisible())
    {
        setter_tab_widget_->setVisible(false);
        setter_btn_->setArrowType(Qt::RightArrow);
    }
    else
    {
        setter_tab_widget_->setVisible(true);
        setter_btn_->setArrowType(Qt::LeftArrow);
    }
}

void CVehicleTopViewWidget::HandleActZoom()
{
    if (sender()->objectName() == "ZoomX")
    {
        graphics_scene_->SetZoomMode(cZoomMode::ZoomX);
    }
    else if (sender()->objectName() == "ZoomY")
    {
        graphics_scene_->SetZoomMode(cZoomMode::ZoomY);
    }
    else if (sender()->objectName() == "ZoomXY")
    {
        graphics_scene_->SetZoomMode(cZoomMode::ZoomXY);
    }
}

void CVehicleTopViewWidget::AddSetterItem(const QString &name)
{
    setter_tab_widget_->AddCompomentName(name);
}