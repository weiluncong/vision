#include "cmain_widget.h"
#include "cexplorer_box.h"

CMainWidget::CMainWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    InitUi();
}

CMainWidget::~CMainWidget()
{
}

/** @brief 窗口放大缩小的时候保持内部子窗口也可以放大缩小*/
void CMainWidget::resizeEvent(QResizeEvent *event)
{
    static int w = this->width();
    static int h = this->height();
}

/** @brief 初始化主窗口界面*/
void CMainWidget::InitUi()
{
    mdi_area_ = new CMdiArea(this);
    main_layout_ = new QHBoxLayout(this);
    main_splitter_ = new QSplitter(Qt::Horizontal);
    main_splitter_->addWidget(mdi_area_);
    main_splitter_->addWidget(CExplorerBox::GetCExplorerBox());
    CExplorerBox::GetCExplorerBox()->hide();
    main_splitter_->setSizes({900, 100});
    main_layout_->addWidget(main_splitter_);
}
