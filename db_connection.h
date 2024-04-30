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

#include "auth.h"

struct db_connection{

static bool open(auth&);

static void close();

static bool set_query(QString, QSqlQueryModel&,QAbstractItemView*);

static bool set_query(QString, QSqlQueryModel&model__,QTableView*,QHeaderView::ResizeMode scalemode);

//static bool set_query(QString, QSqlQueryModel*,QTableView*,QHeaderView::ResizeMode scalemode);

bool set_query(QString, /*QSqlQueryModel&,*/QTableView*,QHeaderView::ResizeMode scalemode/*,int*/);

//bool set_query(QString, QSqlQueryModel&,QTableView*,QHeaderView::ResizeMode scalemode,int);

static bool set_query(QString, QSqlQueryModel&,QComboBox*/*,QHeaderView::ResizeMode scalemode*/);

QSqlQueryModel model_;
};

#endif // DB_CONNECTION_H
