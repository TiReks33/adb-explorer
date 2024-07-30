#ifndef MULTI_CONNECTION_H
#define MULTI_CONNECTION_H

#include <QStringList>


struct multi_connection{
    multi_connection() //:
//        con_name_("")

//      , unique_number_(int(0))
    {}

    void delete_sql_connection();

    static QStringList multi_con_names_;

    QStringList con_list_;
    //QString con_name_;

    static int con_counter_;
    static int unique_number_;

};

#endif // MULTI_CONNECTION_H
