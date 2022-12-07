#ifndef CVEHICLE_TOPVIEW_WIDGET_H
#define CVEHICLE_TOPVIEW_WIDGET_H

#include <QtWidgets>
#include <QDebug>
#include "cgraphics_view.h"
#include "cgraphics_scene.h"
#include "cvehicle_background_item.h"
#include "csetter_tab_widget.h"
#include "cglobal_param.h"
#include "cobject_item.h"
#include "cline_item.h"
#include "cpoint_set_item.h"
#include "ctransfer.h"

using namespace cav;
template <class T>
class CDataHash
{
public:
    CDataHash() {}
    ~CDataHash() {}
    QHash<QString, QVector<T>> hash_;
};

class CVehicleTopViewWidget : public QMainWindow
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event) override;

public:
    explicit CVehicleTopViewWidget(QWidget *parent = nullptr);
    ~CVehicleTopViewWidget() override;

    void AddSetterItem(const QString &name);
    void UpdateInsData(const CPointData &ins_data);
    void UpdateItemData(const QString &name, double delta_time,
                        const QVector<CObjectData> &data, const QColor &color);
    void UpdateItemData(const QString &name, double delta_time,
                        const QVector<CLineData> &data, const QColor &color);
    void UpdateItemData(const QString &name, double delta_time,
                        const QVector<CPointData> &data, const QColor &color);

    template <class T>
    void AppendValue(const QString &name, const T &data);

    template <class T>
    CDataHash<T> *GetDataPtr();

public:
    CGraphicsView *graphics_view_ = nullptr;
    CGraphicsScene *graphics_scene_ = nullptr;
    CVehicleBackGroundItem *background_item_ = nullptr;
    CSetterTabWidget *setter_tab_widget_ = nullptr;

private:
    void CreateMenu();
    void HandleActBtnClicked();
    void HandleActZoom();
    void HandleActColorChanged(const QString &name, const QColor &color);
    void HandleActCheckStatusChanged(const QString &name, bool status);
    void TransferMap(QVector<CLineData> &lines);
    template <class T>
    void HandleActSetStatus(const QString &name, const T &status);
    template <class T>
    void SetStatus(const QString &name, const QColor &color);
    template <class T>
    void SetStatus(const QString &name, bool status);

private:
    QSplitter *main_splitter_ = nullptr;
    QToolButton *setter_btn_ = nullptr;
    QMap<QString, void *> data_ptr_hash_;
    CPointData ins_data_;
    CTransfer transfer_;

signals:
    void SigVehicleTopViewClosed();
};

template <class T>
void CVehicleTopViewWidget::AppendValue(const QString &name, const T &data)
{
    QString class_name = TOQSTR(typeid(T).name());
    if (!data_ptr_hash_.contains(class_name))
    {
        CDataHash<T> *hash = new CDataHash<T>();
        data_ptr_hash_[class_name] = hash;
    }
    CDataHash<T> *hash_ptr = static_cast<CDataHash<T> *>(data_ptr_hash_[class_name]);
    if (hash_ptr)
        hash_ptr->hash_[name].push_back(data);
}

template <class T>
CDataHash<T> *CVehicleTopViewWidget::GetDataPtr()
{
    QString class_name = TOQSTR(typeid(T).name());
    if (data_ptr_hash_.contains(class_name))
    {
        return static_cast<CDataHash<T> *>(data_ptr_hash_[class_name]);
    }
    return nullptr;
}

template <class T>
void CVehicleTopViewWidget::HandleActSetStatus(const QString &name, const T &status)
{
    if (name.contains("FusionProto.FusObjects") || name.contains("CameraProto.CamObjects") ||
        name.contains("RadarProto.RadarObjects") || name.contains("LidarObjectProto.Objects") ||
        name.contains("prediction.RNPObjectOut-obj"))
    {
        SetStatus<CObjectItem>(name, status);
    }
    else if (name.contains("prediction.RNPObjectDebugOut") ||
             name.contains("prediction.RNPObjectOut-history_trajectory") ||
             name.contains("prediction.RNPObjectOut-predict_trajectory") ||
             name.contains("CameraProto.CamLines"))
    {
        SetStatus<CLineItem>(name, status);
    }
    else if (name.contains("FusionProto.FusFreeSpace") || name.contains("FusionProto.RadarFreeSpace") ||
             name.contains("FusionProto.VisionFreeSpace") || name.contains("CameraProto.CamFreeSpace") ||
             name.contains("StaticHDMapInfo") || name.contains("StaticIDMapInfo") || name.contains("RNPEnvOut"))
    {
        SetStatus<CPointSetItem>(name, status);
    }
    graphics_scene_->update();
}

template <class T>
void CVehicleTopViewWidget::SetStatus(const QString &name, const QColor &color)
{
    auto hash = GetDataPtr<T *>();
    if (hash)
    {
        for (auto i : hash->hash_[name])
        {
            i->SetColor(color);
        }
    }
}

template <class T>
void CVehicleTopViewWidget::SetStatus(const QString &name, bool status)
{
    auto hash = GetDataPtr<T *>();
    if (hash)
    {
        for (auto i : hash->hash_[name])
        {
            i->setVisible(status);
        }
    }
}
#endif // CVEHICLE_TOPVIEW_WIDGET_H
