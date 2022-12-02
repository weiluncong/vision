#ifndef CPARSERMANAGER_H
#define CPARSERMANAGER_H

#include <QtWidgets>
#include <QObject>
#include "cglobal_param.h"
#include "cproto_pool.h"
#include "cthread_pool.h"
#include "cdata_center.h"
#include "camera_parser.h"
#include "csda_vision_parser.h"
#include "csda_parser.h"
#include "cobject_parser.h"
#include "csignal_manager.h"
#include "csda_parser.h"

class CParserManager : public QObject
{
    Q_OBJECT
public:
    CParserManager(QObject *parent = nullptr);
    ~CParserManager();
    void Parser(const QList<cReplayData> &list);
    void Parser(double timestamp, const std::string &topic_name, const std::string &data);

private:
    void HandleMetaData(double timestamp, const QString &topic_name, const std::string &data);
    void SplitTopicName(const QString &topic_name, QString &swc_name, QString &package_msg_name);
    void ParseMessage(const std::string &topic_name, const std::string &package_msg_name,
                      const std::string &data, double time);
    void WaitForFinished();
    void InitParseFunc();

    template <typename _Func, typename... _Args>
    void AddParserFun(const QString &key, _Func &&f, _Args &&... args)
    {
        parse_functions_.insert(key, std::bind(std::forward<_Func>(f), std::forward<_Args>(args)...,
                                               std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

private:
    std::shared_ptr<CameraParser> camera_parser_ = nullptr;
    std::shared_ptr<CSdaParser> csda_parser_ = nullptr;
    std::shared_ptr<CSdaVisionParser> cvision_parser_ = nullptr;
    std::shared_ptr<CObjectParser> cobject_parser_ = nullptr;
    std::shared_ptr<CSdaParser> csda_parser = nullptr;

    CProtoPool *proto_pool_ = nullptr;
    CDataCenter *data_center_ = nullptr;
    CSignalManager *signal_manager_ = nullptr;
    double start_time_ = 0;
    double end_time_ = 0;
    QStringList signal_parser_;
    QStringList msg_parsed_;
    QMap<QString, CThreadPool *> parse_pools_;
    QMap<QString, std::function<void(const QString &, const google::protobuf::Message &, double)>> parse_functions_;
};

#endif // CPARSERMANAGER_H