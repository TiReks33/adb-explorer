#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QAbstractItemView>
#include <QTableView>
#include <QHeaderView>
#include <QComboBox>
#include <QSqlError>
#include <QMessageBox>
#include <QListWidget>
#include <QSqlRecord>

#include "auth.h"

struct multi_connection; // declarations
class TwoListSelection;


//enum class default_con_{
//    non_default_ = 0,
//    default_=1
//};


struct db_connection{
//    // get unique connection name if format: [ class-name + next_number ]
//    static QString get_unique_con_name(QMetaObject const * class_meta_obj__,multi_connection*multi_con__);



static bool open(auth&,QString const & con_name__ = auth::con_name_);

static bool try_to_reopen(auth &,QString const & con_name__ = auth::con_name_);

static void close/*_con*/(QString const & = auth::con_name_);

static void remove(QString const & = auth::con_name_);







static bool set_query(QString const&, QSqlQueryModel&model__,QListWidget*,QString const & = auth::con_name_); //*qlistwidget

static bool set_query(QString const&, /*QSqlQueryModel&model__,*/TwoListSelection*,QString const & = auth::con_name_); //*double-list class

static bool set_query(QString const&, QSqlQueryModel*model__,QAbstractItemView*, QString const & = auth::con_name_);

static bool set_query(QString const&, QSqlQueryModel*model__,QTableView*,QHeaderView::ResizeMode scalemode=QHeaderView::Stretch,QString const & = auth::con_name_);

static bool set_query(QString const&, QSqlQueryModel*model__,QComboBox*, QString const & = auth::con_name_);



private:
static bool reopen_exist(QString const & = auth::con_name_);

};



#endif // DB_CONNECTION_H
