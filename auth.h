#ifndef AUTH_H
#define AUTH_H
#include <QString>
struct auth{
    auth() :
        db_server_("QMYSQL")
      , host_("localhost")
    {}
//private:
    QString db_server_; //default
    QString host_;

    QString login_; //db_window
    QString passw_;

    QString db_name_; //tables_window
    QString table_name_;
};

#endif // AUTH_H
