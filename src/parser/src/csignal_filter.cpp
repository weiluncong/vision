#include "csignal_filter.h"

CSignalFilter::CSignalFilter()
{
    Initial();
}

CSignalFilter::~CSignalFilter()
{
    SafeClear(pass_signals_);
}

bool CSignalFilter::Initial()
{
    SafeClear(pass_signals_);
    std::shared_ptr<CYamlHandler> yaml = std::shared_ptr<CYamlHandler>(new CYamlHandler("./dbc/config.yaml"));
    if (nullptr == yaml)
    {
        qDebug() << "not found ./dbc/config.yaml!";
        return false;
    }

    std::vector<std::string> yaml_names = {"ParseFilter", "CSVFilter", "RecordFilter", "ConsoleFilter", "SignalFilter"};
    for (auto &name : yaml_names)
    {
        std::string filter_msgs_string = yaml->GetScalarItem<std::string>(name);
        QString filter_msgs = QString::fromStdString(filter_msgs_string);
        filter_msgs = filter_msgs.replace(QRegExp("[nN][uU][lL][lL]"), "");
        pass_signals_[TOQSTR(name)] = filter_msgs.split(",");
    }

    return true;
}

bool CSignalFilter::IsPassed(const QString &signal_name, const QString &filter_name)
{
    if (!pass_signals_.contains(filter_name))
        return false;

    /// 默认为空表示所有的信号都会被解析
    if (pass_signals_[filter_name].size() == 1 && pass_signals_[filter_name][0].isEmpty())
    {
        if (filter_name == "ConsoleFilter" || filter_name == "SignalFilter")
            return false;

        return true;
    }

    return pass_signals_[filter_name].contains(signal_name);
}