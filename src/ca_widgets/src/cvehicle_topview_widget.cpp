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
        name.contains("RadarProto.RadarObjects") || name.contains("LidarObjectProto.Objects") ||
        name.contains("prediction.RNPObjectOut-obj"))
    {
        auto hash = GetDataPtr<CObjectItem *>();
        if(hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->SetColor(color);
            }
        }
    }
    else if (name.contains("Detections") || name.contains("FreeSpace") || name.contains("LidarFreeSpaceProto.FreeSpaceData"))
    {
        if (!point_item_map_.isEmpty())
        {
            for (auto i : point_item_map_[name])
            {
                i->SetColor(color);
            }
        }
    }
    else if (name.contains("StaticHDMapInfo") || name.contains("StaticIDMapInfo") || name.contains("RNPEnvOut"))
    {
        auto hash = GetDataPtr<CPointItem *>();
        if (hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->SetColor(color);
            }
        }
    }
    else if (name.contains("prediction.RNPObjectDebugOut") ||
             name.contains("prediction.RNPObjectOut-history_trajectory") ||
             name.contains("prediction.RNPObjectOut-predict_trajectory"))
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
    graphics_scene_->update();
}

/** @brief 响应复选框选择事件，更改item是否显示的状态*/
void CVehicleTopViewWidget::HandleActCheckStatusChanged(const QString &name, bool status)
{
    if (name.contains("FusionProto.FusObjects") || name.contains("CameraProto.CamObjects") ||
        name.contains("RadarProto.RadarObjects") || name.contains("LidarObjectProto.Objects") ||
        name.contains("prediction.RNPObjectOut-obj"))
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
    else if (name.contains("Detections") || name.contains("FreeSpace") || name.contains("LidarFreeSpaceProto.FreeSpaceData"))
    {
        if (!point_item_map_.isEmpty())
        {
            for (auto i : point_item_map_[name])
            {
                i->setVisible(status);
            }
        }
    }
    else if (name.contains("StaticHDMapInfo") || name.contains("StaticIDMapInfo"))
    {
        auto hash = GetDataPtr<CPointItem *>();
        if (hash)
        {
            for (auto i : hash->hash_[name])
            {
                i->setVisible(status);
            }
        }
    }
    else if (name.contains("RNPEnvOut") ||
             name.contains("prediction.RNPObjectDebugOut") ||
             name.contains("prediction.RNPObjectOut-history_trajectory") ||
             name.contains("prediction.RNPObjectOut-predict_trajectory"))
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

void CVehicleTopViewWidget::UpdateObjectItemData(const QString &name, double delta_time, const QVector<CObjectData> &data, const QColor &color)
{
    if (delta_time*1000 <= 250)
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

void CVehicleTopViewWidget::UpdateMapLine(const QString &name, const QVector<CMapLine> &lines, const QColor &color)
{
    if(!IsValidIns(ins_))
    {
        qDebug() << "InsData not valid!";
        return;
    }

    typedef QMap<unsigned long, QVector<CPointData>> typePoints;
    QMap<unsigned long, int> marking_type;
    QMap<unsigned long, int> line_type;
    QVector<typePoints> mapdata;
    CPointData tmp_point;
    double heading = (360.0 - ins_.heading_);

    QVector<CPointData> tmp_line;
    for(CMapLine line : lines)
    {
        typePoints tPoints;
        for (auto &point : line.points_)
        {
            transfer_.PointTransForm(point.longitude_, point.latitude_,
                                   ins_.gnss_.longitude_, ins_.gnss_.latitude_, heading,
                                   tmp_point.x_, tmp_point.y_);
            tPoints[line.index_].push_back(tmp_point);
        }

        if(tPoints.size() <= 0)
            continue;

        marking_type[line.index_] = line.marking_type_;
        line_type[line.index_] = line.type_;
        mapdata.push_back(tPoints);
    }

    CDataHash<CPointItem *> *point_hash = GetDataPtr<CPointItem *>();
    if(nullptr != point_hash && point_hash->hash_.contains(name))
    {
        for(CPointItem *item : point_hash->hash_[name])
        {
            SAFE_DELETE(item);
        }

        point_hash->hash_[name].clear();
    }

    for (auto &tPoint : mapdata)
    {
        for (auto &type : tPoint.keys())
        {
            CPointItem *item = new CPointItem(background_item_);
            item->SetColor(color);
            item->SetLineType(line_type[type]);
            item->SetPointType(marking_type[type]);
            item->SetPointName(QString::number(type));
            item->SetData(tPoint[type]);
            item->map_switch_ = true;
            item->is_draw_line_node_ = true;
            AppendValue<CPointItem *>(name, item);
        }
    }
    graphics_scene_->update();
}

bool CVehicleTopViewWidget::IsValidIns(const CMapInsData &ins)
{
    return ((ins.gnss_.latitude_ != 0.00000 || ins.gnss_.longitude_ != 0.0000)
            || ins.heading_ != 0.000);
}

void CVehicleTopViewWidget::UpdateInsData(const CMapInsData &ins_data)
{
    if(!IsValidIns(ins_data))
        return;

    ins_ = ins_data;
}

void CVehicleTopViewWidget::UpdatePointItemData(const QString &name, double time, const QVector<CPointData> &data, const QColor &color)
{
    QVector<CPointData> points;
    for (auto point : data)
    {
        if (point.x_ == 0.0 && point.y_ == 0.0 && point.z_ == 0.0)
            continue;

        points.push_back(point);
    }
    QVector<CPointItem *> &items = point_item_map_[name];
    qDeleteAll(items);
    items.clear();
    CPointItem *item = new CPointItem(background_item_);
    item->connect_swtich_ = false;
    item->SetColor(color);
    item->SetData(points);
    items.append(item);
}

void CVehicleTopViewWidget::UpdateLineItemData(const QString &name, double delta_time, const QVector<CLineData> &data, const QColor &color)
{
    if (delta_time*1000 <= 250)
    {
        auto objs_hash = GetDataPtr<CLineItem *>();
        if (objs_hash && objs_hash->hash_[name].size() > 0)
        {
            auto &items = objs_hash->hash_[name];
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
