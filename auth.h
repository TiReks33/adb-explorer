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
        db_server_("QMYSQL"/*"QMARIADB"*//*"QMYSQL3"*/)
      , host_("localhost")
//      , con_name_("ADBEXPLORER")
    {}
//private:
    /*static*/ QString db_server_; //default
    /*static*/ QString host_;

    /*static*/ QString login_; //db_window
    /*static*/ QString passw_;

    /*static*/ QString db_name_; //tables_window
    /*static*/ QString table_name_; //?

    //void backup_db_name(){ if(db_name_!="") db_name_backup_=db_name_;}
    //void reset_db_name_(){ if(db_name_backup_!="") db_name_=db_name_backup_;}

    //QString db_name_backup_="";

    static QString const con_name_;

//    static QString const default_con_name_;

};

#endif // AUTH_H


