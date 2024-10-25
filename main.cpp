#include "loginwindow.h"
#include <QApplication>
#include <QtMessageHandler>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(customMessageHandler);

    QDir dir(adbexplorer::filepath_);
    if(!dir.exists())
        dir.mkpath(".");

    loginWindow w;
    w.setWindowIcon(QIcon(":/pic/anthead2.png"));
    w.show();

    return a.exec();
}
