#include "cmain_window.h"

CMainWindow::CMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1200, 1000);
    this->setWindowIcon(QIcon(":/icon/ca.png"));
    this->setWindowTitle("CaVision");

    window_manager_ = new CWindowManager(this);

    QWidget *central_widget = new QWidget(this);
    main_widget_ = new CMainWidget(central_widget);
    QVBoxLayout *main_layout = new QVBoxLayout(central_widget);
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(main_widget_);

    setCentralWidget(central_widget);
    CreateActions();
    CreateMenu();
    CreateToolBar();
    CreateStatusBar();
}

CMainWindow::~CMainWindow()
{
    delete main_widget_;
    delete window_manager_;
}

/** @brief 定义actions*/
void CMainWindow::CreateActions()
{
    /** @brief 在线离线模式切换*/
    act_mode_ = new QAction(QIcon(":/icon/online.png"), tr("&Mode"), this);
    act_mode_->setStatusTip(tr("choose global mode"));
    act_mode_->setShortcut(QKeySequence("M"));
    act_mode_->setEnabled(true);
    connect(act_mode_, &QAction::triggered, window_manager_, &CWindowManager::HandleActModel);
    /** @brief 打开文件的指令*/
    act_open_file_ = new QAction(QIcon(":/icon/file.png"), tr("&open"), this);
    act_open_file_->setShortcut(QKeySequence("Ctrl+P"));
    act_open_file_->setStatusTip(tr("open a new dat file"));
    act_open_file_->setEnabled(false);
    connect(act_open_file_, &QAction::triggered, window_manager_, &CWindowManager::HandleActOpenFile);
    /** @brief 播放数据和暂停播放数据的指令*/
    act_switch_ = new QAction(QIcon(":/icon/start.png"), tr("&start"), this);
    act_switch_->setShortcut(QKeySequence("space"));
    act_switch_->setEnabled(true);
    act_switch_->setStatusTip(tr("start to play data"));
    connect(act_switch_, &QAction::triggered, window_manager_, &CWindowManager::HandleActSwitch);
    /** @brief 隐藏搜索栏的指令*/
    act_hide_explorer_ = new QAction(tr("&hide"), this);
    act_hide_explorer_->setShortcut(QKeySequence("Ctrl+return"));
    act_hide_explorer_->setStatusTip(tr("hide explorerBox"));
    /** @brief 选择图表放大轴方向的指令*/
    act_zoom_x_ = new QAction(QIcon(":/icon/x.png"), tr("&zoomX"), this);
    act_zoom_x_->setShortcut(QKeySequence("ctrl+x"));
    connect(act_zoom_x_, &QAction::triggered, window_manager_, &CWindowManager::HandleActZoomX);
    act_zoom_y_ = new QAction(QIcon(":/icon/Y.png"), tr("&zoomY"), this);
    act_zoom_y_->setShortcut(QKeySequence("ctrl+y"));
    connect(act_zoom_y_, &QAction::triggered, window_manager_, &CWindowManager::HandleActZoomY);
    act_zoom_xy_ = new QAction(QIcon(":/icon/XY.png"), tr("&zoom"), this);
    act_zoom_xy_->setShortcut(QKeySequence("ctrl+z"));
    connect(act_zoom_xy_, &QAction::triggered, window_manager_, &CWindowManager::HandleActZoomXY);
    /** @brief 添加游标指令*/
    act_add_cursor_ = new QAction(QIcon(":/icon/cursor.png"), tr("&cursor"), this);
    act_add_cursor_->setStatusTip("show the measurement cursor");
    act_add_cursor_->setShortcut(QKeySequence("C"));
    act_add_double_cursors_ = new QAction(QIcon(":/icon/cursors.png"), tr("&double cursors"), this);
    act_add_double_cursors_->setStatusTip("show the measurement cursors");
    act_add_double_cursors_->setShortcut(QKeySequence("V"));
    /** @brief 弹出help窗口*/
    act_help_ = new QAction(QIcon(":/icon/help.png"), tr("&help"), this);
    act_help_->setStatusTip("Tool Helper");
    /** @brief 保存当前控件的信息*/
    act_save_config_ = new QAction(this);
    act_save_config_->setShortcut(QKeySequence("Ctrl+S"));
    /** @brief slider按键倍速*/
    act_speed1_ = new QAction(tr("1"), this);
    act_speed1_->setShortcut(QKeySequence("Ctrl+1"));
    act_speed2_ = new QAction(tr("2"), this);
    act_speed2_->setShortcut(QKeySequence("Ctrl+2"));
    act_speed3_ = new QAction(tr("3"), this);
    act_speed3_->setShortcut(QKeySequence("Ctrl+3"));
    act_speed4_ = new QAction(tr("4"), this);
    act_speed4_->setShortcut(QKeySequence("Ctrl+4"));
    act_speed5_ = new QAction(tr("5"), this);
    act_speed5_->setShortcut(QKeySequence("Ctrl+5"));
    act_speed6_ = new QAction(tr("6"), this);
    act_speed6_->setShortcut(QKeySequence("Ctrl+6"));
    act_speed7_ = new QAction(tr("7"), this);
    act_speed7_->setShortcut(QKeySequence("Ctrl+7"));
    act_speed8_ = new QAction(tr("8"), this);
    act_speed8_->setShortcut(QKeySequence("Ctrl+8"));
    act_speed9_ = new QAction(tr("9"), this);
    act_speed9_->setShortcut(QKeySequence("Ctrl+9"));
    /** @brief 全量数据采集功能开关*/
    act_total_record_ = new QAction(QIcon(":/icon/record.png"), tr("&record"), this);
    act_total_record_->setShortcut(QKeySequence("Ctrl+R"));
    act_total_record_->setStatusTip(tr("data total record"));
    act_total_record_->setEnabled(true);
    connect(act_total_record_, &QAction::triggered, window_manager_, &CWindowManager::HandleActDataTotalRecord);
    /** @brief 打点数据记录功能开关*/
    act_point_record_ = new QAction(QIcon(":/icon/point_record_.png"), tr("&point record"), this);
    act_point_record_->setShortcut(QKeySequence("Ctrl+T"));
    act_point_record_->setStatusTip(tr("data point record"));
    act_point_record_->setEnabled(true);
    connect(act_point_record_, &QAction::triggered, window_manager_, &CWindowManager::HandleActDataPointRecord);
    /** @brief 打点记录事件触发*/
    act_once_point_record_ = new QAction(this);
    act_once_point_record_->setShortcut(QKeySequence("P"));
}

/** @brief 定义menu*/
void CMainWindow::CreateMenu()
{
    /** @brief 定义file_menu*/
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(act_open_file_);
    recent_menu_ = file_menu_->addMenu(tr("Recent..."));
    /** @brief 定义tool_menu*/
    tool_menu_ = menuBar()->addMenu(tr("&Tool"));
    tool_menu_->addAction(act_mode_);
    tool_menu_->addAction(act_switch_);
    tool_menu_->addAction(act_add_cursor_);
    tool_menu_->addAction(act_add_double_cursors_);
    QMenu *short_cut_menu = tool_menu_->addMenu(tr("&ShortCut"));
    short_cut_menu->addAction(act_hide_explorer_);
    short_cut_menu->addAction(act_zoom_x_);
    short_cut_menu->addAction(act_zoom_y_);
    short_cut_menu->addAction(act_zoom_xy_);
    short_cut_menu->addAction(act_speed1_);
    short_cut_menu->addAction(act_speed2_);
    short_cut_menu->addAction(act_speed3_);
    short_cut_menu->addAction(act_speed4_);
    short_cut_menu->addAction(act_speed5_);
    short_cut_menu->addAction(act_speed6_);
    short_cut_menu->addAction(act_speed7_);
    short_cut_menu->addAction(act_speed8_);
    short_cut_menu->addAction(act_speed9_);
    short_cut_menu->addAction(act_once_point_record_);

    view_menu_ = menuBar()->addMenu(tr("&View"));
    help_menu_ = menuBar()->addMenu(tr("&Help"));
    help_menu_->addAction(act_help_);
}

/** @brief 定义toolBar*/
void CMainWindow::CreateToolBar()
{
    QToolBar *tool_bar = new QToolBar(tr("Tool"));
    addToolBar(Qt::LeftToolBarArea, tool_bar);
    tool_bar->setOrientation(Qt::Vertical);
    tool_bar->addSeparator();
    tool_bar->addAction(act_mode_);
    tool_bar->addSeparator();
    tool_bar->addAction(act_open_file_);
    tool_bar->addAction(act_switch_);
    tool_bar->addSeparator();
    tool_bar->addAction(act_total_record_);
    tool_bar->addAction(act_point_record_);
    tool_bar->addSeparator();
    tool_bar->addAction(act_add_cursor_);
    tool_bar->addAction(act_add_double_cursors_);
}

/** @brief 定义statusBar*/
void CMainWindow::CreateStatusBar()
{
    statusBar()->showMessage(tr("version 1.0.0"));
}