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

    ~auth(){
        ////qDebug() << "~auth destructor called";
    }


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

    // copy object data to other
    auth& copy_(auth const& auth__)
    {
        if(this!=&auth__){

            db_driver_=auth__.db_driver_;
            host_=auth__.host_;
            port_=auth__.port_;

            login_=auth__.login_;
            passw_=auth__.passw_;

            db_name_=auth__.db_name_;

        }
        return *this;
    }

    static QMap<QString,QString> SQLDBtype2SQLdriver;

    static QMap<QString,int> SQLdriver2SQLDBtype;

    static bool SQLdriverMatch(QString const&, SQLDBtype);
};



#endif // AUTH_H


