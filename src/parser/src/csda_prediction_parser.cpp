#include "csda_prediction_parser.h"

void CSDAPredictionParser::ParsePredictions(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("prediction.RNPObjectOut"))
        return;
    QVector<cav::CObjectData> obj_vector;
    QVector<cav::CPredictLine> history_lines_vec;
    QVector<cav::CPredictLine> predict_lines_vec;

    auto reflection = msg.GetReflection();
    auto descriptor = msg.GetDescriptor();
    auto objs_field = descriptor->FindFieldByName("objects");
    int size = reflection->FieldSize(msg, objs_field);
    for (int i = 0; i < size; i++)
    {
        CObjectData obj;
        const auto &obj_msg = reflection->GetRepeatedMessage(msg, objs_field, i);
        auto obj_descript = obj_msg.GetDescriptor();
        auto obj_reflection = obj_msg.GetReflection();
        AssignStruct(obj_msg, obj_descript, obj.timestamp_, "utc");
        AssignStruct(obj_msg, obj_descript, obj.classification_, "class_type");
        AssignStruct(obj_msg, obj_descript, obj.track_id_, "trackid");
        AssignStruct(obj_msg, obj_descript, obj.algorithm_type_, "alg_type");
        {
            auto object_info_field = obj_descript->FindFieldByName("object_info");
            const auto &object_info_msg = obj_reflection->GetMessage(obj_msg, object_info_field);
            auto object_info_descript = object_info_msg.GetDescriptor();
            AssignStruct(object_info_msg, object_info_descript, obj.length_, "length");
            AssignStruct(object_info_msg, object_info_descript, obj.width_, "width");
            AssignStruct(object_info_msg, object_info_descript, obj.sensor_flag_, "detect_sensor");
        }
        {
            auto object_status_field = obj_descript->FindFieldByName("object_status");
            const auto &object_status_msg = obj_reflection->GetMessage(obj_msg, object_status_field);
            auto object_status_descript = object_status_msg.GetDescriptor();
            bool state = false;
            AssignStruct(object_status_msg, object_status_descript, state, "obj_is_moving");
            if (state)
                obj.move_status_ = 1;
            AssignStruct(object_status_msg, object_status_descript, state, "obj_is_oncoming");
            if (state)
                obj.move_status_ = 2;
            AssignStruct(object_status_msg, object_status_descript, state, "obj_is_cross");
            if (state)
                obj.move_status_ = 3;
        }
        {
            auto object_position_field = obj_descript->FindFieldByName("object_position_legacy");
            const auto &object_position_msg = obj_reflection->GetMessage(obj_msg, object_position_field);
            auto obj_position_descript = object_position_msg.GetDescriptor();
            AssignStruct(object_position_msg, obj_position_descript, obj.pos_x_, "posx");
            AssignStruct(object_position_msg, obj_position_descript, obj.pos_y_, "posy");
            AssignStruct(object_position_msg, obj_position_descript, obj.abs_velocity_x_, "velx");
            AssignStruct(object_position_msg, obj_position_descript, obj.abs_velocity_y_, "vely");
            AssignStruct(object_position_msg, obj_position_descript, obj.heading_angle_, "heading_angle_local");
        }
        obj_vector.push_back(obj);

        //历史轨迹解析
        CPredictLine history_line;
        auto history_line_field = obj_descript->FindFieldByName("trajectory_history");
        const auto &history_line_msg = obj_reflection->GetMessage(obj_msg, history_line_field);
        ParserTrajectory(history_line_msg, history_line.predict_line_);
        history_lines_vec.push_back(history_line);

        //预测轨迹解析
        auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodal_prediction");
        const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
        auto predict_lines_multi_descriptor = predict_lines_multi_msg.GetDescriptor();
        auto predict_lines_multi_reflection = predict_lines_multi_msg.GetReflection();
        //轨迹线仅有一条，不做repeate处理
        auto trajector_field = predict_lines_multi_descriptor->FindFieldByName("trajectories");
        const auto &trajector_msg = predict_lines_multi_reflection->GetRepeatedMessage(predict_lines_multi_msg, trajector_field, 0);
        CPredictLine predict_line_multi;
        ParserTrajectory(trajector_msg, predict_line_multi.predict_line_);

        auto prob_field = predict_lines_multi_descriptor->FindFieldByName("probs");
        int size = predict_lines_multi_reflection->FieldSize(predict_lines_multi_msg, prob_field);
        if (size != 0)
            predict_line_multi.prob_ = predict_lines_multi_reflection->GetRepeatedFloat(predict_lines_multi_msg, prob_field, 0);
        predict_lines_vec.push_back(predict_line_multi);
    }
    QString obj_name = package_msg_name + "-obj";
    data_center_->InsertValue<QVector<cav::CObjectData>>(obj_name, time, obj_vector);
    QString history_name = package_msg_name + "-history_trajectory";
    data_center_->InsertValue<QVector<cav::CPredictLine>>(history_name, time, history_lines_vec);
    QString predict_name = package_msg_name + "-predict_trajectory";
    data_center_->InsertValue<QVector<cav::CPredictLine>>(predict_name, time, predict_lines_vec);
    ParseFinished("topview", time);
}

void CSDAPredictionParser::ParsePredictObjectDebug(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("prediction.RNPObjectDebugOut"))
        return;
    QVector<cav::CPredictLine> predictions;

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
            CPredictLine prediction;
            prediction.id_ = id;
            prediction.type_ = cav::CPredictionType::AlgorithmCar;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, prediction);
            if (!prediction.predict_line_.isEmpty())
                predictions.push_back(prediction);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_rule_ped");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CPredictLine prediction;
            prediction.id_ = id;
            prediction.type_ = cav::CPredictionType::AlgorithmPerson;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, prediction);
            if (!prediction.predict_line_.isEmpty())
                predictions.push_back(prediction);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_cslstm");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CPredictLine prediction;
            prediction.id_ = id;
            prediction.type_ = cav::CPredictionType::AlgorithmCslstm;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, prediction);
            if (!prediction.predict_line_.isEmpty())
                predictions.push_back(prediction);
        }
        {
            auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodel_raster");
            const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
            CPredictLine prediction;
            prediction.id_ = id;
            prediction.type_ = cav::CPredictionType::AlgorithmRaster;
            ParserTrajectoryMultimodal(predict_lines_multi_msg, prediction);
            if (!prediction.predict_line_.isEmpty())
                predictions.push_back(prediction);
        }
    }
    data_center_->InsertValue<QVector<cav::CPredictLine>>(package_msg_name, time, predictions);
    ParseFinished("topview", time);
}

void CSDAPredictionParser::ParserTrajectoryMultimodal(const google::protobuf::Message &msg, cav::CPredictLine &debug_line)
{
    auto descriptor = msg.GetDescriptor();
    auto reflection = msg.GetReflection();
    //轨迹线仅有一条，不做repeate处理
    auto trajector_field = descriptor->FindFieldByName("trajectories");
    int size = reflection->FieldSize(msg, trajector_field);
    if (size == 0)
        return;
    const auto &trajector_msg = reflection->GetRepeatedMessage(msg, trajector_field, 0);
    QVector<cav::CPointData> predict_line_multi;
    ParserTrajectory(trajector_msg, predict_line_multi);
    debug_line.predict_line_ = predict_line_multi;

    auto prob_field = descriptor->FindFieldByName("probs");
    debug_line.prob_ = reflection->GetRepeatedFloat(msg, prob_field, 0);
}

void CSDAPredictionParser::ParserTrajectory(const google::protobuf::Message &msg, QVector<cav::CPointData> &line)
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
        if (ValueCheck(point.x_) && ValueCheck(point.y_))
            line.push_back(point);
    }
}
