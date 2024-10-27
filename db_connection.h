#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QAbstractItemView>
#include <QTableView>
#include <QComboBox>
#include <QSqlError>
#include <QMessageBox>
#include <QListWidget>
#include <QSqlRecord>

#include "auth.h"
#include "helping_stuff.h"


class TwoListSelection;

// main interface that provides connect to qSqlDatabase
struct db_connection{

////     get unique connection name if format: [ class-name + next_number ]
////    static QString get_unique_con_name(QMetaObject const * class_meta_obj__,multi_connection*multi_con__);

// open connection; doesn't care that db-connection already exists in list or not -- if not opened -- create/recreate it from scratch
static bool open(auth&,QString const & con_name__ = auth::con_name_/*,QString const & = ""*/);

// runs in 2 phases -- 1) trying to reopen existing connection; if fails -- 2) remove old from list, and recreating from scratch
static bool try_to_reopen(auth &,QString const & con_name__ = auth::con_name_);

// close the qsql connection
static void close/*_con*/(QString const & = auth::con_name_);

// remove connection from list (must be execute if connection is closed )
static void remove(QString const & = auth::con_name_);



friend class loginWindow;

// group of overloaded static functions that places result of sql query exec. to various forms[

// QStringList
static bool set_query(QString const&, QSqlQueryModel&model__,QStringList*,QString const & = auth::con_name_); //*qStringList

// QListWidget
static bool set_query(QString const&, QSqlQueryModel&model__,QListWidget*,QString const & = auth::con_name_); //*qlistwidget

// TwoListSelection custom class (twolistselection.h/.cpp)
static bool set_query(QString const&, /*QSqlQueryModel&model__,*/TwoListSelection*,QString const & = auth::con_name_); //*double-list class

// QAbstractItemView
static bool set_query(QString const&, QSqlQueryModel*model__,QAbstractItemView*, QString const & = auth::con_name_);

// QTableView
static bool set_query(QString const&, QSqlQueryModel*model__,QTableView*,/*QHeaderView::ResizeMode scalemode=QHeaderView::Stretch,*/QString const & = auth::con_name_);

// QComboBox
static bool set_query(QString const&, QSqlQueryModel*model__,QComboBox*, QString const & = auth::con_name_);

// ]

private:

// [utility class, not for external use] opens already exist connection; if connection not exist in list -- returns false;
static bool reopen_exist(QString const & = auth::con_name_);

};



#endif // DB_CONNECTION_H
