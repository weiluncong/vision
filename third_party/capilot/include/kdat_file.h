#ifndef KDAT_FILE_H
#define KDAT_FILE_H

// 1.1 init
// 1.2 topic_name中自动添加task_name
const float kDatVersion = 1.2;

#pragma pack(1)
struct KDataFileHead {
    float version;
    size_t protoContentSize;
    uint16_t commentLength;
    char reserve[200];
};
#pragma pack()

#endif  // KDAT_FILE_H