# idraw

可视化项目

- 若编译时出现: c++:inter compiler error: Killed (program cc1plus), 解决思路：将make时使用的线程数减少到处理器内核允许的的线程数量
- main函数中第一行需要添加: qputenv("QT_PLUGIN_PATH", "./third_party/Qt/plugins"); 否则会提示错误: qt.qpa.plugin: Could not find the Qt platform plugin "xcb" in ""
- 添加kpb库，兼容框架capilot最低版本为2.3.1
