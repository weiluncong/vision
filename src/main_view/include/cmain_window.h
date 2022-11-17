#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QtWidgets>
#include <QDebug>
#include "cglobal_param.h"
#include "cmain_widget.h"
#include "cwindow_manager.h"

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = nullptr);
    ~CMainWindow();

private:
    /** @brief 创建主界面的action、menu和Bar*/
    void CreateActions();
    void CreateStatusBar();
    void CreateMenus();
    void CreateToolBar();

public:
    /** @brief 定义所有menu*/
    QMenu *recent_menu_;
    QMenu *view_menu_;
    QMenu *file_menu_;
    QMenu *tool_menu_;
    QMenu *help_menu_;
    /** @brief 定义所有action*/
    QAction *act_mode_;
    QAction *act_open_file_;
    QAction *act_switch_;
    QAction *act_hide_explorer_;
    QAction *act_add_cursor_;
    QAction *act_add_double_cursors_;
    QAction *act_help_;
    QAction *act_save_config_;
    QAction *act_total_record_;
    QAction *act_point_record_;
    QAction *act_once_point_record_;

    QAction *act_speed1_;
    QAction *act_speed2_;
    QAction *act_speed3_;
    QAction *act_speed4_;
    QAction *act_speed5_;
    QAction *act_speed6_;
    QAction *act_speed7_;
    QAction *act_speed8_;
    QAction *act_speed9_;

    /**@brief 定义控件*/
    CMainWidget *main_widget_ = nullptr;
    CWindowManager *window_manager_ = nullptr;
};

#endif // CMAINWINDOW_H