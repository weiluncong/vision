#include "ccamline_parser.h"

void CCamLineParser::ParseCamLines(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("CameraProto.CamLines"))
    {
        return;
    }
    QVector<CLineData> line_vector;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto lines_field = descriptor->FindFieldByName("lines");
    if (!lines_field)
        return;
    int size = reflection->FieldSize(msg, lines_field);
    for (int n = 0; n < size; ++n)
    {
        CLineData line;
        const auto &nmsg = reflection->GetRepeatedMessage(msg, lines_field, n);
        ParseLine(nmsg, line);
        line_vector.push_back(line);
    }
    data_center_->InsertValue<QVector<CLineData>>(package_msg_name, time, line_vector);
    ParseFinished("topview", time);
}

void CCamLineParser::ParseLine(const google::protobuf::Message &msg, CLineData &line)
{
    auto descriptor = msg.GetDescriptor();
    AssignStruct(msg, descriptor, line.a0_, "curve_parameter_a0");
    AssignStruct(msg, descriptor, line.a1_, "curve_parameter_a1");
    AssignStruct(msg, descriptor, line.a2_, "curve_parameter_a2");
    AssignStruct(msg, descriptor, line.a3_, "curve_parameter_a3");
    AssignStruct(msg, descriptor, line.type_, "type");
    AssignStruct(msg, descriptor, line.range_, "line_range");
    AssignStruct(msg, descriptor, line.start_, "start_x");
    AssignStruct(msg, descriptor, line.end_, "end_x");
    line.range_ = line.end_ - line.start_;
}