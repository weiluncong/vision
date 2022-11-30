#include "cload_data.h"
#include "csignal_manager.h"

CLoadData::CLoadData(const QString &file_path)
    : file_path_(file_path)
{
    proto_pool_ = CProtoPool::GetCProtoPool();
    parser_manager_ = new CParserManager;
}

CLoadData::~CLoadData()
{
    SafeClear(data2parse_);
    SAFE_DELETE(parser_manager_);
    malloc_trim(0);
}

void CLoadData::Dat2Replay()
{
    ReplayGetData("ParseFilter");
    parser_manager_->Parser(data2parse_);
    SafeClear(data2parse_);
    malloc_trim(0);
}

void CLoadData::ReplayGetData(const QString &filter)
{
    SafeClear(data2parse_);

    ifs_.open(file_path_.toStdString());
    if (!ifs_.is_open())
    {
        throw std::runtime_error("open file error: " + file_path_.toStdString());
    }

    ifs_.seekg(0, std::ios::beg);
    CDataFileHead file_head;
    ifs_.read((char *)&file_head, sizeof(file_head));
    std::string proto_content;
    if (file_head.proto_content_size > 100000000)
    {
        qDebug() << "proto content size error!!!!!!!";
    }

    proto_content.resize(file_head.proto_content_size);
    ifs_.read((char *)&proto_content[0], file_head.proto_content_size);
    proto_pool_->LoadProtoContent(proto_content);

    std::string comment;
    if (file_head.comment_length > 0)
    {
        comment.resize(file_head.comment_length);
        ifs_.read((char *)&comment[0], file_head.comment_length);
    }

    cReplayData replay_data;
    std::shared_ptr<CSignalFilter> signal_filter = std::shared_ptr<CSignalFilter>(new CSignalFilter());
    while (ifs_.read((char *)&replay_data.head, sizeof(replay_data.head)))
    {
        size_t size = replay_data.head.size;
        if (size > 50000000)
        {
            qDebug() << replay_data.head.timestamp << ": " << replay_data.head.name << "数据错误，请上报问题";
        }
        replay_data.mdata.resize(size);
        ifs_.read(&replay_data.mdata[0], size);
        if (signal_filter->IsPassed(replay_data.head.name, filter) && !QString(replay_data.head.name).contains("topic"))
        {
            data2parse_.push_back(replay_data);
        }
    }
    ifs_.close();
}

void CLoadData::Dat2Csv()
{
}
