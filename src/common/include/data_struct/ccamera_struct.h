#ifndef CAMERASTRUCT_H
#define CAMERASTRUCT_H

#include <iostream>
#include <vector>

namespace cav
{
    enum CH265Type
    {
        KeyI,
        KeyP
    };
    class CImageData
    {
    public:
        CImageData() {}
        ~CImageData() {}
        int frame_id_ = 0; //当前分包数
        int p_num_ = 0;    //当前包p帧数
        CH265Type type_ = CH265Type::KeyI;
        std::vector<unsigned char> img_;
    };
} // namespace cav

#endif // CAMERASTRUCT_H