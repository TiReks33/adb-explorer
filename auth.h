#ifndef AUTH_H
#define AUTH_H
#include <QString>
#include <QDebug>
#include <iostream>

enum SQLDBtype{
    MARIADB,
    MYSQL,
    PSQL
};

// credential structure (cross-window transer solution)
struct auth{

    auth();

    ~auth(){}


    QString db_driver_="";
    QString host_="localhost";

    // if no port added -- default driver port is used
    int port_=-1;

    QString login_="";
    QString passw_="";

    QString db_name_="";

    // default SQL connection name for app (not default connection, which is the unnamed connection)
    static QString const con_name_;

    // debug overload
    friend QDebug operator<<(QDebug stream__, auth const &auth__);

    // copy object data to the other
    auth &copy_(auth const& originalAuthObjToBCopied);

    static QMap<QString,QString> SQLDBtype2SQLdriver;

    static QMap<QString,int> SQLdriver2SQLDBtype;

    static bool SQLdriverMatch(QString const&, SQLDBtype);
};

//QDebug operator<<(QDebug stream__, auth const &auth__){
//    stream__ << "db_server::" << auth__.db_driver_
//             << "host::" << auth__.host_
//             << "db_name::" << auth__.db_name_
////             << "table_name::" << auth__.table_name_
//                ;
//    return stream__;
//}


#endif // AUTH_H


