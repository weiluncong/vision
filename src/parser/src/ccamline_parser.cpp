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
    line.type_ = (line.type_ == 1 || line.type_ == 2) ? CLineData::DashedLine : CLineData::SolidLine;
    AssignStruct(msg, descriptor, line.range_, "line_range");
    AssignStruct(msg, descriptor, line.start_, "start_x");
    AssignStruct(msg, descriptor, line.end_, "end_x");
    line.range_ = line.end_ - line.start_;
}

void CCamLineParser::ParseVpCamLines(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("VpCameraProto.CamLines"))
    {
        return;
    }
    QVector<CLineData> line_vector;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto field = descriptor->FindFieldByName("lines");
    int size = reflection->FieldSize(msg, field);
    for (int i = 0; i < size; i++)
    {
        const auto &nmsg = reflection->GetRepeatedMessage(msg, field, i);
        auto nreflection = nmsg.GetReflection();
        auto ndescriptor = nmsg.GetDescriptor();
        double near, far;
        AssignStruct(nmsg, ndescriptor, near, "near_x_distance");
        AssignStruct(nmsg, ndescriptor, far, "far_x_distance");
        double distance = far - near;
        int type;
        AssignStruct(nmsg, ndescriptor, type, "type");

        auto nfield = ndescriptor->FindFieldByName("curve_x_parameter_a0");
        int full_size = nreflection->FieldSize(nmsg, nfield);
        for (int i = 0; i < full_size; i++)
        {
            CLineData line;
            line.a0_ = nreflection->GetRepeatedDouble(nmsg, nfield, i);
            line.range_ = distance;
            line.type_ = type;
            line_vector.push_back(line);
        }
        nfield = ndescriptor->FindFieldByName("curve_x_parameter_a1");
        int size = nreflection->FieldSize(nmsg, nfield);
        if (size > full_size)
            size = full_size;
        for (int i = 0; i < size; i++)
        {
            CLineData &line = line_vector[i];
            line.a1_ = nreflection->GetRepeatedDouble(nmsg, nfield, i);
        }
        nfield = ndescriptor->FindFieldByName("curve_x_parameter_a2");
        size = nreflection->FieldSize(nmsg, nfield);
        if (size > full_size)
            size = full_size;
        for (int i = 0; i < size; i++)
        {
            CLineData &line = line_vector[i];
            line.a2_ = nreflection->GetRepeatedDouble(nmsg, nfield, i);
        }
        nfield = ndescriptor->FindFieldByName("curve_x_parameter_a3");
        size = nreflection->FieldSize(msg, nfield);
        if (size > full_size)
            size = full_size;
        for (int i = 0; i < size; i++)
        {
            CLineData &line = line_vector[i];
            line.a3_ = nreflection->GetRepeatedDouble(nmsg, nfield, i);
        }
        nfield = ndescriptor->FindFieldByName("x_begin_piont");
        size = nreflection->FieldSize(nmsg, nfield);
        if (size > full_size)
            size = full_size;
        for (int i = 0; i < size; i++)
        {
            CLineData &line = line_vector[i];
            const auto &point_msg = nreflection->GetRepeatedMessage(nmsg, nfield, i);
            CPointData point = ParserPoint(point_msg);
            line.start_ = point.x_;
        }
        nfield = ndescriptor->FindFieldByName("x_end_piont");
        size = nreflection->FieldSize(nmsg, nfield);
        if (size > full_size)
            size = full_size;
        for (int i = 0; i < size; i++)
        {
            CLineData &line = line_vector[i];
            const auto &point_msg = nreflection->GetRepeatedMessage(nmsg, nfield, i);
            CPointData point = ParserPoint(point_msg);
            line.end_ = point.x_;
        }
    }
    qDebug() << "line_vector size =" << line_vector.size();
    data_center_->InsertValue(package_msg_name, time, line_vector);
    ParseFinished("topview", time);
}

void CCamLineParser::ParsePredictionLines(const QString &package_msg_name, QVector<const google::protobuf::Message *> msgs, double time)
{
    QVector<CLineData> lines_vec;
    for (auto msg : msgs)
    {
        CLineData line;
        ParserTrajectory(*msg, line.points_);
        lines_vec.push_back(line);
    }
    data_center_->InsertValue(package_msg_name, time, lines_vec);
}

void CCamLineParser::ParsePredictObjectDebug(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("prediction.RNPObjectDebugOut"))
        return;
    QVector<CLineData> predict_lines_vec;

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto objs_field = descriptor->FindFieldByName("objects_debug");
    int size = reflection->FieldSize(msg, objs_field);
    for (int i = 0; i < size; i++)
    {
        int id = 0;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, objs_field, i);
        auto obj_descript = obj_msg.GetDescriptor();
        auto obj_reflection = obj_msg.GetReflection();
        AssignStruct(obj_msg, obj_descript, id, "trackid");
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_rule_veh");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CLineData line;
            line.id_ = id;
            line.type_ = CLineData::SolidLine; // AlgorithmCar
            ParserTrajectoryMultimodal(predict_lines_multi_msg, line);
            predict_lines_vec.push_back(line);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_rule_ped");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CLineData line;
            line.id_ = id;
            line.type_ = CLineData::RedSolidLine; // AlgorithmPerson
            ParserTrajectoryMultimodal(predict_lines_multi_msg, line);
            predict_lines_vec.push_back(line);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_cslstm");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CLineData line;
            line.id_ = id;
            line.type_ = CLineData::GreenSolidLine; // AlgorithmCslstm;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, line);
            predict_lines_vec.push_back(line);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_raster");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CLineData line;
            line.id_ = id;
            line.type_ = CLineData::BlueSolidLine; // AlgorithmRaster;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, line);
            predict_lines_vec.push_back(line);
        }
    }
    data_center_->InsertValue(package_msg_name, time, predict_lines_vec);
    ParseFinished("topview", time);
}

void CCamLineParser::ParserTrajectoryMultimodal(const google::protobuf::Message &msg, CLineData &line)
{
    auto descriptor = msg.GetDescriptor();
    auto reflection = msg.GetReflection();
    // 轨迹线仅有一条，不做repeate处理
    auto trajector_field = descriptor->FindFieldByName("trajectories");
    int size = reflection->FieldSize(msg, trajector_field);
    if (size == 0)
        return;
    const auto &trajector_msg = reflection->GetRepeatedMessage(msg, trajector_field, 0);
    ParserTrajectory(trajector_msg, line.points_);

    auto prob_field = descriptor->FindFieldByName("probs");
    line.confidence_ = reflection->GetRepeatedFloat(msg, prob_field, 0);
}

void CCamLineParser::ParserTrajectory(const google::protobuf::Message &msg, QVector<cav::CPointData> &line)
{
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto points_field = descriptor->FindFieldByName("state");
    int size = reflection->FieldSize(msg, points_field);
    for (int i = 0; i < size; i++)
    {
        cav::CPointData point;
        const auto &state_msg = reflection->GetRepeatedMessage(msg, points_field, i);
        auto state_descript = state_msg.GetDescriptor();
        auto state_reflection = state_msg.GetReflection();
        auto point_field = state_descript->FindFieldByName("point");
        auto &point_msg = state_reflection->GetMessage(state_msg, point_field);
        point = ParserPoint(point_msg);
        line.push_back(point);
    }
}

CPointData CCamLineParser::ParserPoint(const google::protobuf::Message &msg)
{
    CPointData point;
    auto descriptor = msg.GetDescriptor();
    AssignStruct(msg, descriptor, point.x_, "x");
    AssignStruct(msg, descriptor, point.y_, "y");
    AssignStruct(msg, descriptor, point.z_, "z");
    return point;
}

void CCamLineParser::ParsePlanningPath(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("PlanningPaths"))
        return;
    QVector<CLineData> line_vector;
    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto field = descriptor->FindFieldByName("planned_paths");
    int size = reflection->FieldSize(msg, field);
    for (int j = 0; j < size; j++)
    {
        CLineData line;
        line.type_ = CLineData::SolidLine;
        const auto &line_msg = reflection->GetRepeatedMessage(msg, field, j);
        auto reflection = line_msg.GetReflection();
        auto descriptor = line_msg.GetDescriptor();
        auto point_field = descriptor->FindFieldByName("point_host");
        int size = reflection->FieldSize(line_msg, point_field);
        for (int i = 0; i < size; i++)
        {
            const auto &point_msg = reflection->GetRepeatedMessage(line_msg, point_field, i);
            line.points_.push_back(ParserPoint(point_msg));
        }
        line_vector.push_back(line);
    }
    data_center_->InsertValue(package_msg_name, time, line_vector);
    ParseFinished("topview", time);
}
