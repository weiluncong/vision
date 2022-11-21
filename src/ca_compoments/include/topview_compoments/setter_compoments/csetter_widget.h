#ifndef CSETTER_WIDGET_H
#define CSETTER_WIDGET_H

#include <QtWidgets>
#include <QLayout>
#include <QTableView>
#include <QStandardItemModel>

class CSetterParam
{
public:
    CSetterParam() {}
    CSetterParam(bool status, const QColor &color)
        : check_status_(status), color_(color) {}

public:
    bool check_status_ = false;
    QColor color_;
};

class CSetterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSetterWidget(QWidget *parent = nullptr);
    ~CSetterWidget() {}
    void CreateSetter(const QString &name);
    QMap<QString, CSetterParam> GetSetterParams() { return params_map_; }

public slots:
    void HandleActItemClicked(const QModelIndex &index);
    void HandleActColorChanged(int index);

private:
    QStandardItem *CreateItem(const QString &name);
    QComboBox *CreateComboBox(const QString &name);

private:
    QHBoxLayout *main_layout_;
    QTableView *main_table_;
    QStandardItemModel *table_model_;

    int color_index_ = 0;
    QMap<QString, CSetterParam> params_map_;
    const QList<QColor> color_list_ = {
        Qt::color0, Qt::color1, Qt::black, Qt::white, Qt::darkGray,
        Qt::gray, Qt::lightGray, Qt::red, Qt::green, Qt::blue, Qt::cyan,
        Qt::magenta, Qt::yellow, Qt::darkRed, Qt::darkGreen, Qt::darkBlue,
        Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow};

signals:
    void SigCheckChanged(const QString &, bool);
    void SigColorChanged(const QString &, const QColor &);
};

#endif // CSETTER_WIDGET_H