#ifndef CSTM_ERROR_MESSAGE_H
#define CSTM_ERROR_MESSAGE_H


#include <unordered_map>
#include <unordered_set>
#include <QString>
#include <vector>


using namespace std;

/**
 * STM错误信息
 */
class CStmErrorMessage {

public:

    /**
     * 获取错误信息
     * @param signalName 信号名称
     * @param errorCode 错误代码
     * @return
     */
    static string GetErrorMessage(const string &signalName, int errorCode);

    /**
     * 获取错误信息
     * @param signalName 信号名称
     * @param errorCode 错误代码
     * @return
     */
    static QString GetErrorMessage(const QString &signalName, int errorCode);

    /**
     *  是否包含信号名称
     * @param signalName 信号名称
     * @return
     */
    static bool ContainSignalName(const string &signalName);

    /**
     *  是否包含信号名称
     * @param signalName 信号名称
     * @return
     */
    static bool ContainSignalName(const QString &signalName);

private:
    CStmErrorMessage();

    ~CStmErrorMessage();

    static CStmErrorMessage *p;

    /**
     * 获取错误信息
     * @param signalName 信号名称
     * @param errorCode 错误代码
     * @return
     */
    string GetMessage(const string &signalName, int errorCode);

    /**
     * 部分信号需要根据错误代码的字节位去获取错误信息
     * @param signalName 信号名称
     * @param errorCode 错误代码
     * @return
     */
    vector<int> GetErrorCodeByBit(const string &signalName, int errorCode);

    //需要用错误代码的二进制去校验的信号名称
    unordered_set<string> signal_name_to_check_bit_error_code_set_ = {"StmProto.DebugSTM-lngAvailableWord", "StmProto.DebugSTM-latAvailableWord"};


    //key: 信号名称  value: {key:错误代码，value:错误信息}
    unordered_map<string, unordered_map<int, string>> signal_error_map_ = {
            {"OdcProto.FromODC-odc397Textinh-odcCruiseFeaturerescond1text",
                    {
                            {0, "无故障"},     {1, "控制器硬件故障"},  {2, "控制器通讯故障"},            {3, "FR故障"},    {4, "左侧前角雷达故障"},
                            {5, "右侧前角雷达故障"},        {6, "左侧后角雷达故障"},    {7, "右侧后角雷达故障"},    {8, "前区12U故障"},    {9, "后区12U故障"},
                            {10, "前摄像头故障"}, {11, "VSM故障"}, {12, "4V前摄像头故障"}, {13, "定位传感器故障"}, {14, "按键故障"},
                            {15, "IACC按键故障"}, {16, "显示系统故障"}, {17, "转向系统故障"}, {18, "行车制动系统故障"}, {19, "动力系统故障"},
                            {20, "动力系统通讯故障"}, {21, "驻车制动系统故障"}, {22, "车辆传感器故障"}, {23, "门盖状态不确定"}, {24, "转向灯开关故障"},
                            {25, "灯光系统故障"},     {26, "配置未打开"}, {27, "非安全状态"}, {28, "FR未标定"}, {29, "左侧前角雷达未标定"},
                            {30, "右侧前角雷达未标定"}, {31, "左侧后角雷达未标定"}, {32, "右侧后角雷达未标定"}, {33, "FC未标定"}, {34, "VSM未标定"}, {35, "4V未标定"}, {36, "续航里程不足"}, {37, "轮胎异常"}, {38, "轮胎状态不确认"}, {39, "制动盘过热"},
                            {40, "大雨"}, {41, "路面颠簸"}, {42, "FR被遮挡"}, {43, "左侧前角雷达被遮挡"}, {44, "右侧前角雷达被遮挡"},
                            {45, "左侧后角雷达被遮挡"}, {46, "右侧后角雷达被遮挡"}, {47, "前区12U被遮挡"}, {48, "后区12U被遮挡"}, {49, "FC被遮挡"},
                            {50, "VSM被遮挡"}, {51, "4V被遮挡"}, {52, "IMS被遮挡"}, {53, "检测不到驾驶员"}, {54, "疲劳"},
                            {55, "注意力分散"}, {56, "脱手"}, {57, "无法检测驾驶员状态"}, {58, "车门未全关闭"}, {59, "未系安全带"},
                            {60, "未启动车辆"}, {61, "未开启ESP"}, {62, "IACC未启用"}, {63, "UDLC未启用"}, {64, "NDA未启用"},
                            {65, "无导航信息"}, {66, "手力矩过大"}, {67, "车速超出范围"}, {68, "未处于前进档"}, {69, "方向盘转角过大"},
                            {70, "EPB未释放"}, {71, "静止未踩刹车"}, {72, "运动时踩刹车"}, {73, "CANCEL按键未释放"}, {74, "4V后摄像头故障"},
                            {75, "4V侧摄像头故障"}, {76, "左前门状态不确定"}, {77, "安全气囊控制器故障"}, {78, "4V炫目"}, {79, "中雨"},
                            {80, "温度超过范围"}, {81, "温度超过范围"}, {82, "左前门未关闭"}, {83, "转向灯未关闭"},
                            {85, "AEB接口响应抑制"}, {88, "前溜"}, {89, "后溜"},
                            {90, "路面湿滑"}, {91, "TJP未启用"}, {92, "InfoCAN通讯故障"}, {93, "BodyCAN通讯故障"}, {94, "冗余电池电量过低"},
                            {95, "DDSA未启用"}, {96, "周视左前摄像头故障"}, {97, "周视后向中距离摄像头故障"}, {98, "周视右后摄像头故障"}, {99, "周视右前摄像头故障"},
                            {100, "周视左后摄像头故障"}, {101, "ACC方向盘转角过大"}, {102, "IACC方向盘转角过大"}, {103, "NDA方向盘转角过大"}, {104, "TJP方向盘转角过大"},
                            {105, "DDSA方向盘转角过大"}, {106, "ACC方向盘转速过大"}, {107, "IACC方向盘转速过大"}, {108, "NDA方向盘转速过大"}, {109, "TJP方向盘转速过大"},
                            {110, "DDSA方向盘转速过大"}
                    }
            },
            {"OdcProto.FromODC-odc821Textinh-odcCruiseFeaturerescond2text",
                    {
                            {0, "无故障"},     {1, "地图不可用"},    {2, "障碍物"},                {3, "定位异常"},    {4, "地图非最新状态"},
                            {5, "ALC FIDA道路类型不适用"}, {6, "UDLC道路类型不适用"}, {7, "IACC道路类型不适用"}, {8, "NDA道路类型不适用"}, {9, "不满足电子围栏要求"},
                            {10, "陡坡"},     {11, "急弯"},    {12, "车道错位"},     {13, "施工区域"},    {14, "事故区域"},
                            {15, "匝道区域"},     {16, "收费站区域"},  {17, "非可行驶车道"}, {18, "道路拥堵"},     {19, "车道线不稳定"},
                            {20, "车道宽度不合适"},  {21, "车辆超速"},     {22, "APA已激活"},  {23, "行人"},      {24, "航向角过大"},
                            {25, "TJP道路类型不适用"}, {26, "车辆压线"}
                    }
            },
            {"StmProto.DebugSTM-lngAvailableWord",
                    {
                            {0, "odc功能异常"}, {1, "纵向规划功能异常"}, {2, "纵向控制功能异常"}
                    }
            },
            {"OdcProto.FromODC-odc397Textinh-odcAccFunrescond1text",
                    {
                            {0, "无故障"},     {1, "控制器硬件故障"},  {2, "控制器通讯故障"},            {3, "FR故障"},    {4, "左侧前角雷达故障"},
                            {5, "右侧前角雷达故障"},        {6, "左侧后角雷达故障"},    {7, "右侧后角雷达故障"},    {8, "前区12U故障"},    {9, "后区12U故障"},
                            {10, "前摄像头故障"}, {11, "VSM故障"}, {12, "4V前摄像头故障"}, {13, "定位传感器故障"}, {14, "按键故障"},
                            {15, "IACC按键故障"}, {16, "显示系统故障"}, {17, "转向系统故障"}, {18, "行车制动系统故障"}, {19, "动力系统故障"},
                            {20, "动力系统通讯故障"}, {21, "驻车制动系统故障"}, {22, "车辆传感器故障"}, {23, "门盖状态不确定"}, {24, "转向灯开关故障"},
                            {25, "灯光系统故障"},     {26, "配置未打开"}, {27, "非安全状态"}, {28, "FR未标定"}, {29, "左侧前角雷达未标定"},
                            {30, "右侧前角雷达未标定"}, {31, "左侧后角雷达未标定"}, {32, "右侧后角雷达未标定"}, {33, "FC未标定"}, {34, "VSM未标定"}, {35, "4V未标定"}, {36, "续航里程不足"}, {37, "轮胎异常"}, {38, "轮胎状态不确认"}, {39, "制动盘过热"},
                            {40, "大雨"}, {41, "路面颠簸"}, {42, "FR被遮挡"}, {43, "左侧前角雷达被遮挡"}, {44, "右侧前角雷达被遮挡"},
                            {45, "左侧后角雷达被遮挡"}, {46, "右侧后角雷达被遮挡"}, {47, "前区12U被遮挡"}, {48, "后区12U被遮挡"}, {49, "FC被遮挡"},
                            {50, "VSM被遮挡"}, {51, "4V被遮挡"}, {52, "IMS被遮挡"}, {53, "检测不到驾驶员"}, {54, "疲劳"},
                            {55, "注意力分散"}, {56, "脱手"}, {57, "无法检测驾驶员状态"}, {58, "车门未全关闭"}, {59, "未系安全带"},
                            {60, "未启动车辆"}, {61, "未开启ESP"}, {62, "IACC未启用"}, {63, "UDLC未启用"}, {64, "NDA未启用"},
                            {65, "无导航信息"}, {66, "手力矩过大"}, {67, "车速超出范围"}, {68, "未处于前进档"}, {69, "方向盘转角过大"},
                            {70, "EPB未释放"}, {71, "静止未踩刹车"}, {72, "运动时踩刹车"}, {73, "CANCEL按键未释放"}, {74, "4V后摄像头故障"},
                            {75, "4V侧摄像头故障"}, {76, "左前门状态不确定"}, {77, "安全气囊控制器故障"}, {78, "4V炫目"}, {79, "中雨"},
                            {80, "温度超过范围"}, {81, "温度超过范围"}, {82, "左前门未关闭"}, {83, "转向灯未关闭"},
                            {85, "AEB接口响应抑制"}, {88, "前溜"}, {89, "后溜"},
                            {90, "路面湿滑"}, {91, "TJP未启用"}, {92, "InfoCAN通讯故障"}, {93, "BodyCAN通讯故障"}, {94, "冗余电池电量过低"},
                            {95, "DDSA未启用"}, {96, "周视左前摄像头故障"}, {97, "周视后向中距离摄像头故障"}, {98, "周视右后摄像头故障"}, {99, "周视右前摄像头故障"},
                            {100, "周视左后摄像头故障"}, {101, "ACC方向盘转角过大"}, {102, "IACC方向盘转角过大"}, {103, "NDA方向盘转角过大"}, {104, "TJP方向盘转角过大"},
                            {105, "DDSA方向盘转角过大"}, {106, "ACC方向盘转速过大"}, {107, "IACC方向盘转速过大"}, {108, "NDA方向盘转速过大"}, {109, "TJP方向盘转速过大"},
                            {110, " DDSA方向盘转速过大"}
                    }
            },
            {"OdcProto.FromODC-odc397Textinh-odcAccFunrescond1text",
                    {
                            {0, "无故障"},     {1, "地图不可用"},    {2, "障碍物"},                {3, "定位异常"},    {4, "地图非最新状态"},
                            {5, "ALC FIDA道路类型不适用"}, {6, "UDLC道路类型不适用"}, {7, "IACC道路类型不适用"}, {8, "NDA道路类型不适用"}, {9, "不满足电子围栏要求"},
                            {10, "陡坡"},     {11, "急弯"},    {12, "车道错位"},     {13, "施工区域"},    {14, "事故区域"},
                            {15, "匝道区域"},     {16, "收费站区域"},  {17, "非可行驶车道"}, {18, "道路拥堵"},     {19, "车道线不稳定"},
                            {20, "车道宽度不合适"},  {21, "车辆超速"},     {22, "APA已激活"},  {23, "行人"},      {24, "航向角过大"},
                            {25, "TJP道路类型不适用"}, {26, "车辆压线"}
                    }
            },
            {"StmProto.DebugSTM-latAvailableWord",
                    {
                            {0, "驾驶员超越"},   {1, "横向控制功能异常"}, {2, "轨迹有效标志path_valid无效"}, {3, "横向功能被禁用"}, {4, "规划报警等级状态有报警"},
                            {5, "odc功能异常"}
                    }
            },
            {"OdcProto.FromODC-odc397Textinh-odcLccFeaturerescond1text",
                    {
                            {0, "无故障"},     {1, "控制器硬件故障"},  {2, "控制器通讯故障"},            {3, "FR故障"},    {4, "左侧前角雷达故障"},
                            {5, "右侧前角雷达故障"},        {6, "左侧后角雷达故障"},    {7, "右侧后角雷达故障"},    {8, "前区12U故障"},    {9, "后区12U故障"},
                            {10, "前摄像头故障"}, {11, "VSM故障"}, {12, "4V前摄像头故障"}, {13, "定位传感器故障"}, {14, "按键故障"},
                            {15, "IACC按键故障"}, {16, "显示系统故障"}, {17, "转向系统故障"}, {18, "行车制动系统故障"}, {19, "动力系统故障"},
                            {20, "动力系统通讯故障"}, {21, "驻车制动系统故障"}, {22, "车辆传感器故障"}, {23, "门盖状态不确定"}, {24, "转向灯开关故障"},
                            {25, "灯光系统故障"},     {26, "配置未打开"}, {27, "非安全状态"}, {28, "FR未标定"}, {29, "左侧前角雷达未标定"},
                            {30, "右侧前角雷达未标定"}, {31, "左侧后角雷达未标定"}, {32, "右侧后角雷达未标定"}, {33, "FC未标定"}, {34, "VSM未标定"},
                                                                                                                    {35, "4V未标定"}, {36, "续航里程不足"}, {37, "轮胎异常"}, {38, "轮胎状态不确认"}, {39, "制动盘过热"},
                            {40, "大雨"}, {41, "路面颠簸"}, {42, "FR被遮挡"}, {43, "左侧前角雷达被遮挡"}, {44, "右侧前角雷达被遮挡"},
                            {45, "左侧后角雷达被遮挡"}, {46, "右侧后角雷达被遮挡"}, {47, "前区12U被遮挡"}, {48, "后区12U被遮挡"}, {49, "FC被遮挡"},
                            {50, "VSM被遮挡"}, {51, "4V被遮挡"}, {52, "IMS被遮挡"}, {53, "检测不到驾驶员"}, {54, "疲劳"},
                            {55, "注意力分散"}, {56, "脱手"}, {57, "无法检测驾驶员状态"}, {58, "车门未全关闭"}, {59, "未系安全带"},
                            {60, "未启动车辆"}, {61, "未开启ESP"}, {62, "IACC未启用"}, {63, "UDLC未启用"}, {64, "NDA未启用"},
                            {65, "无导航信息"}, {66, "手力矩过大"}, {67, "车速超出范围"}, {68, "未处于前进档"}, {69, "方向盘转角过大"},
                            {70, "EPB未释放"}, {71, "静止未踩刹车"}, {72, "运动时踩刹车"}, {73, "CANCEL按键未释放"}, {74, "4V后摄像头故障"},
                            {75, "4V侧摄像头故障"}, {76, "左前门状态不确定"}, {77, "安全气囊控制器故障"}, {78, "4V炫目"}, {79, "中雨"},
                            {80, "温度超过范围"}, {81, "温度超过范围"}, {82, "左前门未关闭"}, {83, "转向灯未关闭"},
                            {85, "AEB接口响应抑制"}, {88, "前溜"}, {89, "后溜"},
                            {90, "路面湿滑"}, {91, "TJP未启用"}, {92, "InfoCAN通讯故障"}, {93, "BodyCAN通讯故障"}, {94, "冗余电池电量过低"},
                            {95, "DDSA未启用"}, {96, "周视左前摄像头故障"}, {97, "周视后向中距离摄像头故障"}, {98, "周视右后摄像头故障"}, {99, "周视右前摄像头故障"},
                            {100, "周视左后摄像头故障"}, {101, "ACC方向盘转角过大"}, {102, "IACC方向盘转角过大"}, {103, "NDA方向盘转角过大"}, {104, "TJP方向盘转角过大"},
                            {105, "DDSA方向盘转角过大"}, {106, "ACC方向盘转速过大"}, {107, "IACC方向盘转速过大"}, {108, "NDA方向盘转速过大"}, {109, "TJP方向盘转速过大"},
                            {110, "DDSA方向盘转速过大"}
                    }
            },
            {"OdcProto.FromODC-odc821Textinh-odcLccFeaturerescond2text",
                    {
                            {0, "无故障"},     {1, "地图不可用"},    {2, "障碍物"},                {3, "定位异常"},    {4, "地图非最新状态"},
                            {5, "ALC FIDA道路类型不适用"}, {6, "UDLC道路类型不适用"}, {7, "IACC道路类型不适用"}, {8, "NDA道路类型不适用"}, {9, "不满足电子围栏要求"},
                            {10, "陡坡"},     {11, "急弯"},    {12, "车道错位"},     {13, "施工区域"},    {14, "事故区域"},
                            {15, "匝道区域"},     {16, "收费站区域"},  {17, "非可行驶车道"}, {18, "道路拥堵"},     {19, "车道线不稳定"},
                            {20, "车道宽度不合适"},  {21, "车辆超速"},     {22, "APA已激活"},  {23, "行人"},      {24, "航向角过大"},
                            {25, "TJP道路类型不适用"}, {26, "车辆压线"}
                    }
            },
            {"OdcProto.FromODC-odc397Textinh-odcIaccIaccrescond1text",
                    {
                            {0, "无故障"},     {1, "控制器硬件故障"},  {2, "控制器通讯故障"},            {3, "FR故障"},    {4, "左侧前角雷达故障"},
                            {5, "右侧前角雷达故障"},        {6, "左侧后角雷达故障"},    {7, "右侧后角雷达故障"},    {8, "前区12U故障"},    {9, "后区12U故障"},
                            {10, "前摄像头故障"}, {11, "VSM故障"}, {12, "4V前摄像头故障"}, {13, "定位传感器故障"}, {14, "按键故障"},
                            {15, "IACC按键故障"}, {16, "显示系统故障"}, {17, "转向系统故障"}, {18, "行车制动系统故障"}, {19, "动力系统故障"},
                            {20, "动力系统通讯故障"}, {21, "驻车制动系统故障"}, {22, "车辆传感器故障"}, {23, "门盖状态不确定"}, {24, "转向灯开关故障"},
                            {25, "灯光系统故障"},     {26, "配置未打开"}, {27, "非安全状态"}, {28, "FR未标定"}, {29, "左侧前角雷达未标定"},
                            {30, "右侧前角雷达未标定"}, {31, "左侧后角雷达未标定"}, {32, "右侧后角雷达未标定"}, {33, "FC未标定"}, {34, "VSM未标定"}, {35, "4V未标定"}, {36, "续航里程不足"}, {37, "轮胎异常"}, {38, "轮胎状态不确认"}, {39, "制动盘过热"},
                            {40, "大雨"}, {41, "路面颠簸"}, {42, "FR被遮挡"}, {43, "左侧前角雷达被遮挡"}, {44, "右侧前角雷达被遮挡"},
                            {45, "左侧后角雷达被遮挡"}, {46, "右侧后角雷达被遮挡"}, {47, "前区12U被遮挡"}, {48, "后区12U被遮挡"}, {49, "FC被遮挡"},
                            {50, "VSM被遮挡"}, {51, "4V被遮挡"}, {52, "IMS被遮挡"}, {53, "检测不到驾驶员"}, {54, "疲劳"},
                            {55, "注意力分散"}, {56, "脱手"}, {57, "无法检测驾驶员状态"}, {58, "车门未全关闭"}, {59, "未系安全带"},
                            {60, "未启动车辆"}, {61, "未开启ESP"}, {62, "IACC未启用"}, {63, "UDLC未启用"}, {64, "NDA未启用"},
                            {65, "无导航信息"}, {66, "手力矩过大"}, {67, "车速超出范围"}, {68, "未处于前进档"}, {69, "方向盘转角过大"},
                            {70, "EPB未释放"}, {71, "静止未踩刹车"}, {72, "运动时踩刹车"}, {73, "CANCEL按键未释放"}, {74, "4V后摄像头故障"},
                            {75, "4V侧摄像头故障"}, {76, "左前门状态不确定"}, {77, "安全气囊控制器故障"}, {78, "4V炫目"}, {79, "中雨"},
                            {80, "温度超过范围"}, {81, "温度超过范围"}, {82, "左前门未关闭"}, {83, "转向灯未关闭"},
                            {85, "AEB接口响应抑制"}, {88, "前溜"}, {89, "后溜"},
                            {90, "路面湿滑"}, {91, "TJP未启用"}, {92, "InfoCAN通讯故障"}, {93, "BodyCAN通讯故障"}, {94, "冗余电池电量过低"},
                            {95, "DDSA未启用"}, {96, "周视左前摄像头故障"}, {97, "周视后向中距离摄像头故障"}, {98, "周视右后摄像头故障"}, {99, "周视右前摄像头故障"},
                            {100, "周视左后摄像头故障"}, {101, "ACC方向盘转角过大"}, {102, "IACC方向盘转角过大"}, {103, "NDA方向盘转角过大"}, {104, "TJP方向盘转角过大"},
                            {105, "DDSA方向盘转角过大"}, {106, "ACC方向盘转速过大"}, {107, "IACC方向盘转速过大"}, {108, "NDA方向盘转速过大"}, {109, "TJP方向盘转速过大"},
                            {110, "DDSA方向盘转速过大"}
                    }
            },
            {"OdcProto.FromODC-odc821Textinh-odcIaccIaccrescond2text",
                    {
                            {0, "无故障"},     {1, "地图不可用"},    {2, "障碍物"},                {3, "定位异常"},    {4, "地图非最新状态"},
                            {5, "ALC FIDA道路类型不适用"}, {6, "UDLC道路类型不适用"}, {7, "IACC道路类型不适用"}, {8, "NDA道路类型不适用"}, {9, "不满足电子围栏要求"},
                            {10, "陡坡"},     {11, "急弯"},    {12, "车道错位"},     {13, "施工区域"},    {14, "事故区域"},
                            {15, "匝道区域"},     {16, "收费站区域"},  {17, "非可行驶车道"}, {18, "道路拥堵"},     {19, "车道线不稳定"},
                            {20, "车道宽度不合适"},  {21, "车辆超速"},     {22, "APA已激活"},  {23, "行人"},      {24, "航向角过大"},
                            {25, "TJP道路类型不适用"}, {26, "车辆压线"}
                    }
            },
            {"OdcProto.FromODC-odc397Textinh-odcAlcFeaturerescond1text",
                    {
                            {0, "无故障"},     {1, "控制器硬件故障"},  {2, "控制器通讯故障"},            {3, "FR故障"},    {4, "左侧前角雷达故障"},
                            {5, "右侧前角雷达故障"},        {6, "左侧后角雷达故障"},    {7, "右侧后角雷达故障"},    {8, "前区12U故障"},    {9, "后区12U故障"},
                            {10, "前摄像头故障"}, {11, "VSM故障"}, {12, "4V前摄像头故障"}, {13, "定位传感器故障"}, {14, "按键故障"},
                            {15, "IACC按键故障"}, {16, "显示系统故障"}, {17, "转向系统故障"}, {18, "行车制动系统故障"}, {19, "动力系统故障"},
                            {20, "动力系统通讯故障"}, {21, "驻车制动系统故障"}, {22, "车辆传感器故障"}, {23, "门盖状态不确定"}, {24, "转向灯开关故障"},
                            {25, "灯光系统故障"},     {26, "配置未打开"}, {27, "非安全状态"}, {28, "FR未标定"}, {29, "左侧前角雷达未标定"},
                            {30, "右侧前角雷达未标定"}, {31, "左侧后角雷达未标定"}, {32, "右侧后角雷达未标定"}, {33, "FC未标定"}, {34, "VSM未标定"}, {35, "4V未标定"}, {36, "续航里程不足"}, {37, "轮胎异常"}, {38, "轮胎状态不确认"}, {39, "制动盘过热"},
                            {40, "大雨"}, {41, "路面颠簸"}, {42, "FR被遮挡"}, {43, "左侧前角雷达被遮挡"}, {44, "右侧前角雷达被遮挡"},
                            {45, "左侧后角雷达被遮挡"}, {46, "右侧后角雷达被遮挡"}, {47, "前区12U被遮挡"}, {48, "后区12U被遮挡"}, {49, "FC被遮挡"},
                            {50, "VSM被遮挡"}, {51, "4V被遮挡"}, {52, "IMS被遮挡"}, {53, "检测不到驾驶员"}, {54, "疲劳"},
                            {55, "注意力分散"}, {56, "脱手"}, {57, "无法检测驾驶员状态"}, {58, "车门未全关闭"}, {59, "未系安全带"},
                            {60, "未启动车辆"}, {61, "未开启ESP"}, {62, "IACC未启用"}, {63, "UDLC未启用"}, {64, "NDA未启用"},
                            {65, "无导航信息"}, {66, "手力矩过大"}, {67, "车速超出范围"}, {68, "未处于前进档"}, {69, "方向盘转角过大"},
                            {70, "EPB未释放"}, {71, "静止未踩刹车"}, {72, "运动时踩刹车"}, {73, "CANCEL按键未释放"}, {74, "4V后摄像头故障"},
                            {75, "4V侧摄像头故障"}, {76, "左前门状态不确定"}, {77, "安全气囊控制器故障"}, {78, "4V炫目"}, {79, "中雨"},
                            {80, "温度超过范围"}, {81, "温度超过范围"}, {82, "左前门未关闭"}, {83, "转向灯未关闭"},
                            {85, "AEB接口响应抑制"}, {88, "前溜"}, {89, "后溜"},
                            {90, "路面湿滑"}, {91, "TJP未启用"}, {92, "InfoCAN通讯故障"}, {93, "BodyCAN通讯故障"}, {94, "冗余电池电量过低"},
                            {95, "DDSA未启用"}, {96, "周视左前摄像头故障"}, {97, "周视后向中距离摄像头故障"}, {98, "周视右后摄像头故障"}, {99, "周视右前摄像头故障"},
                            {100, "周视左后摄像头故障"}, {101, "ACC方向盘转角过大"}, {102, "IACC方向盘转角过大"}, {103, "NDA方向盘转角过大"}, {104, "TJP方向盘转角过大"},
                            {105, "DDSA方向盘转角过大"}, {106, "ACC方向盘转速过大"}, {107, "IACC方向盘转速过大"}, {108, "NDA方向盘转速过大"}, {109, "TJP方向盘转速过大"},
                            {110, "DDSA方向盘转速过大"}
                    }
            },
            {"OdcProto.FromODC-odc821Textinh-odcAlcFeaturerescond2text",
                    {
                            {0, "无故障"},     {1, "地图不可用"},    {2, "障碍物"},                {3, "定位异常"},    {4, "地图非最新状态"},
                            {5, "ALC FIDA道路类型不适用"}, {6, "UDLC道路类型不适用"}, {7, "IACC道路类型不适用"}, {8, "NDA道路类型不适用"}, {9, "不满足电子围栏要求"},
                            {10, "陡坡"},     {11, "急弯"},    {12, "车道错位"},     {13, "施工区域"},    {14, "事故区域"},
                            {15, "匝道区域"},     {16, "收费站区域"},  {17, "非可行驶车道"}, {18, "道路拥堵"},     {19, "车道线不稳定"},
                            {20, "车道宽度不合适"},  {21, "车辆超速"},     {22, "APA已激活"},  {23, "行人"},      {24, "航向角过大"},
                            {25, "TJP道路类型不适用"}, {26, "车辆压线"}
                    }
            },
            {"OdcProto.FromODC-odc397Textinh-odcFidaFeaturerescond1text",
                    {
                            {0, "无故障"},     {1, "控制器硬件故障"},  {2, "控制器通讯故障"},            {3, "FR故障"},    {4, "左侧前角雷达故障"},
                            {5, "右侧前角雷达故障"},        {6, "左侧后角雷达故障"},    {7, "右侧后角雷达故障"},    {8, "前区12U故障"},    {9, "后区12U故障"},
                            {10, "前摄像头故障"}, {11, "VSM故障"}, {12, "4V前摄像头故障"}, {13, "定位传感器故障"}, {14, "按键故障"},
                            {15, "IACC按键故障"}, {16, "显示系统故障"}, {17, "转向系统故障"}, {18, "行车制动系统故障"}, {19, "动力系统故障"},
                            {20, "动力系统通讯故障"}, {21, "驻车制动系统故障"}, {22, "车辆传感器故障"}, {23, "门盖状态不确定"}, {24, "转向灯开关故障"},
                            {25, "灯光系统故障"},     {26, "配置未打开"}, {27, "非安全状态"}, {28, "FR未标定"}, {29, "左侧前角雷达未标定"},
                            {30, "右侧前角雷达未标定"}, {31, "左侧后角雷达未标定"}, {32, "右侧后角雷达未标定"}, {33, "FC未标定"}, {34, "VSM未标定"}, {35, "4V未标定"}, {36, "续航里程不足"}, {37, "轮胎异常"}, {38, "轮胎状态不确认"}, {39, "制动盘过热"},
                            {40, "大雨"}, {41, "路面颠簸"}, {42, "FR被遮挡"}, {43, "左侧前角雷达被遮挡"}, {44, "右侧前角雷达被遮挡"},
                            {45, "左侧后角雷达被遮挡"}, {46, "右侧后角雷达被遮挡"}, {47, "前区12U被遮挡"}, {48, "后区12U被遮挡"}, {49, "FC被遮挡"},
                            {50, "VSM被遮挡"}, {51, "4V被遮挡"}, {52, "IMS被遮挡"}, {53, "检测不到驾驶员"}, {54, "疲劳"},
                            {55, "注意力分散"}, {56, "脱手"}, {57, "无法检测驾驶员状态"}, {58, "车门未全关闭"}, {59, "未系安全带"},
                            {60, "未启动车辆"}, {61, "未开启ESP"}, {62, "IACC未启用"}, {63, "UDLC未启用"}, {64, "NDA未启用"},
                            {65, "无导航信息"}, {66, "手力矩过大"}, {67, "车速超出范围"}, {68, "未处于前进档"}, {69, "方向盘转角过大"},
                            {70, "EPB未释放"}, {71, "静止未踩刹车"}, {72, "运动时踩刹车"}, {73, "CANCEL按键未释放"}, {74, "4V后摄像头故障"},
                            {75, "4V侧摄像头故障"}, {76, "左前门状态不确定"}, {77, "安全气囊控制器故障"}, {78, "4V炫目"}, {79, "中雨"},
                            {80, "温度超过范围"}, {81, "温度超过范围"}, {82, "左前门未关闭"}, {83, "转向灯未关闭"},
                            {85, "AEB接口响应抑制"}, {88, "前溜"}, {89, "后溜"},
                            {90, "路面湿滑"}, {91, "TJP未启用"}, {92, "InfoCAN通讯故障"}, {93, "BodyCAN通讯故障"}, {94, "冗余电池电量过低"},
                            {95, "DDSA未启用"}, {96, "周视左前摄像头故障"}, {97, "周视后向中距离摄像头故障"}, {98, "周视右后摄像头故障"}, {99, "周视右前摄像头故障"},
                            {100, "周视左后摄像头故障"}, {101, "ACC方向盘转角过大"}, {102, "IACC方向盘转角过大"}, {103, "NDA方向盘转角过大"}, {104, "TJP方向盘转角过大"},
                            {105, "DDSA方向盘转角过大"}, {106, "ACC方向盘转速过大"}, {107, "IACC方向盘转速过大"}, {108, "NDA方向盘转速过大"}, {109, "TJP方向盘转速过大"},
                            {110, "DDSA方向盘转速过大"}
                    }
            },
            {"OdcProto.FromODC-odc821Textinh-odcFidaFeaturerescond2text",
                    {
                            {0, "无故障"},     {1, "地图不可用"},    {2, "障碍物"},                {3, "定位异常"},    {4, "地图非最新状态"},
                            {5, "ALC FIDA道路类型不适用"}, {6, "UDLC道路类型不适用"}, {7, "IACC道路类型不适用"}, {8, "NDA道路类型不适用"}, {9, "不满足电子围栏要求"},
                            {10, "陡坡"},     {11, "急弯"},    {12, "车道错位"},     {13, "施工区域"},    {14, "事故区域"},
                            {15, "匝道区域"},     {16, "收费站区域"},  {17, "非可行驶车道"}, {18, "道路拥堵"},     {19, "车道线不稳定"},
                            {20, "车道宽度不合适"},  {21, "车辆超速"},     {22, "APA已激活"},  {23, "行人"},      {24, "航向角过大"},
                            {25, "TJP道路类型不适用"}, {26, "车辆压线"}
                    }
            },
            {"OdcProto.FromODC-odc397Textinh-odcNdaFunrescond1text",
                    {
                            {0, "无故障"},     {1, "控制器硬件故障"},  {2, "控制器通讯故障"},            {3, "FR故障"},    {4, "左侧前角雷达故障"},
                            {5, "右侧前角雷达故障"},        {6, "左侧后角雷达故障"},    {7, "右侧后角雷达故障"},    {8, "前区12U故障"},    {9, "后区12U故障"},
                            {10, "前摄像头故障"}, {11, "VSM故障"}, {12, "4V前摄像头故障"}, {13, "定位传感器故障"}, {14, "按键故障"},
                            {15, "IACC按键故障"}, {16, "显示系统故障"}, {17, "转向系统故障"}, {18, "行车制动系统故障"}, {19, "动力系统故障"},
                            {20, "动力系统通讯故障"}, {21, "驻车制动系统故障"}, {22, "车辆传感器故障"}, {23, "门盖状态不确定"}, {24, "转向灯开关故障"},
                            {25, "灯光系统故障"},     {26, "配置未打开"}, {27, "非安全状态"}, {28, "FR未标定"}, {29, "左侧前角雷达未标定"},
                            {30, "右侧前角雷达未标定"}, {31, "左侧后角雷达未标定"}, {32, "右侧后角雷达未标定"}, {33, "FC未标定"}, {34, "VSM未标定"},
                                                                                                                    {35, "4V未标定"}, {36, "续航里程不足"}, {37, "轮胎异常"}, {38, "轮胎状态不确认"}, {39, "制动盘过热"},
                            {40, "大雨"}, {41, "路面颠簸"}, {42, "FR被遮挡"}, {43, "左侧前角雷达被遮挡"}, {44, "右侧前角雷达被遮挡"},
                            {45, "左侧后角雷达被遮挡"}, {46, "右侧后角雷达被遮挡"}, {47, "前区12U被遮挡"}, {48, "后区12U被遮挡"}, {49, "FC被遮挡"},
                            {50, "VSM被遮挡"}, {51, "4V被遮挡"}, {52, "IMS被遮挡"}, {53, "检测不到驾驶员"}, {54, "疲劳"},
                            {55, "注意力分散"}, {56, "脱手"}, {57, "无法检测驾驶员状态"}, {58, "车门未全关闭"}, {59, "未系安全带"},
                            {60, "未启动车辆"}, {61, "未开启ESP"}, {62, "IACC未启用"}, {63, "UDLC未启用"}, {64, "NDA未启用"},
                            {65, "无导航信息"}, {66, "手力矩过大"}, {67, "车速超出范围"}, {68, "未处于前进档"}, {69, "方向盘转角过大"},
                            {70, "EPB未释放"}, {71, "静止未踩刹车"}, {72, "运动时踩刹车"}, {73, "CANCEL按键未释放"}, {74, "4V后摄像头故障"},
                            {75, "4V侧摄像头故障"}, {76, "左前门状态不确定"}, {77, "安全气囊控制器故障"}, {78, "4V炫目"}, {79, "中雨"},
                            {80, "温度超过范围"}, {81, "温度超过范围"}, {82, "左前门未关闭"}, {83, "转向灯未关闭"},
                            {85, "AEB接口响应抑制"}, {88, "前溜"}, {89, "后溜"},
                            {90, "路面湿滑"}, {91, "TJP未启用"}, {92, "InfoCAN通讯故障"}, {93, "BodyCAN通讯故障"}, {94, "冗余电池电量过低"},
                            {95, "DDSA未启用"}, {96, "周视左前摄像头故障"}, {97, "周视后向中距离摄像头故障"}, {98, "周视右后摄像头故障"}, {99, "周视右前摄像头故障"},
                            {100, "周视左后摄像头故障"}, {101, "ACC方向盘转角过大"}, {102, "IACC方向盘转角过大"}, {103, "NDA方向盘转角过大"}, {104, "TJP方向盘转角过大"},
                            {105, "DDSA方向盘转角过大"}, {106, "ACC方向盘转速过大"}, {107, "IACC方向盘转速过大"}, {108, "NDA方向盘转速过大"}, {109, "TJP方向盘转速过大"},
                            {110, "DDSA方向盘转速过大"}
                    }
            },
            {"OdcProto.FromODC-odc821Textinh-odcNdaFunrescond2text",
                    {
                            {0, "无故障"},     {1, "地图不可用"},    {2, "障碍物"},                {3, "定位异常"},    {4, "地图非最新状态"},
                            {5, "ALC FIDA道路类型不适用"}, {6, "UDLC道路类型不适用"}, {7, "IACC道路类型不适用"}, {8, "NDA道路类型不适用"}, {9, "不满足电子围栏要求"},
                            {10, "陡坡"},     {11, "急弯"},    {12, "车道错位"},     {13, "施工区域"},    {14, "事故区域"},
                            {15, "匝道区域"},     {16, "收费站区域"},  {17, "非可行驶车道"}, {18, "道路拥堵"},     {19, "车道线不稳定"},
                            {20, "车道宽度不合适"},  {21, "车辆超速"},     {22, "APA已激活"},  {23, "行人"},      {24, "航向角过大"},
                            {25, "TJP道路类型不适用"}, {26, "车辆压线"}
                    }
            },
            {"OdcProto.FromODC-odc397Textinh-odcTjpFunrescond1text",
                    {
                            {0, "无故障"},     {1, "控制器硬件故障"},  {2, "控制器通讯故障"},            {3, "FR故障"},    {4, "左侧前角雷达故障"},
                            {5, "右侧前角雷达故障"},        {6, "左侧后角雷达故障"},    {7, "右侧后角雷达故障"},    {8, "前区12U故障"},    {9, "后区12U故障"},
                            {10, "前摄像头故障"}, {11, "VSM故障"}, {12, "4V前摄像头故障"}, {13, "定位传感器故障"}, {14, "按键故障"},
                            {15, "IACC按键故障"}, {16, "显示系统故障"}, {17, "转向系统故障"}, {18, "行车制动系统故障"}, {19, "动力系统故障"},
                            {20, "动力系统通讯故障"}, {21, "驻车制动系统故障"}, {22, "车辆传感器故障"}, {23, "门盖状态不确定"}, {24, "转向灯开关故障"},
                            {25, "灯光系统故障"},     {26, "配置未打开"}, {27, "非安全状态"}, {28, "FR未标定"}, {29, "左侧前角雷达未标定"},
                            {30, "右侧前角雷达未标定"}, {31, "左侧后角雷达未标定"}, {32, "右侧后角雷达未标定"}, {33, "FC未标定"}, {34, "VSM未标定"},
                                                                                                                    {35, "4V未标定"}, {36, "续航里程不足"}, {37, "轮胎异常"}, {38, "轮胎状态不确认"}, {39, "制动盘过热"},
                            {40, "大雨"}, {41, "路面颠簸"}, {42, "FR被遮挡"}, {43, "左侧前角雷达被遮挡"}, {44, "右侧前角雷达被遮挡"},
                            {45, "左侧后角雷达被遮挡"}, {46, "右侧后角雷达被遮挡"}, {47, "前区12U被遮挡"}, {48, "后区12U被遮挡"}, {49, "FC被遮挡"},
                            {50, "VSM被遮挡"}, {51, "4V被遮挡"}, {52, "IMS被遮挡"}, {53, "检测不到驾驶员"}, {54, "疲劳"},
                            {55, "注意力分散"}, {56, "脱手"}, {57, "无法检测驾驶员状态"}, {58, "车门未全关闭"}, {59, "未系安全带"},
                            {60, "未启动车辆"}, {61, "未开启ESP"}, {62, "IACC未启用"}, {63, "UDLC未启用"}, {64, "NDA未启用"},
                            {65, "无导航信息"}, {66, "手力矩过大"}, {67, "车速超出范围"}, {68, "未处于前进档"}, {69, "方向盘转角过大"},
                            {70, "EPB未释放"}, {71, "静止未踩刹车"}, {72, "运动时踩刹车"}, {73, "CANCEL按键未释放"}, {74, "4V后摄像头故障"},
                            {75, "4V侧摄像头故障"}, {76, "左前门状态不确定"}, {77, "安全气囊控制器故障"}, {78, "4V炫目"}, {79, "中雨"},
                            {80, "温度超过范围"}, {81, "温度超过范围"}, {82, "左前门未关闭"}, {83, "转向灯未关闭"},
                            {85, "AEB接口响应抑制"}, {88, "前溜"}, {89, "后溜"},
                            {90, "路面湿滑"}, {91, "TJP未启用"}, {92, "InfoCAN通讯故障"}, {93, "BodyCAN通讯故障"}, {94, "冗余电池电量过低"},
                            {95, "DDSA未启用"}, {96, "周视左前摄像头故障"}, {97, "周视后向中距离摄像头故障"}, {98, "周视右后摄像头故障"}, {99, "周视右前摄像头故障"},
                            {100, "周视左后摄像头故障"}, {101, "ACC方向盘转角过大"}, {102, "IACC方向盘转角过大"}, {103, "NDA方向盘转角过大"}, {104, "TJP方向盘转角过大"},
                            {105, "DDSA方向盘转角过大"}, {106, "ACC方向盘转速过大"}, {107, "IACC方向盘转速过大"}, {108, "NDA方向盘转速过大"}, {109, "TJP方向盘转速过大"},
                            {110, "DDSA方向盘转速过大"}
                    }
            },
            {"OdcProto.FromODC-odc821Textinh-odcTjpFunrescond2text",
                    {
                            {0, "无故障"},     {1, "地图不可用"},    {2, "障碍物"},                {3, "定位异常"},    {4, "地图非最新状态"},
                            {5, "ALC FIDA道路类型不适用"}, {6, "UDLC道路类型不适用"}, {7, "IACC道路类型不适用"}, {8, "NDA道路类型不适用"}, {9, "不满足电子围栏要求"},
                            {10, "陡坡"},     {11, "急弯"},    {12, "车道错位"},     {13, "施工区域"},    {14, "事故区域"},
                            {15, "匝道区域"},     {16, "收费站区域"},  {17, "非可行驶车道"}, {18, "道路拥堵"},     {19, "车道线不稳定"},
                            {20, "车道宽度不合适"},  {21, "车辆超速"},     {22, "APA已激活"},  {23, "行人"},      {24, "航向角过大"},
                            {25, "TJP道路类型不适用"}, {26, "车辆压线"}
                    }
            }
    };


};

#endif //CSTM_ERROR_MESSAGE_H