#include "loginwindow.h"
#include <QApplication>
#include <QtMessageHandler>

bool plugins::enableCryptoPluginFlag = true;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(adb_utility::customMessageHandler);

    QDir dir(adb_utility::filepath_);
    if(!dir.exists())
        dir.mkpath(".");

    if(argc>1)
    {
        std::string arg1{argv[1]};
        if(arg1=="--nocrypto"||arg1=="-nc")
            plugins::enableCryptoPluginFlag = false;
    }

    loginWindow w{};
    w.setWindowIcon(QIcon(":/pic/adb-explorer_logo.png"));
    w.show();


    return a.exec();
}
