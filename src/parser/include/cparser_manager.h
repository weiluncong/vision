#ifndef CPARSERMANAGER_H
#define CPARSERMANAGER_H

#include <QtWidgets>
#include <QObject>
#include "cglobal_param.h"
#include "cproto_pool.h"
#include "cthread_pool.h"
#include "cdata_center.h"
#include "camera_parser.h"
#include "cobject_parser.h"
#include "ccamline_parser.h"
#include "cpoint_set_parser.h"
#include "csignal_manager.h"
#include "csda_parser.h"
#include "csda_vision_parser.h"
#include "csda_lidar_parser.h"
#include "csda_prediction_parser.h"
#include "cswc_parser.h"

class CParserManager : public QObject
{
    Q_OBJECT
public:
    CParserManager(QObject *parent = nullptr);
    ~CParserManager();
    void Parser(const QList<cReplayData> &list);
    void Parser(double timestamp, const std::string &topic_name, const std::string &data);

    void AddOnSignal(QString &singal_name){ on_signal_parser_list_.append(singal_name);}
    void DelOnSignal(QString &singal_name){ on_signal_parser_list_.removeOne(singal_name),data_center_->ClearValue<std::shared_ptr<google::protobuf::Message>>(singal_name);}

private:
    void HandleMetaData(double timestamp, const QString &topic_name, const std::string &data);
    void SplitTopicName(const QString &topic_name, QString &swc_name, QString &package_msg_name);
    void ParseMessage(const std::string &topic_name, const std::string &package_msg_name,
                      const std::string &data, double time);
    void ParseStruct(const std::string &topic_name, const std::string &package_msg_name,
                      const std::string &data, double time);
    void RecodeMsgAndGetSignals(const QString &topic_name, const std::string &package_msg_name, const std::string &data, double time);

    void WaitForFinished();
    void InitParseFunc();

    template <typename _Func, typename... _Args>
    void AddParserFun(const QString &key, _Func &&f, _Args &&...args);
    template <typename _Func, typename... _Args>
    void AddParserStructFun(const QString &key, _Func &&f, _Args &&...args);

private:
    std::shared_ptr<CameraParser> camera_parser_ = nullptr;
    std::shared_ptr<CSwcParser> swc_parser_ = nullptr;
    std::shared_ptr<CObjectParser> cobject_parser_ = nullptr;
    std::shared_ptr<CCamLineParser> camline_parser_ = nullptr;
    std::shared_ptr<CPointSetParser> cpoint_set_parser_ = nullptr;
	std::shared_ptr<CSdaParser> csda_parser_ = nullptr;
    std::shared_ptr<CSdaVisionParser> cvision_parser_ = nullptr;
    std::shared_ptr<CSDALidarParser> csda_lidar_parser_ = nullptr;
    std::shared_ptr<CSDAPredictionParser> csda_pediction_parser_ = nullptr;

    CProtoPool *proto_pool_ = nullptr;
    CDataCenter *data_center_ = nullptr;
    CSignalManager *signal_manager_ = nullptr;
    double start_time_ = 0;
    double end_time_ = 0;
    QStringList signal_parser_;     //信号解析白名单
    QStringList msg_parsed_;        //存储被解析的topic名称
    QStringList on_signal_parser_list_; //在线曲线信号解析列表
    QStringList all_msg_signal_names_;  //全量信号名称列表
    QMap<QString, CThreadPool *> parse_pools_;
    QMap<QString, std::function<void(const QString &, const google::protobuf::Message &, double)>> parse_functions_;
    QMap<QString, std::function<void(const QString &, const std::string &data, double)>> parse_struct_functions_;

signals:
    void UpdateExplorerBoxModel(const QStringList &model_list);
};

template <typename _Func, typename... _Args>
void CParserManager::AddParserFun(const QString &key, _Func &&f, _Args &&...args)
{
    parse_functions_.insert(key, std::bind(std::forward<_Func>(f), std::forward<_Args>(args)...,
                                           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

template <typename _Func, typename... _Args>
void CParserManager::AddParserStructFun(const QString &key, _Func &&f, _Args &&...args)
{
    parse_struct_functions_.insert(key, std::bind(std::forward<_Func>(f), std::forward<_Args>(args)...,
                                           std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
#endif // CPARSERMANAGER_H