#ifndef CAMERASTRUCT_H
#define CAMERASTRUCT_H

#include <iostream>
#include <vector>

namespace cav
{
    class CImageData
    {
    public:
        CImageData() {}
        ~CImageData() {}
        std::vector<unsigned char> img;
    };
}

#endif // CAMERASTRUCT_H