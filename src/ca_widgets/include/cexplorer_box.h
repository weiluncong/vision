#ifndef CEXPLORER_BOX_H
#define CEXPLORER_BOX_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QGroupBox>
#include <QLayout>

class CExplorerBox : public QGroupBox
{
    Q_OBJECT
public:
    CExplorerBox(QWidget *parent = nullptr);
    ~CExplorerBox();

    /*******************************************************8*/
    QVBoxLayout *main_layout_;
    QComboBox *items_comboBox_;
    QListView *items_list_view_;
    QStringListModel *item_string_model_;
    QSortFilterProxyModel *item_sort_filter_;
    std::vector<std::string> added_signals_;
    bool shift_is_pressed_;

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

public:
    static CExplorerBox *GetCExplorerBox();
    void Init();
    void SendSignal(const QModelIndex &index);
    void SetListViewFilter(const QString &txt);
    void InitCurveSignal();

    void MulitSelect(const QModelIndex &index);

    //更新全量信号列表
    void UpdateModelList(const QStringList &model_list){ item_string_model_->setStringList(model_list);}

    //清空列表
    void Clear(){item_string_model_->setStringList(QStringList());}

signals:
    void CurveSignal(QString &);

private:
    static CExplorerBox *explorer_box_;
};

#endif // CEXPLORER_BOX_H
