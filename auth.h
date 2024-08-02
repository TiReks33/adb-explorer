#ifndef AUTH_H
#define AUTH_H
#include <QString>



//enum class default_con_
//{
//    default_ = 0,
//    non_default_ = 1
//};

struct auth{
    auth() :
        db_server_("QMYSQL")
      , host_("localhost")
      //, con_name_("ADBEXPLORER")
    {}
//private:
    QString db_server_; //default
    QString host_;

    QString login_; //db_window
    QString passw_;

    QString db_name_; //tables_window
    QString table_name_;

    static QString const con_name_;

};

#endif // AUTH_H
