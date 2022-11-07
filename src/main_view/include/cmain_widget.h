#ifndef CMAIN_WIDGET_H
#define CMAIN_WIDGET_H

#include <QtWidgets>
#include "cmdi_area.h"

class CMainWidget : public QWidget
{
    Q_OBJECT
protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    explicit CMainWidget(QWidget *parent = nullptr);
    ~CMainWidget();

public:
    CMdiArea *mdi_area_;

private:
    void InitUi();

private:
    QSplitter *main_splitter_;
    QHBoxLayout *main_layout_;
};

#endif // CMAIN_WIDGET_H