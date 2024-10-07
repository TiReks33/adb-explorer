#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginWindow w;
//    Tables* tables_test = new Tables();
//    tables_test->show();
    w.setWindowIcon(QIcon(":/pic/anthead2.png"));
    w.show();
    return a.exec();
}
