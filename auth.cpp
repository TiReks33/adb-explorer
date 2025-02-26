#include "auth.h"

QString const auth::con_name_="ADBEXPLORER";

QMap<QString,QString> auth::SQLDBtype2SQLdriver;

QMap<QString,int> auth::SQLdriver2SQLDBtype;

auth::auth(){

    QString const _MYSQLdrv = "QMYSQL";
    QString const _MARIADBdrv = "QMARIADB";
    QString const _POSTGRESQLdrv = "QPSQL";

    SQLDBtype2SQLdriver.insert("MYSQL",_MYSQLdrv);
    SQLDBtype2SQLdriver.insert("MARIADB",_MARIADBdrv);
    SQLDBtype2SQLdriver.insert("PSQL",_POSTGRESQLdrv);


    SQLdriver2SQLDBtype[_MARIADBdrv] = SQLDBtype::MARIADB;
    SQLdriver2SQLDBtype[_MYSQLdrv] = SQLDBtype::MYSQL;
    SQLdriver2SQLDBtype[_POSTGRESQLdrv] = SQLDBtype::PSQL;
}

auth &auth::copy_(const auth &auth__)
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

bool auth::SQLdriverMatch(const QString &curDrvName__, SQLDBtype matchedSQLDB__)
{
    return (auth::SQLdriver2SQLDBtype[curDrvName__]==matchedSQLDB__);
}
