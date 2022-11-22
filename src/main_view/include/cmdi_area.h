#ifndef CMDI_AREA_H
#define CMDI_AREA_H

#include <QtWidgets>
#include <QMdiArea>
#include <iostream>
#include "cdata_center.h"
#include "cdata_scheduler.h"

class CMdiArea : public QMdiArea
{
    Q_OBJECT
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit CMdiArea(QWidget *parent = nullptr);
    ~CMdiArea();

private:
    /** @brief 创建主界面的右键菜单*/
    void CreateMenus();
    void CreateActions();

    void HandleActAddCamera();
    void HandleActAddVehicleTopView();

private:
    QMenu *menu_;

    QAction *act_add_camera_;
    QAction *act_add_vehicle_topview_;
    CDataCenter *data_center_;
    CDataScheduler *data_scheduler_;
};

#endif // CMDI_AREA_H
