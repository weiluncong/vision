#ifndef CVEHICLE_TOPVIEW_WIDGET_H
#define CVEHICLE_TOPVIEW_WIDGET_H

#include <QtWidgets>
#include "cgraphics_view.h"
#include "cgraphics_scene.h"
#include "cvehicle_background_item.h"
#include "csetter_tab_widget.h"
#include "cglobal_param.h"
#include "cobject_item.h"

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
    void UpdateObjectItemData(const QString &name, double delta_time,
                              const QVector<CObjectData> &data, const QColor &color);

    template <class T>
    void AppendValue(const QString &name, const T &data)
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
    CDataHash<T> *GetDataPtr()
    {
        QString class_name = TOQSTR(typeid(T).name());
        if (data_ptr_hash_.contains(class_name))
        {
            return static_cast<CDataHash<T> *>(data_ptr_hash_[class_name]);
        }
        return nullptr;
    }

public:
    CGraphicsView *graphics_view_ = nullptr;
    CGraphicsScene *graphics_scene_ = nullptr;
    CVehicleBackGroundItem *background_item_ = nullptr;
    CSetterTabWidget *setter_tab_widget_ = nullptr;

private:
    void CreateMenu();

private:
    void HandleActBtnClicked();
    void HandleActZoom();
    void HandleActColorChanged(const QString &name, const QColor &color);
    void HandleActCheckStatusChanged(const QString &name, bool status);

private:
    QSplitter *main_splitter_ = nullptr;
    QToolButton *setter_btn_ = nullptr;
    QMap<QString, void *> data_ptr_hash_;

signals:
    void SigVehicleTopViewClosed();
};

#endif // CVEHICLE_TOPVIEW_WIDGET_H