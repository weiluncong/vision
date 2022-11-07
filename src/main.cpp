#include <QApplication>
#include <QLockFile>
#include <iostream>
#include <sstream>
#include <QDebug>
#include "cmain_window.h"
#include "version.h"
#include "cglobal_param.h"

int main(int argc, char *argv[])
{
    qputenv("QT_PLUGIN_PATH", "./third_party/Qt/plugins");
    QApplication a(argc, argv);
    QLockFile lockFile("./dbc/lock.ini");
    if (!lockFile.tryLock(500))
    {
        return 0;
    }

    std::ostringstream os;
    os << CPACK_PACKAGE_VERSION_MAJOR << "." << CPACK_PACKAGE_VERSION_MINOR
       << "." << CPACK_PACKAGE_VERSION_PATCH;
    gflags::SetVersionString(os.str());
    gflags::ParseCommandLineFlags(&argc, &argv, true);
#if 1
    qSetMessagePattern("[%{time yyyy-MM-dd h:mm:ss.zzz} %{file}:%{line}] : %{message}");
#endif
    CMainWindow w;
    w.show();
    return a.exec();
}
