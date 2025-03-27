#include "auth.h"

QString const auth::con_name_="ADBEXPLORER";

static QString const _MYSQL = "MYSQL";
static QString const _MARIADB = "MARIADB";
static QString const _POSTGRESQL = "PSQL";

static QString const _MYSQLdrv = "QMYSQL";
static QString const _MARIADBdrv = "QMARIADB";
static QString const _POSTGRESQLdrv = "QPSQL";

QMap<QString,QString> auth::SQLDBtype2SQLdriver{
    {_MYSQL,_MYSQLdrv},
    {_MARIADB,_MARIADBdrv},
    {_POSTGRESQL,_POSTGRESQLdrv}
};

QMap<QString,QString> auth::SQLdriver2SQLDBServerName{
    {_MYSQLdrv,_MYSQL},
    {_MARIADBdrv,_MARIADB},
    {_POSTGRESQLdrv,_POSTGRESQL}
};

QMap<QString,int> auth::SQLdriver2SQLDBtype{
    {_MYSQLdrv,SQLDBtype::MYSQL},
    {_MARIADBdrv,SQLDBtype::MARIADB},
    {_POSTGRESQLdrv,SQLDBtype::PSQL}
};

auth::auth(){

//    QString const _MYSQLdrv = "QMYSQL";
//    QString const _MARIADBdrv = "QMARIADB";
//    QString const _POSTGRESQLdrv = "QPSQL";

//    SQLDBtype2SQLdriver.insert("MYSQL",_MYSQLdrv);
//    SQLDBtype2SQLdriver.insert("MARIADB",_MARIADBdrv);
//    SQLDBtype2SQLdriver.insert("PSQL",_POSTGRESQLdrv);

//    SQLdriver2SQLDBtype[_MARIADBdrv] = SQLDBtype::MARIADB;
//    SQLdriver2SQLDBtype[_MYSQLdrv] = SQLDBtype::MYSQL;
//    SQLdriver2SQLDBtype[_POSTGRESQLdrv] = SQLDBtype::PSQL;

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

QString const auth::getConnectionInfo() const
{
    QString __infoStr = QStringLiteral("Server:-> ['%1'], host:-> ['%2'], port number:-> [%3].")
    .arg(auth::SQLdriver2SQLDBServerName[this->db_driver_])
    .arg(this->host_)
    .arg((this->port_ == -1) ? "driver default port" : QString::number(this->port_));

    return __infoStr;
}


