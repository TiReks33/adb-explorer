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

#include "auth.h"

struct db_connection{

static bool open(auth&);

static bool open(auth&,QString);

static int open(auth&,QString,int);

static void close();

static void close_con(QString const &);

static bool set_query(QString, QSqlQueryModel&,QAbstractItemView*);

static bool set_query(QString, QSqlQueryModel&model__,QTableView*,QHeaderView::ResizeMode scalemode);

static bool set_query(QString, QSqlQueryModel&model__,QTableView*,QHeaderView::ResizeMode scalemode,QString); //NEWNEW

static bool set_query(QString, QSqlQueryModel&model__,QComboBox*,QString,int); //NEWNEWNEW

static bool set_query(QString, QSqlQueryModel&model__,QComboBox*,int);

static bool set_query(QString, QSqlQueryModel*model__,QComboBox*,int);

bool set_query(QString query, QSqlQueryModel&model__, QComboBox *comboBox, QHeaderView::ResizeMode scalemode);

//static bool set_query(QString, QSqlQueryModel*,QTableView*,QHeaderView::ResizeMode scalemode);

bool set_query(QString, /*QSqlQueryModel&,*/QTableView*,QHeaderView::ResizeMode scalemode/*,int*/);

//bool set_query(QString, /*QSqlQueryModel&,*/QTableView*,QHeaderView::ResizeMode scalemode/*,int*/,QString); //NEW

//bool set_query(QString, QSqlQueryModel&,QTableView*,QHeaderView::ResizeMode scalemode,int);

static bool set_query(QString, QSqlQueryModel&,QComboBox*/*,QHeaderView::ResizeMode scalemode*/);

QSqlQueryModel model_;
};

#endif // DB_CONNECTION_H
