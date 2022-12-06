#include "csda_common_parser.h"

CPointData CSdaCommmonParser::ParserPoint(const google::protobuf::Message &msg)
{
    cav::CPointData point;
    auto descriptor = msg.GetDescriptor();
    AssignStruct(msg, descriptor, point.x_, "x");
    AssignStruct(msg, descriptor, point.y_, "y");
    AssignStruct(msg, descriptor, point.z_, "z");
    return point;
}

CPointData CSdaCommmonParser::ParserGnss(const google::protobuf::Message &msg)
{
    CPointData point;
    auto descriptor = msg.GetDescriptor();
    int field_count = descriptor->field_count();
    for (int i = 0; i < field_count; i++)
    {
        auto field = descriptor->field(i);
        if (field->lowercase_name() == "lat" || field->lowercase_name() == "latitude")
        {
            point.latitude_ = FieldToQStr(msg, field).toDouble();
        }
        else if (field->lowercase_name() == "lng" || field->lowercase_name() == "longitude")
        {
            point.longitude_ = FieldToQStr(msg, field).toDouble();
        }
    }
    return point;
}
