#include "loginwindow.h"
#include <QApplication>
#include <QtMessageHandler>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(adb_utility::customMessageHandler);

    QDir dir(adb_utility::filepath_);
    if(!dir.exists())
        dir.mkpath(".");

    loginWindow w;
    w.setWindowIcon(QIcon(":/pic/adb-explorer_logo.png"));
    w.show();


    return a.exec();
}
