#include "csetter_tab_widget.h"

CSetterTabWidget::CSetterTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabBar(new CTabBar);
    setTabPosition(QTabWidget::East);
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("QTabBar::tab{background:white;}\
                   QTabBar::tab::selected{background:rgb(135,206,235);}\
                   QTabBar::tab::hover{background:rgb(135,206,235);}");
}

void CSetterTabWidget::AddCompomentName(const QString &name)
{
    QString compoment_name = name.split("-")[0];
    if (!setters_map_.contains(compoment_name))
    {
        CSetterWidget *setter = new CSetterWidget(this);
        setters_map_[compoment_name] = setter;
        addTab(setter, compoment_name);
        connect(setter, &CSetterWidget::SigColorChanged, this, &CSetterTabWidget::SigColorChanged);
        connect(setter, &CSetterWidget::SigCheckChanged, this, &CSetterTabWidget::SigCheckChanged);
    }
    setters_map_[compoment_name]->CreateSetter(name);
}

QMap<QString, CSetterParam> CSetterTabWidget::GetAllItemParam() const
{
    QMap<QString, CSetterParam> res;
    for (auto setter : setters_map_.values())
    {
        res.unite(setter->GetSetterParams());
    }
    return res;
}