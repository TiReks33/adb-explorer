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

// credential structure (cross-window transfer solution)
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

    static QMap<QString,QString> SQLdriver2SQLDBServerName;

    static QMap<QString,int> SQLdriver2SQLDBtype;

    static bool SQLdriverMatch(QString const&, SQLDBtype);

    QString const getConnectionInfo() const;

};



#endif // AUTH_H


