#include "cswc_parser.h"

void CSwcParser::GetMsgSignalName(const QString &source_name, const google::protobuf::Message &msg, bool &flag)
{
    if (source_name.contains("CanPacks"))
        return;

    QString signal_name;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);
        if (!field)
            continue;

        signal_name = source_name + '~' + TOQSTR(field->name());
        if (field->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE)
        {
            if (field->is_repeated())
            {
                int size = reflection->FieldSize(msg, field);
                for (int j = 0; j < size; j++)
                {
                    const auto &nmsg = reflection->GetRepeatedMessage(msg, field, j);
                    GetMsgSignalName(signal_name + '~' + QString::number(j), nmsg, flag);
                }
            }
            else
            {
                const auto &nmsg = reflection->GetMessage(msg, field);
                GetMsgSignalName(signal_name, nmsg, flag);
            }
        }
        else
        {
            if (field->is_repeated())
            {
                int size = reflection->FieldSize(msg, field);
                for (int j = 0; j < size; j++)
                {
                    data_center_->dat_msg_signal_names_.append(signal_name + '~' + QString::number(j));
                    flag = true;
                }
            }
            else
            {
                data_center_->dat_msg_signal_names_.append(signal_name);
                flag = true;
            }
        }
    }
}

void CSwcParser::ParseOnlineSwcData(const google::protobuf::Message &msg, const QStringList &online_signals, const QString &topic_name,
                                    double timestamp)
{
    for (auto siganl_name : online_signals)
    {
        if (siganl_name.contains(topic_name))
        {
            QStringList source_names = siganl_name.split("~");
            QMap<double, double> time_point;
            source_names.removeFirst();
            ParseOfflineSwcData(msg, source_names, timestamp, time_point);
            double val = time_point.value(timestamp);
            if (!time_point.isEmpty() && !std::isnan(val) && !std::isinf(val))
            {
                data_center_->InsertValue<double>(siganl_name, timestamp, val);
            }
        }
    }
    ParseFinished("graphic", timestamp);
}

void CSwcParser::ParseOfflineSwcData(const google::protobuf::Message &msg, QStringList &source_names,
                                     double timestamp, QMap<double, double> &dat_map)
{
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto field = descriptor->FindFieldByName(TOSTR(source_names.first()));
    if (!field)
        return;

    if (field->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE)
    {
        source_names.removeFirst();
        if (field->is_repeated())
        {
            int index = source_names.takeFirst().toInt();
            if (index < reflection->FieldSize(msg, field))
            {
                const auto &nmsg = reflection->GetRepeatedMessage(msg, field, index);
                ParseOfflineSwcData(nmsg, source_names, timestamp, dat_map);
            }
        }
        else
        {
            const auto &nmsg = reflection->GetMessage(msg, field);
            ParseOfflineSwcData(nmsg, source_names, timestamp, dat_map);
        }
    }
    else
    {
        if (field->is_repeated())
        {
            int index = source_names.last().toInt();
            if (index < reflection->FieldSize(msg, field))
            {
                dat_map[timestamp] = GetRepeatedMsg(msg, field, index).toDouble();
            }
        }
        else
        {
            dat_map[timestamp] = FieldToQStr(msg, field).toDouble();
        }
    }
}
