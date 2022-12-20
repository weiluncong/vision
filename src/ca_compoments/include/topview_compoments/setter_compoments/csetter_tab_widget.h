#ifndef CSETTER_TAB_WIDGET_H
#define CSETTER_TAB_WIDGET_H

#include <QtWidgets>
#include "ctab_bar.h"
#include "csetter_widget.h"

class CSetterTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CSetterTabWidget(QWidget *parent = nullptr);
    ~CSetterTabWidget()
    {
        qDeleteAll(setters_map_);
    }

    void AddCompomentName(const QString &name);
    QMap<QString, CSetterParam> GetAllItemParam() const;

private:
    QMap<QString, CSetterWidget *> setters_map_;

signals:
    void SigCheckChanged(const QString &, bool);
    void SigColorChanged(const QString &, const QColor &);
};

#endif // CSETTER_TAB_WIDGET_H