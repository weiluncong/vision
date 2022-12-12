#include "cswc_parser.h"

CSwcParser::CSwcParser()
{
}

void CSwcParser::ParseSwcData(const google::protobuf::Message &msg, const QString &package_msg_name, double timestamp, QMap<double, double> &dat_map)
{
    QStringList source_names = package_msg_name.split("~");
    source_names.removeFirst();
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
                ParseSwcData(nmsg, source_names.join("~"), timestamp, dat_map);
            }
        }
        else
        {
            const auto &nmsg = reflection->GetMessage(msg, field);
            ParseSwcData(nmsg, source_names.join("~"), timestamp, dat_map);
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
            dat_map[timestamp] = StringToAny<double>(FieldToStr(msg, field));
        }
    }
}

void CSwcParser::ParseAllSignalName(const QString &topic_name, const google::protobuf::Message &msg, bool &flag)
{
    if (topic_name.contains("CanPacks"))
        return;

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; ++i)
    {
        auto field = descriptor->field(i);
        if (!field)
            continue;

        QString signal_name = topic_name + '~' + TOQSTR(field->name());
        if (field->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) // type为message
        {
            if (field->is_repeated()) // repeated
            {
                int size = reflection->FieldSize(msg, field);
                for (int j = 0; j < size; ++j)
                {
                    const auto &nmsg = reflection->GetRepeatedMessage(msg, field, j);
                    ParseAllSignalName(signal_name + '~' + QString::number(j), nmsg, flag);
                }
            }
            else
            {
                const auto &nmsg = reflection->GetMessage(msg, field);
                ParseAllSignalName(signal_name, nmsg, flag);
            }
        }
        else // type不为message
        {
            if (field->is_repeated()) // repeated
            {
                int size = msg.GetReflection()->FieldSize(msg, field);
                flag = (size > 0);
                for (int j = 0; j < size; ++j)
                {
                    data_center_->InsertSignalName(signal_name + '~' + QString::number(j));
                }
            }
            else
            {
                data_center_->InsertSignalName(signal_name);
                flag = true;
            }
        }
    }
}