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

struct db_connection{
    // get unique connection name if format: [ class-name + next_number ]
    static QString get_unique_con_name(QMetaObject const * class_meta_obj__,multi_connection*multi_con__);

static bool open(auth&);

//static bool open(auth&, default_con_); // switch between methods

static bool open(auth&,QString); // main method for open connection(!)

static int open(auth&,QString,int);

static bool open (auth&,/*QString*/QMetaObject const *,multi_connection*); // Unique name inside

static bool open (auth&,QString,multi_connection*); // Unique name OUTside



static void close();

static void close_con(QString const &);

static void close(QStringList*);






// USES NON DEFAULT CONNECTION -->
static bool set_query(QString, QSqlQueryModel&model__,QTableView*,QHeaderView::ResizeMode scalemode,QString); //NEWNEW

static bool set_query(QString, QSqlQueryModel&model__,QComboBox*,QString,int); //NEWNEWNEW

static bool set_query(QString, QSqlQueryModel&model__,QListWidget*,QString,int); //*qlistwidget

static bool set_query(QString, /*QSqlQueryModel&model__,*/TwoListSelection*,QString/*,int*/); //*double-list class
//<<-





// USES DEFAULT CONNECTION -->
static bool set_query(QString, QSqlQueryModel&,QAbstractItemView*);

static bool set_query(QString, QSqlQueryModel&model__,QTableView*,QHeaderView::ResizeMode scalemode);

static bool set_query(QString, QSqlQueryModel&model__,QComboBox*,int);

static bool set_query(QString, QSqlQueryModel*model__,QComboBox*,int);

bool set_query(QString query, QSqlQueryModel&model__, QComboBox *comboBox, QHeaderView::ResizeMode scalemode);

bool set_query(QString, /*QSqlQueryModel&,*/QTableView*,QHeaderView::ResizeMode scalemode/*,int*/);

static bool set_query(QString, QSqlQueryModel&,QComboBox*/*,QHeaderView::ResizeMode scalemode*/);
//<<-










QSqlQueryModel model_;

};



#endif // DB_CONNECTION_H
