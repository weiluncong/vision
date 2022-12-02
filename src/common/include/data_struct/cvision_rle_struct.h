#ifndef CVISION_RLE_STRUCT_H
#define CVISION_RLE_STRUCT_H

#include <QVector>
#include "opencv2/core.hpp"

namespace cav
{
    // RLE对应三通道色彩类型
    enum CRLEType
    {
        Road,          //  color(128,64,128)
        Sidewalk,      //  color(70,70,70)
        Person,        //  color(220,20,60)
        Car,           //  color(0,0,142)
        Motareycle,    //  color(0,0,70)
        Trafficlane,   //  color(200,200,200)
        Guideline,     //  color(0,192,192)
        Stopline,      //  color(255,0,0)
        Fencerle,      //  color(190,153,153)
        Crosswalkline, //  color(64,192,0)
        Signline,      //  color(200,200,128)
        Trafficcone,   //  color(0,64,64)
        Pole,          //  color(153,153,153)
        Trafficlight,  //  color(250,170,30)
        Traffic_arrow, //  color(128,0,192)
        Speed_bump,    //  color(220,220,0)
    };

    // 视觉语义分割用车道线类型
    enum CLineType
    {
        Unknown,         // gray (240,255,240)
        LaneLine,        // blue  color(65,105,225) LaneLine：[/],(2,/,/),[/],(1,0),车道线
        CurbLine,        // green color(0,255,0) Curb：[/],(4,/,/),[/],(1,0),路沿
        CenterLine,      // yello color(227,207,87) Center：[/],(8,/,/),[/],(1,0),车道中心线
        GuardBar,        //  color(255,97,3) Guardrail：[/],(16,/,/),[/],(1,0),护栏
        ConcreteBarrier, //  color(200,180,33) ConcreteBarrier：[/],(32,/,/),[/],(1,0),混泥土护栏
        Fence,           //  color(255,128,0) Fence：[/],(64,/,/),[/],(1,0),栅栏，表征道路边界
        Wall,            //  color(255,0,0) Wall：[/],(128,/,/),[/],(1,0),墙
        Canopy,          //  color(50,205,50) Canopy：[/],(256,/,/),[/],(1,0),天蓬
        Virtual,         //  color(163,128,148) Virtual：[/],(512,/,/),[/],(1,0), 虚拟线
        Cone             //  color(255,255,0) Cone：[/],(1024,/,/),[/],(1,0),锥桶线
    };

    /**
     * @brief vision use： 基础语义分割
     * 目前语义分割图均为 480 * 270图像分割结果，原图目前为 3840 * 2160
     * @param row  行数
     * @param startCol 起始列
     * @param endCol 结束列
     */
    class CSDARLE
    {
    public:
        CSDARLE() {}
        ~CSDARLE() {}

    public:
        int row_ = 0;       //行数
        int start_col_ = 0; //起始列
        int end_col_ = 0;   //结束列
    };

    /**
     * @brief 视觉车道线区块合集
     * @param type  车道线类型，参考enum CLineType
     * @param line  单条道路语义分割集合
     */
    class CSDALineRle
    {
    public:
        CSDALineRle() {}
        ~CSDALineRle() {}

    public:
        CLineType type_ = CLineType::Unknown;
        QVector<CSDARLE> line_; //车道线像素合集（每个框由宽度，起始行、结束行构成）
    };
    typedef QVector<CSDALineRle> CSDALineRleBlocks;

    /**
     * @brief 视觉车道线合集
     * @param type 车道线了性，二进制值，仅与65536（边界线）作对比区分 if (type & 65536)
     * @param points 单个车道线点集合
     */
    class CSDALineCV
    {
    public:
        CSDALineCV() {}
        ~CSDALineCV() {}

    public:
        CLineType type_ = CLineType::Unknown;
        QVector<cv::Point> points_;
    };
    typedef QVector<CSDALineCV> CSDALineCVBlocks;
}

#endif // CVISION_RLE_STRUCT_H