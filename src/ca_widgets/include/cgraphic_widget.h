#ifndef KGRAPHIC_WIDGET_H
#define KGRAPHIC_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QListWidget>
#include <QMainWindow>
#include <QLayout>
#include <QSplitter>
#include <mutex>
#include "cchart.h"
#include "cchart_view_cursor.h"


typedef struct
{
    QCheckBox *checkBox = nullptr;
    QLabel *colorLabel = nullptr;
    QLabel *signalName = nullptr;
    QLabel *signalVal = nullptr;
} KSignal;

typedef struct
{
    QLineSeries *series = nullptr;
    QValueAxis *axisY = nullptr;
    double yMax = 0;
    double yMin = 0;
    QString sensorName;
} KLine;

class CGraphicWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit CGraphicWidget(QWidget *parent = nullptr);
    virtual ~CGraphicWidget();

public:
    void InitGraphic();
    void CreateStatusBar() { statusBar()->showMessage(tr("Ready")); }


    void AddSignals(const QString &name);
    void ChangeGraphicModel(bool status);
    void AddListWidgetItem(const QString &name, const QLineSeries *line);
    void HideOrShowSeries();
    void HandleDeleteItem();
    void HandleDeleteAllItem();
    void HandleListWidgetMenuRequested() { signal_list_widget_menu_->exec(QCursor::pos()); }
    void ChangeAxisY(QListWidgetItem *item);
    void ChangeSeriesColor(QListWidgetItem *item);
    void UpdateSignalVal(const double &valX);
    void AppendCurrentPoint(QString name, double value, double time);
    void UpdateSeries(const QString &name, QPointF pf, const int &rowId);
    void ChangeChartAxisX(const double &time);
    void ChangeChartAxisY(const QString &name, const double &value, const int &rowId);
    void UpdataListData(int row, double value);

public:
    QWidget *center_ = nullptr;
    QWidget *threshold_setting_window_ = nullptr;//阈值设置窗口
    QDoubleSpinBox *threshold_box = nullptr;
    QHBoxLayout *main_layout_;
    QSplitter *graphic_splitter_;
    QMenu *signal_list_widget_menu_ = nullptr;
    QListWidget *signal_list_widget_ = nullptr;
    QAction *act_delete_item_;
    QAction *re_chose_;
    QAction *chose_all_;
    QAction *chose_none_;
    QAction *onlye_chose_;
    QAction *chart_window_switch_;//曲线窗口开关
    QAction *threshold_setting_;    //阈值设置
    QMap<int, KLine> series_map_; /// manage series, key is the row of listWidget
    QMap<int, QMap<double, double>> row_time_val_map_;
    QStringList signal_names_;    /// manage signal whether it's selected before;

    CChart *chart_ = nullptr;
    CChartViewCursor *chart_view_cursor_ = nullptr;
    std::mutex delete_data_Lock_;
    QMap<QString, int> name_to_row_num_;
    double max_axis_x_ = 0;

private:
    double y_min_, y_max_, x_max_ = 0;
    QString name_ = "px";
    QMap<QString, double> threshold_value_map_;//阈值存储键值对

    QMap<QString, QString>
        NameParam_ = {{"CamObjects_fc", "fc"},
                      {"CamObjects_ac", "ac"},
                      {"RadarObjects_fr", "fr"},
                      {"RadarObjects_cr", "cr"},
                      {"FusObjects_ac", "acFus"},
                      {"FusObjects_fc", "fcFus"},
                      {"FusObjects_cr", "crFus"},
                      {"FusObjects_fr", "frFus"},
                      {"FusObjects_sc", "scFus"},
                      {"FusExObjects_fus", "exFus"},
                      {"FusObjects_fus", "fus"}};

    void ThresholdSettingFinished();
    void HideOrShowChartWindow();
    void ThresholdSetting();
    void CreateActions();
    void ReCheck();
    void SetAllItem(bool is_checked);
    void SetItemChecked(QListWidgetItem *item, int is_checked = -1);

protected:
    void closeEvent(QCloseEvent *event) override;
signals:
    void SigGraphicClose();
    void DeletechartSignal(const QString &);
    void DeleteSignalGraphicSignal(const QString &);
};

#endif // KGRAPHIC_WIDGET_H
