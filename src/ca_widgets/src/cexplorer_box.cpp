#include "cexplorer_box.h"
#include "cglobal_param.h"

CExplorerBox *CExplorerBox::explorer_box_ = nullptr;

CExplorerBox::CExplorerBox(QWidget *parent)
    : QGroupBox(parent)
{
    shift_is_pressed_ = false;
    setAttribute(Qt::WA_DeleteOnClose);
    Init();
}


CExplorerBox *CExplorerBox::GetCExplorerBox()
{
    if (!explorer_box_)
        explorer_box_ = new CExplorerBox();
    return explorer_box_;
}


CExplorerBox::~CExplorerBox() {}

void CExplorerBox::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        QModelIndexList selected = items_list_view_->selectionModel()->selectedIndexes();
        for (QModelIndex index : selected)
        {
            emit SendSignal(index);
        }

        items_list_view_->selectionModel()->clear();
    }
}

/** @brief 定义搜索栏控件*/
void CExplorerBox::Init()
{
    setFont(QFont("微软雅黑", 10));
    setTitle("ExplorerBox");
    setMinimumWidth(200);
    main_layout_ = new QVBoxLayout(this);
    items_comboBox_ = new QComboBox;
    items_comboBox_->setMinimumHeight(25);
    items_comboBox_->setFont(QFont("微软雅黑", 10));
    items_comboBox_->setEditable(true);
    items_comboBox_->setFocusPolicy(Qt::ClickFocus);
    items_comboBox_->setCurrentText("");
    connect(items_comboBox_, &QComboBox::editTextChanged, this, &CExplorerBox::SetListViewFilter);
    items_list_view_ = new QListView;
    items_list_view_->setSelectionMode(QAbstractItemView::ExtendedSelection);
    item_string_model_ = new QStringListModel(this);
    item_sort_filter_ = new QSortFilterProxyModel(this);
    item_sort_filter_->setSourceModel(item_string_model_);
    items_list_view_->setModel(item_sort_filter_);
    items_list_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(items_list_view_, &QListView::doubleClicked, this, &CExplorerBox::SendSignal);
    main_layout_->addWidget(items_comboBox_);
    main_layout_->addWidget(items_list_view_);
}

/** @brief 发送信号名信号*/
void CExplorerBox::SendSignal(const QModelIndex &index)
{
    QString name = items_list_view_->model()->data(index).toString();
    added_signals_.push_back(TOSTR(name));
    emit CurveSignal(name);
}

/** @brief 定义当筛选框输入内容变化后*/
void CExplorerBox::SetListViewFilter(const QString &txt)
{
    QRegExp re = QRegExp(txt, Qt::CaseInsensitive, QRegExp::Wildcard);
    item_sort_filter_->setFilterRegExp(re);
}

void CExplorerBox::InitCurveSignal()
{
    for (auto name : added_signals_)
    {
        QString sendName = TOQSTR(name);
        emit CurveSignal(sendName);
    }
}
