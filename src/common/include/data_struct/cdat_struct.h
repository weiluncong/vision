#ifndef DATSTRUCT_H
#define DATSTRUCT_H

#include <iostream>

namespace cav
{
    typedef std::string MetaData;

    const int CPageSize = 4096;
    const int CRawDataBufferMaxSize = CPageSize * 1e4; // 40M

#pragma pack(1)
    struct CDataFileHead
    {
        float version;
        size_t proto_content_size;
        uint16_t comment_length;
        char reserve[200];
    };
#pragma pack()

#pragma pack(1)
    struct CRawDataHead
    {
        char name[100]; //    unsigned int size;
        size_t size;
        double timestamp;
    };
#pragma pack()

    struct cReplayData
    {
        CRawDataHead head;
        MetaData mdata;
    };
}

#endif // CAMERASTRUCT_H