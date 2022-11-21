#ifndef CVEHICLE_TOPVIEW_WIDGET_H
#define CVEHICLE_TOPVIEW_WIDGET_H

#include <QtWidgets>
#include "cgraphics_view.h"
#include "cgraphics_scene.h"
#include "cvehicle_background_item.h"
#include "csetter_tab_widget.h"
#include "cglobal_param.h"

class CVehicleTopViewWidget : public QMainWindow
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event) override;

public:
    explicit CVehicleTopViewWidget(QWidget *parent = nullptr);
    ~CVehicleTopViewWidget() override;

    void AddSetterItem(const QString &name);

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

signals:
    void SigTopViewClosed();
};

#endif // CVEHICLE_TOPVIEW_WIDGET_H