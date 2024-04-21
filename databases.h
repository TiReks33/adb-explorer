#ifndef DATABASES_H
#define DATABASES_H

#include <QDialog>
#include <QStatusBar>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
//#include <loginwindow.h>
#include "tables.h"
#include "auth.h"

namespace Ui {
class Databases;
}

class Databases : public QDialog
{
    Q_OBJECT

public:
    explicit /*Databases(QWidget *parent = nullptr);*/
             Databases(auth&, QWidget *parent = nullptr);
    ~Databases();


             void close_DB();

public slots:
    void message_from_login(QString);

//    void receive_auth(auth&);

signals:
    void test_signal();

    void select_cells_signal(const QModelIndex &,const QModelIndex &);

private slots:
    void on_showDB_button_clicked();

//    void on_pushButton_2_clicked();

    void select_cells(const QModelIndex &);

    void select_cells(int,int);

    void select_cells(const QModelIndex &,const QModelIndex &);

    void select_cells(int,int,int,int);


    void on_tableView_activated(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_showTables_button_clicked();

    void on_pushButton_clicked();

private:
    Ui::Databases *ui;
    //QStatusBar* status_bar_;
    //QSqlDatabase db_connection_;

//    QString db_server_;
//    QString db_login_;
//    QString db_passw_;
//    QString db_host_;

    auth& auth_;

    Tables* tables_window_;

    void test();

};

#endif // DATABASES_H
