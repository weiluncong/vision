#include "csda_prediction_parser.h"

CSDAPredictionParser::CSDAPredictionParser() 
{
    line_parser_ = new CCamLineParser();
    object_parser_ = new CObjectParser();
}

CSDAPredictionParser::~CSDAPredictionParser()
{
    SAFE_DELETE(line_parser_);
    SAFE_DELETE(object_parser_);
}

void CSDAPredictionParser::ParsePredictions(const QString &package_msg_name, const google::protobuf::Message &msg, double time)
{
    if (!package_msg_name.contains("prediction.RNPObjectOut"))
        return;
    QVector<const google::protobuf::Message *> history_lines_msg;
    QVector<const google::protobuf::Message *> predict_lines_msg;
    object_parser_->ParsePredictionObjects(package_msg_name + "-obj", msg, time);

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

        //历史轨迹解析
        CLineData history_line;
        auto history_line_field = obj_descript->FindFieldByName("trajectory_history");
        const auto &history_line_msg = obj_reflection->GetMessage(obj_msg, history_line_field);
        history_lines_msg.push_back(&history_line_msg);

        //预测轨迹解析
        auto predict_lines_multi_field = obj_descript->FindFieldByName("trajectory_multimodal_prediction");
        const auto &predict_lines_multi_msg = obj_reflection->GetMessage(obj_msg, predict_lines_multi_field);
        auto predict_lines_multi_descriptor = predict_lines_multi_msg.GetDescriptor();
        auto predict_lines_multi_reflection = predict_lines_multi_msg.GetReflection();
        //轨迹线仅有一条，不做repeate处理
        auto trajector_field = predict_lines_multi_descriptor->FindFieldByName("trajectories");
        const auto &trajector_msg = predict_lines_multi_reflection->GetRepeatedMessage(predict_lines_multi_msg, trajector_field, 0);
        predict_lines_msg.push_back(&trajector_msg);
    }
    line_parser_->ParsePredictionLines(package_msg_name + "-history_trajectory", history_lines_msg, time);
    line_parser_->ParsePredictionLines(package_msg_name + "-predict_trajectory", predict_lines_msg, time);
}
