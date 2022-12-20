#ifndef CSIGNALFILTER_H
#define CSIGNALFILTER_H

#include <mutex>
#include <QMap>
#include <QString>
#include <QDebug>
#include <memory>
#include "cyaml_handler.h"
#include "cglobal_param.h"

class CSignalFilter final
{
public:
    CSignalFilter();
    ~CSignalFilter();

    bool Initial();

    bool IsPassed(const QString &signal_name, const QString &filter_name);

private:
    /// key  is filter name value is filter signal name
    QMap<QString, QStringList> pass_signals_;
};

#endif // CSIGNALFILTER_H