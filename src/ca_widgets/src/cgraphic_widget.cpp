#include "cgraphic_widget.h"
#include "cglobal_param.h"

CGraphicWidget::CGraphicWidget(QWidget *parent)
    : QMainWindow(parent)
{
    InitGraphic();
    CreateStatusBar();
    setAttribute(Qt::WA_DeleteOnClose);
}

CGraphicWidget::~CGraphicWidget()
{
    SAFE_DELETE(chart_);
    SAFE_DELETE(chart_view_cursor_);
    SAFE_DELETE(signal_list_widget_);
    SAFE_DELETE(threshold_box);
    SAFE_DELETE(threshold_setting_window_);
}

/** @brief 初始化曲线图的布局，添加信号显示listWidget、chartView及chart*/
void CGraphicWidget::InitGraphic()
{
    this->resize(600, 400);
    center_ = new QWidget(this);
    setCentralWidget(center_);
    main_layout_ = new QHBoxLayout(center_);
    main_layout_->setContentsMargins(0, 0, 0, 0);
    graphic_splitter_ = new QSplitter(Qt::Horizontal);

    signal_list_widget_ = new QListWidget(); /// init signal ListWidget to show signal info(include color and value)
    signal_list_widget_->setFrameShape(QListWidget::NoFrame);
    signal_list_widget_->setStyleSheet("QListWidget#signal_list_widget_{background-color:transparent}");
    signal_list_widget_->setFont(QFont("微软雅黑", 10));
    signal_list_widget_->setContextMenuPolicy(Qt::CustomContextMenu);
    signal_list_widget_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    signal_list_widget_->setDragEnabled(true);
    connect(signal_list_widget_, &QListWidget::customContextMenuRequested, this, &CGraphicWidget::HandleListWidgetMenuRequested);
    connect(signal_list_widget_, &QListWidget::itemClicked, this, &CGraphicWidget::ChangeAxisY);
    connect(signal_list_widget_, &QListWidget::itemDoubleClicked, this, &CGraphicWidget::ChangeSeriesColor);

    threshold_setting_window_ = new QWidget();
    threshold_setting_window_->setFixedSize(200, 60);
    QHBoxLayout *set_layout = new QHBoxLayout(threshold_setting_window_);
    threshold_box = new QDoubleSpinBox(); // ThresholdSettingFinished
    connect(threshold_box, &QDoubleSpinBox::editingFinished, this, &CGraphicWidget::ThresholdSettingFinished);
    set_layout->addWidget(threshold_box);

    signal_list_widget_menu_ = new QMenu(this);
    CreateActions();

    chart_ = new CChart();
    chart_->resize(500, 300);
    chart_view_cursor_ = new CChartViewCursor(chart_);

    graphic_splitter_->addWidget(signal_list_widget_);
    graphic_splitter_->addWidget(chart_view_cursor_);
    graphic_splitter_->setSizes({300, 200});
    main_layout_->addWidget(graphic_splitter_);
}

void CGraphicWidget::CreateActions()
{
    act_delete_item_ = new QAction(tr("Delete"), this);
    act_delete_item_->setShortcut(Qt::Key_Delete);
    connect(act_delete_item_, &QAction::triggered, this, &CGraphicWidget::HandleDeleteItem);

    re_chose_ = new QAction(tr("ReChoose"), this);
    connect(re_chose_, &QAction::triggered, this, &CGraphicWidget::ReCheck);

    chose_all_ = new QAction(tr("ChooseAll"), this);
    connect(chose_all_, &QAction::triggered, this, [this] { SetAllItem(true); });

    chose_none_ = new QAction(tr("ChooseNone"), this);
    connect(chose_none_, &QAction::triggered, this, [this] { SetAllItem(false); });

    onlye_chose_ = new QAction(tr("ChooseOnly"), this);
    connect(onlye_chose_, &QAction::triggered, this, [this] {
        SetAllItem(false);
        ReCheck();
    });

    chart_window_switch_ = new QAction(tr("HideChartWindow"), this);
    connect(chart_window_switch_, &QAction::triggered, this, &CGraphicWidget::HideOrShowChartWindow);

    threshold_setting_ = new QAction(tr("ThresholdSetting"), this);

    connect(threshold_setting_, &QAction::triggered, this, &CGraphicWidget::ThresholdSetting);

    signal_list_widget_menu_->addAction(act_delete_item_);
    signal_list_widget_menu_->addSeparator();
    signal_list_widget_menu_->addAction(chose_all_);
    signal_list_widget_menu_->addSeparator();
    signal_list_widget_menu_->addAction(chose_none_);
    signal_list_widget_menu_->addSeparator();
    signal_list_widget_menu_->addAction(re_chose_);
    signal_list_widget_menu_->addSeparator();
    signal_list_widget_menu_->addAction(onlye_chose_);
    signal_list_widget_menu_->addSeparator();
    signal_list_widget_menu_->addAction(chart_window_switch_);
    signal_list_widget_menu_->addSeparator();
    signal_list_widget_menu_->addAction(threshold_setting_);
}

/** @brief 当添加信号曲线时响应，添加信号曲线及时间戳*/
void CGraphicWidget::AddSignals(const QString &name)
{

    if (signal_names_.indexOf(name) == -1)
    {
        /// add signal series
        KLine line;
        line.sensorName = name;
        y_min_ = 0;
        y_max_ = 0;
        line.series = new QLineSeries(this);
        line.series->setObjectName(name);
        line.axisY = new QValueAxis(this);
        chart_->addSeries(line.series);

        chart_->axes(Qt::Horizontal)[0]->setRange(0, x_max_ + 1);
        line.axisY->setRange(y_min_, y_max_ + 1);
        line.yMin = y_min_;
        line.yMax = y_max_;
        line.axisY->setLabelFormat("%.1f");
        line.axisY->setGridLinePen(QPen(Qt::darkGreen, 0.5, Qt::DashLine));
        line.axisY->setTickCount(20);
        line.axisY->setLinePen(QPen(line.series->color()));
        chart_->addAxis(line.axisY, Qt::AlignLeft);
        for (auto axe : chart_->axes(Qt::Vertical))
        {
            axe->setVisible(false);
        }
        line.axisY->setVisible(true);

        line.series->attachAxis(chart_->axes(Qt::Horizontal)[0]);
        line.series->attachAxis(chart_->axes(Qt::Vertical).back());
        signal_names_.append(name);
        AddListWidgetItem(name.section(".", -1, -1), line.series);
        series_map_.insert(signal_list_widget_->count() - 1, line);
    }
    else
    {
        signal_list_widget_->setCurrentRow(signal_names_.indexOf(name));
    }
}
/** @brief 添加信号的listWidgetItem控件*/
void CGraphicWidget::AddListWidgetItem(const QString &name, const QLineSeries *line)
{
    /// add listWidgetItem to manage color and checkbox
    QFont font("Times New Roma", 10);
    QListWidgetItem *item = new QListWidgetItem(signal_list_widget_);
    QSplitter *signalSplitter = new QSplitter(Qt::Horizontal);

    KSignal signal;
    /// add checkbox
    signal.checkBox = new QCheckBox(this);
    signal.checkBox->setFont(font);
    signal.checkBox->setChecked(true);
    signal.checkBox->setMaximumWidth(20);
    connect(signal.checkBox, &QCheckBox::stateChanged, this, &CGraphicWidget::HideOrShowSeries);
    /// add labels
    signal.colorLabel = new QLabel("");
    signal.colorLabel->setFont(font);
    signal.colorLabel->setStyleSheet(QString("background-color:rgb(%1,%2,%3)").arg(line->color().red()).arg(line->color().green()).arg(line->color().blue()));
    signal.colorLabel->setMaximumWidth(10);
    signal.colorLabel->setMinimumWidth(10);
    signal.colorLabel->setAlignment(Qt::AlignLeft);
    signal.signalName = new QLabel(name);
    signal.signalName->setFont(font);
    signal.signalVal = new QLabel("");
    signal.signalVal->setFont(font);
    signal.signalVal->setAlignment(Qt::AlignLeft);

    signalSplitter->addWidget(signal.checkBox);
    signalSplitter->addWidget(signal.colorLabel);
    signalSplitter->addWidget(signal.signalName);
    signalSplitter->addWidget(signal.signalVal);
    signal_list_widget_->addItem(item);
    signal_list_widget_->setItemWidget(item, signalSplitter);
    // 用作后面的记忆功能模块
    item->setData(Qt::UserRole, line->objectName());
}
/** @brief 当signal被点击时，更改chart坐标轴为当前信号变量的值范围*/
void CGraphicWidget::ChangeAxisY(QListWidgetItem *item)
{
    int row = signal_list_widget_->row(item);
    for (auto i : chart_->axes(Qt::Vertical))
    {
        i->setVisible(false);
    }
    series_map_[row].axisY->setVisible(true);
}
/** @brief checkbox更换选择模式的时候，chart图像显示*/
void CGraphicWidget::HideOrShowSeries()
{
    for (int i = 0; i < signal_list_widget_->count(); ++i)
    {
        QWidget *w = signal_list_widget_->itemWidget(signal_list_widget_->item(i));
        QCheckBox *cb = w->findChild<QCheckBox *>();
        if (!cb->checkState())
        {
            if (chart_->series().indexOf(series_map_.value(i).series) != -1)
            {
                chart_->removeSeries(series_map_.value(i).series);
                chart_->removeAxis(series_map_.value(i).axisY);
            }
        }
        else
        {
            if (chart_->series().indexOf(series_map_.value(i).series) == -1)
            {
                chart_->addSeries(series_map_.value(i).series);
                chart_->addAxis(series_map_.value(i).axisY, Qt::AlignLeft);
                for (auto axe : chart_->axes(Qt::Vertical))
                {
                    axe->setVisible(false);
                }
                series_map_.value(i).axisY->setVisible(true);

                series_map_.value(i).series->attachAxis(chart_->axes(Qt::Horizontal)[0]);
                series_map_.value(i).series->attachAxis(chart_->axes(Qt::Vertical).back());
            }
        }
    }
}

/** @brief 删除选中的参数组件*/
void CGraphicWidget::HandleDeleteItem()
{
    std::mutex mtx;
    mtx.lock();
    QList<QListWidgetItem *> items = signal_list_widget_->selectedItems();
    if (items.count() > 0)
    {
        for (QListWidgetItem *i : items)
        {
            int rowVal = signal_list_widget_->row(i);
            if (series_map_.find(rowVal) != series_map_.end())
            {
                if (chart_->series().indexOf(series_map_.value(rowVal).series) != -1)
                {
                    chart_->removeSeries(series_map_.value(rowVal).series);
                    chart_->removeAxis(series_map_.value(rowVal).axisY);
                }

                emit DeletechartSignal(signal_names_[rowVal]);
                emit DeleteSignalGraphicSignal(signal_names_[rowVal]);
                series_map_.remove(rowVal);
                signal_names_.removeAt(rowVal);
            }
            QListWidgetItem *w = signal_list_widget_->takeItem(rowVal);
            delete w;
            /// update series_map_ index
            QMap<int, KLine> tempMap_;
            int index = 0;
            std::lock_guard<std::mutex> lg(delete_data_Lock_);
            name_to_row_num_.clear();
            for (auto var = series_map_.begin(); var != series_map_.end(); ++var)
            {
                tempMap_.insert(index, var.value());
                name_to_row_num_[var.value().sensorName] = index;
                ++index;
            }
            series_map_ = tempMap_;
        }
    }
    if (signal_list_widget_->count() > 0)
    {
        QListWidgetItem *current_item = signal_list_widget_->item(0);
        ChangeAxisY(current_item);
    }
    mtx.unlock();
}

void CGraphicWidget::HandleDeleteAllItem()
{
}

/** @brief 双击listWidget时，进入颜色选择栏更改颜色*/
void CGraphicWidget::ChangeSeriesColor(QListWidgetItem *item)
{
    QColorDialog colorDialog(this);
    colorDialog.setOption((QColorDialog::DontUseNativeDialog));
    if (colorDialog.exec() == QDialog::Accepted)
    {
        QColor color = colorDialog.currentColor();
        QString colorCss = QString("background-color:rgb(%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue());
        auto currentWidget = signal_list_widget_->itemWidget(item);
        auto colorLabel = currentWidget->findChildren<QLabel *>()[2];
        colorLabel->setStyleSheet(colorCss);
        QLineSeries *series = series_map_[signal_list_widget_->row(item)].series;
        series->setPen(QPen(color));
        series_map_[signal_list_widget_->row(item)].axisY->setLinePen(QPen(series->color()));
    }
    //    delete &colorDialog;
}

/** @brief 当选择多线模式的时候，将存在的所有曲线进行平行化；当为否的时候还原*/
void CGraphicWidget::ChangeGraphicModel(bool status)
{
    int total = signal_list_widget_->count();
    for (int i = 0; i < total; ++i)
    {
        if (status)
        {
            double yMax = series_map_[i].yMax + i * (series_map_[i].yMax - series_map_[i].yMin);
            double yMin = series_map_[i].yMin - (total - i - 1) * (series_map_[i].yMax - series_map_[i].yMin);
            series_map_[i].axisY->setRange(yMin, yMax);
        }
        else
        {
            series_map_[i].axisY->setRange(series_map_[i].yMin, series_map_[i].yMax);
        }
    }
}

/** @brief 当有游标线的时候，移动游标线，更新其信号数据*/
void CGraphicWidget::AppendCurrentPoint(QString name, double value, double time)
{
    if (signal_names_.indexOf(name) == -1)
        return;
    for (auto rowId : series_map_.keys())
    {
        if (series_map_[rowId].series->objectName() == name)
        {
            QPointF pointF;
            pointF.setX(time);
            pointF.setY(value);
            row_time_val_map_[rowId][time] = value;
            UpdateSeries(name, pointF, rowId);
            if (FLAGS_v_online)
                UpdataListData(rowId, pointF.y());
        }
    }
}

/** @brief 更新显示的曲线，以移动坐标轴的方式  **/
void CGraphicWidget::UpdateSeries(const QString &name, QPointF pointF, const int &rowId)
{
    if (series_map_.contains(rowId))
    {
        QList<QPointF> oldPointF;
        oldPointF = series_map_[rowId].series->points();
        if (FLAGS_v_online && oldPointF.size() > 220)
        {
            oldPointF.erase(oldPointF.begin());
        }
        oldPointF.append(pointF);
        if (series_map_.contains(rowId))
            series_map_[rowId].series->replace(oldPointF);
        ChangeChartAxisX(pointF.x());
        ChangeChartAxisY(name, pointF.y(), rowId);
    }
}
/** @brief 更改横坐标显示的范围，以达到移动曲线的目的 **/
void CGraphicWidget::ChangeChartAxisX(const double &time)
{
    if (chart_ == nullptr)
        return;
    if (time > max_axis_x_ && time > 4)
    {
        max_axis_x_ = time + 3;
        chart_->axisX()->setRange(time - 4, max_axis_x_);
        QValueAxis *factor_axisX = static_cast<QValueAxis *>(chart_->axes(Qt::Horizontal)[0]);
    }
}

/** @brief 更改纵坐标显示的范围，以达到移动曲线的目的 **/
void CGraphicWidget::ChangeChartAxisY(const QString &name, const double &value, const int &rowId)
{
    if (series_map_[rowId].yMin > value)
    {
        series_map_[rowId].axisY->setRange(value, series_map_[rowId].yMax);
        series_map_[rowId].yMin = value;
    }
    else if (series_map_[rowId].yMax < value)
    {
        series_map_[rowId].axisY->setRange(series_map_[rowId].yMin, value);
        series_map_[rowId].yMax = value;
    }
}

/** @brief 从当前帧获取数据，更新各个信号行显示的数据 **/
void CGraphicWidget::UpdataListData(int row, double value)
{
    if (signal_list_widget_->count() > row)
    {
        auto currentWidget = signal_list_widget_->itemWidget(signal_list_widget_->item(row));
        currentWidget->findChildren<QLabel *>()[0]->setText(QString::number(value, 10, 9));
        QLabel *valueLable = currentWidget->findChildren<QLabel *>()[0];
        if (FLAGS_v_online && threshold_value_map_.contains(valueLable->text()))
        {
            if (threshold_value_map_[valueLable->text()] < value)
            {
                valueLable->setStyleSheet("QLabel{background-color:rgb(255,0,0);}");
            }
            else
            {
                valueLable->setStyleSheet("QLabel{background-color:transparent;}");
            }
        }
    }
}

void CGraphicWidget::ReCheck()
{
    QList<QListWidgetItem *> items = signal_list_widget_->selectedItems();
    if (items.size() <= 0)
        return;

    for (QListWidgetItem *item : items)
    {
        SetItemChecked(item);
    }
}

void CGraphicWidget::SetAllItem(bool is_checked)
{
    for (int i = 0; i < signal_list_widget_->count(); i++)
    {
        SetItemChecked(signal_list_widget_->item(i), is_checked);
    }
}

void CGraphicWidget::SetItemChecked(QListWidgetItem *item, int is_checked)
{
    QSplitter *splitter = (QSplitter *)signal_list_widget_->itemWidget(item);
    if (nullptr == splitter)
        return;

    QCheckBox *check_box = (QCheckBox *)splitter->widget(0);
    if (nullptr == check_box)
        return;

    if (is_checked == 0 || is_checked == 1)
        check_box->setChecked(is_checked);
    else
        check_box->setChecked(!check_box->isChecked());
}

void CGraphicWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    emit SigGraphicClose();
}

void CGraphicWidget::ThresholdSettingFinished()
{
    double value = threshold_box->value();
    if (value != 0)
    {
        threshold_value_map_.insert(threshold_setting_window_->windowTitle(), value);
        signal_list_widget_->currentItem()->setToolTip(QString::number(value));
    }
    threshold_setting_window_->hide();
}

void CGraphicWidget::HideOrShowChartWindow()
{
    if (chart_view_cursor_->isVisible())
    {
        chart_view_cursor_->setVisible(false);
        chart_window_switch_->setText(tr("ShowChartWindow"));
    }
    else
    {
        chart_view_cursor_->setVisible(true);
        chart_window_switch_->setText(tr("HideChartWindow"));
    }
}

void CGraphicWidget::ThresholdSetting()
{
    if (!FLAGS_v_online)
        return;
    QList<QListWidgetItem *> select_items = signal_list_widget_->selectedItems();
    if (select_items.count() == 1)
    {
        QListWidgetItem *item = select_items[0];
        auto currentWidget = signal_list_widget_->itemWidget(item);
        auto name_label = currentWidget->findChildren<QLabel *>()[1];
        threshold_setting_window_->setWindowTitle(name_label->text());
        threshold_box->setValue(threshold_value_map_[name_label->text()]);
        threshold_setting_window_->show();
    }
    else
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("请选择一个信号进行阈值设置"));
    }
}
