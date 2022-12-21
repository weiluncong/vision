#include "cswc_parser.h"

void CSwcParser::GetMsgSignalName(const QString &source_name, const google::protobuf::Message &msg, bool &flag)
{
    if (source_name.contains("CanPacks"))
    {
        return;
    }
    QString val_name;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);

        if (field)
        {
            if (field->cpp_type() != 10 && !field->is_repeated()) // 判断是否是meesage
            {
                val_name = source_name + '~' + TOQSTR(field->name());
                data_center_->dat_msg_signal_names_.append(val_name);
                flag = true;
            }
            else if (field->cpp_type() != 10 && field->is_repeated())
            {
                val_name = source_name + '~' + TOQSTR(field->name());
                int val_size = msg.GetReflection()->FieldSize(msg, field);

                for (int k = 0; k < val_size; k++)
                {
                    data_center_->dat_msg_signal_names_.append(val_name + '~' + QString::number(k));
                    flag = true;
                }
            }
            else if (field->cpp_type() == 10 && !field->is_repeated())
            {
                const auto &nmsg = reflection->GetMessage(msg, field);
                GetMsgSignalName(source_name + '~' + TOQSTR(field->name()), nmsg, flag);
            }

            else if (field->cpp_type() == 10 && field->is_repeated())
            {
                int repeatedSize = reflection->FieldSize(msg, field);
                for (int j = 0; j < repeatedSize; ++j)
                {
                    const auto &nmsg = reflection->GetRepeatedMessage(msg, field, j);
                    val_name = source_name + '~' + TOQSTR(field->name()) + '~' + QString::number(j);

                    GetMsgSignalName(val_name, nmsg, flag);
                }
            }
        }
    }
}

void CSwcParser::ParseTickTime(const double &tick_data)
{
    data_center_->tick_time_.push_back(tick_data);
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
            double val = time_point[timestamp];
            if (!time_point.isEmpty() && !std::isnan(val) && !std::isinf(val))
            {
                data_center_->InsertValue<double>(siganl_name, timestamp, val);
            }
        }
    }
    ParseFinished("graphic", timestamp);
}

void CSwcParser::ParseOfflineSwcData(const google::protobuf::Message &msg, QStringList &source_names,
                                     const double &timestamp, QMap<double, double> &dat_map)
{
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto field = descriptor->FindFieldByName(TOSTR(source_names.first()));
    if (field)
    {
        if (field->cpp_type() != 10)
        {
            if (!field->is_repeated()) // 判断是否是meesage
                dat_map[timestamp] = FieldToQStr(msg, field).toDouble();
            else if (field->is_repeated() && source_names.last().toInt() < reflection->FieldSize(msg, field))
                dat_map[timestamp] = GetRepeatedMsg(msg, field, source_names.last().toDouble()).toDouble();
        }
        else if (field->cpp_type() == 10)
        {
            if (!field->is_repeated())
            {
                const auto &nmsg = reflection->GetMessage(msg, field);
                source_names.removeFirst();
                ParseOfflineSwcData(nmsg, source_names, timestamp, dat_map);
            }
            else if (field->is_repeated())
            {
                source_names.removeFirst();
                if (source_names.first().toInt() < reflection->FieldSize(msg, field))
                {
                    const auto &nmsg = reflection->GetRepeatedMessage(msg, field, source_names.first().toDouble());
                    source_names.removeFirst();
                    ParseOfflineSwcData(nmsg, source_names, timestamp, dat_map);
                }
            }
        }
    }
}
