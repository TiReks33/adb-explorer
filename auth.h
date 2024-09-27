#ifndef AUTH_H
#define AUTH_H
#include <QString>
#include <QDebug>



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


    ~auth(){
        qDebug() << "~auth destructor called";
    }


//private:
    /*static*/ QString db_server_; //default
    /*static*/ QString host_;

    /*static*/ QString login_=""; //db_window
    /*static*/ QString passw_="";

    /*static*/ QString db_name_=""; //tables_window
////    /*static*/ QString table_name_=""; //?

    //void backup_db_name(){ if(db_name_!="") db_name_backup_=db_name_;}
    //void reset_db_name_(){ if(db_name_backup_!="") db_name_=db_name_backup_;}

    //QString db_name_backup_="";

    static QString const con_name_;

//    static QString const default_con_name_;


    friend QDebug operator<<(QDebug stream__, auth const &auth__);

    auth& copy_(auth const& auth__)
    {
        if(this!=&auth__){
            db_server_=auth__.db_server_;
            host_=auth__.host_;
            login_=auth__.login_;
            passw_=auth__.passw_;
            db_name_=auth__.db_name_;
 ////           table_name_=auth__.table_name_;
        }
        return *this;
    }

};



#endif // AUTH_H


