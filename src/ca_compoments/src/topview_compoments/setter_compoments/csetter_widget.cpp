#include "csetter_widget.h"

CSetterWidget::CSetterWidget(QWidget *parent)
    : QWidget(parent)
{
    main_layout_ = new QHBoxLayout(this);
    main_layout_->setAlignment(Qt::AlignTop);
    main_layout_->setContentsMargins(0, 0, 0, 0);

    main_table_ = new QTableView(this);
    main_table_->setShowGrid(false);
    main_table_->verticalHeader()->setVisible(false);
    main_table_->setSortingEnabled(true);
    main_table_->sortByColumn(0, Qt::DescendingOrder);
    connect(main_table_, &QTableView::clicked, this, &CSetterWidget::HandleActItemClicked);

    table_model_ = new QStandardItemModel;
    table_model_->setHorizontalHeaderLabels({"name", "color"});
    main_table_->setModel(table_model_);
    main_table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    main_table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    main_table_->setColumnWidth(1, 100);
    main_layout_->addWidget(main_table_);
}

void CSetterWidget::CreateSetter(const QString &name)
{
    QStandardItem *item = CreateItem(name);
    table_model_->setItem(table_model_->rowCount(), 0, item);
    QComboBox *combox_color = CreateComboBox(name);
    main_table_->setIndexWidget(table_model_->index(table_model_->rowCount() - 1, 1), combox_color);
    main_table_->update();
}

QStandardItem *CSetterWidget::CreateItem(const QString &name)
{
    QStandardItem *item = new QStandardItem();
    item->setCheckState(Qt::Unchecked);
    item->setCheckable(true);
    item->setToolTip(name);
    item->setText(name);
    item->setEditable(false);
    return item;
}

QComboBox *CSetterWidget::CreateComboBox(const QString &name)
{
    QComboBox *combox = new QComboBox();
    combox->setObjectName(name);
    for (QColor color : color_list_)
    {
        QPixmap pix(QSize(28, 16));
        pix.fill(color);
        combox->addItem(QIcon(pix), "");
        combox->setIconSize(QSize(28, 16));
    }
    int index = color_index_ % color_list_.size();
    params_map_[name] = CSetterParam(false, color_list_[index]);
    combox->setCurrentIndex(index);
    ++color_index_;
    connect(combox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CSetterWidget::HandleActColorChanged);
    return combox;
}

void CSetterWidget::HandleActItemClicked(const QModelIndex &index)
{
    auto item = table_model_->item(index.row());
    QString name = item->text();
    bool flag = (item->checkState() == Qt::Checked);
    params_map_[name].check_status_ = flag;
    emit SigCheckChanged(name, flag);
}

void CSetterWidget::HandleActColorChanged(int index)
{
    if (index >= 0 && index < color_list_.size())
    {
        QString name = sender()->objectName();
        QColor color = color_list_[index];
        params_map_[name].color_ = color;
        emit SigColorChanged(name, color);
    }
}