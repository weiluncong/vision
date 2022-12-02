#include "csda_common_parser.h"

cav::CPointData CSdaCommmonParser::ParserPoint(const google::protobuf::Message &msg)
{
    cav::CPointData point;
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; i++)
    {
        auto field = descriptor->field(i);
        if (field->lowercase_name() == "x")
        {
            point.x_ = FieldToQStr(msg, field).toDouble();
        }
        else if (field->lowercase_name() == "y")
        {
            point.y_ = FieldToQStr(msg, field).toDouble();
        }
        else if (field->lowercase_name() == "z")
        {
            point.z_ = FieldToQStr(msg, field).toDouble();
        }
    }
    return point;
}

cav::CMapPoint CSdaCommmonParser::ParserGnss(const google::protobuf::Message &msg)
{
    cav::CMapPoint gnss;
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; i++)
    {
        auto field = descriptor->field(i);
        if (field->lowercase_name() == "lat" || field->lowercase_name() == "latitude")
        {
            gnss.latitude_ = FieldToQStr(msg, field).toDouble();
        }
        else if (field->lowercase_name() == "lng" || field->lowercase_name() == "longitude")
        {
            gnss.longitude_ = FieldToQStr(msg, field).toDouble();
        }
    }
    return gnss;
}

QString CSdaCommmonParser::FieldToQStr(const google::protobuf::Message &msg,
                                    const google::protobuf::FieldDescriptor *field)
{
    if(nullptr == field)
        return "";

    return QString::fromStdString(FieldTypeConvert(msg, field));
}
