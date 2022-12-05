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
    setter_btn_->setArrowType(Qt::RightArrow);
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
    main_splitter_->setSizes({800, 15, 200});
    setCentralWidget(main_splitter_);
}

CVehicleTopViewWidget::~CVehicleTopViewWidget()
{
    SAFE_DELETE(graphics_view_);
    SAFE_DELETE(graphics_scene_);
    SAFE_DELETE(setter_tab_widget_);
    qDeleteAll(data_ptr_hash_);
}

/** @brief 定义发送事件，当窗口关闭时，可以重新创建*/
void CVehicleTopViewWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit SigVehicleTopViewClosed();
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
    if (name.contains("FusionProto.FusObjects") || name.contains("CameraProto.CamObjects") ||
        name.contains("RadarProto.RadarObjects"))
    {
        auto hash = GetDataPtr<CObjectItem *>();
        if (hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->SetColor(color);
            }
        }
    }
    else if (name.contains("CameraProto.CamLines"))
    {
        auto hash = GetDataPtr<CLineItem *>();
        if (hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->SetColor(color);
            }
        }
    }
    else if (name.contains("FusionProto.FusFreeSpace") || name.contains("FusionProto.RadarFreeSpace") ||
             name.contains("FusionProto.VisionFreeSpace") || name.contains("CameraProto.CamFreeSpace"))
    {
        auto hash = GetDataPtr<CPointSetItem *>();
        if (hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->SetColor(color);
            }
        }
    }
    graphics_scene_->update();
}

/** @brief 响应复选框选择事件，更改item是否显示的状态*/
void CVehicleTopViewWidget::HandleActCheckStatusChanged(const QString &name, bool status)
{
    if (name.contains("FusionProto.FusObjects") || name.contains("CameraProto.CamObjects") ||
        name.contains("RadarProto.RadarObjects"))
    {
        auto hash = GetDataPtr<CObjectItem *>();
        if (hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->setVisible(status);
            }
        }
    }
    else if (name.contains("CameraProto.CamLines"))
    {
        auto hash = GetDataPtr<CLineItem *>();
        if (hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->setVisible(status);
            }
        }
    }
    else if (name.contains("FusionProto.FusFreeSpace") || name.contains("FusionProto.RadarFreeSpace") ||
             name.contains("FusionProto.VisionFreeSpace") || name.contains("CameraProto.CamFreeSpace"))
    {
        auto hash = GetDataPtr<CPointSetItem *>();
        if (hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->setVisible(status);
            }
        }
    }
    graphics_scene_->update();
}

void CVehicleTopViewWidget::HandleActBtnClicked()
{
    if (setter_tab_widget_->isVisible())
    {
        setter_tab_widget_->setVisible(false);
        setter_btn_->setArrowType(Qt::LeftArrow);
    }
    else
    {
        setter_tab_widget_->setVisible(true);
        setter_btn_->setArrowType(Qt::RightArrow);
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

void CVehicleTopViewWidget::UpdateItemData(const QString &name, double delta_time,
                                           const QVector<CObjectData> &data, const QColor &color)
{
    if (delta_time * 1000.0 <= 250)
    {
        QMap<int, CObjectData> object_track_ids;
        for (auto i : data)
        {
            object_track_ids.insert(i.track_id_, i);
        }
        auto objs_hash = GetDataPtr<CObjectItem *>();
        if (objs_hash && objs_hash->hash_[name].size() > 0)
        {
            auto &items = objs_hash->hash_[name];

            for (auto iter = items.begin(); iter != items.end();)
            {
                int track_id = (*iter)->GetTrackId();
                if (object_track_ids.keys().contains(track_id))
                {
                    (*iter)->SetData(object_track_ids[track_id]);
                    object_track_ids.remove(track_id);
                    ++iter;
                }
                else
                {
                    SAFE_DELETE(*iter);
                    items.erase(iter);
                }
            }
            items.shrink_to_fit();
        }
        for (auto i : object_track_ids.values())
        {
            CObjectItem *item = new CObjectItem(background_item_);
            item->SetColor(color);
            item->SetName(name);
            item->SetData(i);
            AppendValue<CObjectItem *>(name, item);
        }
        graphics_scene_->update();
    }
}

void CVehicleTopViewWidget::UpdateItemData(const QString &name, double delta_time,
                                           const QVector<CLineData> &data, const QColor &color)
{
    if (delta_time * 1000.0 <= 250)
    {
        auto lines_hash = GetDataPtr<CLineItem *>();
        if (lines_hash && lines_hash->hash_[name].size() > 0)
        {
            auto &items = lines_hash->hash_[name];
            qDeleteAll(items);
            items.clear();
        }
        for (auto i : data)
        {
            CLineItem *item = new CLineItem(background_item_);
            item->SetColor(color);
            item->SetData(i);
            AppendValue<CLineItem *>(name, item);
        }
        graphics_scene_->update();
    }
}

void CVehicleTopViewWidget::UpdateItemData(const QString &name, double delta_time,
                                           const QVector<CPointData> &data, const QColor &color)
{
    if (delta_time * 1000.0 <= 250)
    {
        QVector<CPointData> points;
        for (auto point : data)
        {
            if (point.x_ == 0.0 && point.y_ == 0.0 && point.z_ == 0.0)
                continue;
            points.push_back(point);
        }
        auto points_hash = GetDataPtr<CPointSetItem *>();
        if (points_hash && points_hash->hash_[name].size() > 0)
        {
            auto &items = points_hash->hash_[name];
            qDeleteAll(items);
            items.clear();
        }
        CPointSetItem *item = new CPointSetItem(background_item_);
        item->SetData(points);
        item->SetColor(color);
        AppendValue<CPointSetItem *>(name, item);
        graphics_scene_->update();
    }
}