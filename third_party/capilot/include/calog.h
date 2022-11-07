#ifndef CALOG_H
#define CALOG_H

#include <glog/logging.h>
#include "kutility.h"

namespace capilot {

#define CA_INFO LOG(INFO)
#define CA_WARNING LOG(WARNING)
#define CA_ERROR LOG(ERROR)
#define CA_FATAL LOG(FATAL)

#define CA_LOG_INIT                           \
    std::string logPath = "./log";            \
    capilot::createFolderIfNotExist(logPath); \
    FLAGS_log_dir = logPath;                  \
    google::InitGoogleLogging(argv[0]);       \
    FLAGS_colorlogtostderr = 1;               \
    FLAGS_stderrthreshold = 0;

struct CaLogOption {
    int min_log_level;
    int stderr_threshold;
    int max_log_size;
    std::string log_name;
    std::string log_dir;
};

void InitCaLog(const CaLogOption option);
}  // namespace capilot

#endif  // CALOG_H