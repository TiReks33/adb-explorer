#ifndef DATABASES_H
#define DATABASES_H

#include <QDialog>
#include <QStatusBar>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QComboBox>

//#include <loginwindow.h>
#include "tables.h"
#include "auth.h"
#include "select_cells.hpp"
#include "db_connection.h"
#include "create_db_name.h"
#include "delete_db.h"

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



public slots:
    void message_from_login(QString);

    void create_db_slot(QString);

    void delete_form_send_slot(QComboBox*);

    void delete_database_slot(QComboBox*);

signals:
    void test_signal();

    void select_cells_signal(const QModelIndex &,const QModelIndex &);

    void show_tables_signal();

    void delete_form_request();

private slots:
    void on_showDB_button_clicked();


//    void select_cells(const QModelIndex &);

//    void select_cells(int,int);

//    void select_cells(const QModelIndex &,const QModelIndex &);

//    void select_cells(int,int,int,int);


    void on_tableView_activated(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_showTables_button_clicked();

    void on_pushButton_clicked();

    void on_create_db_button_clicked();

    void on_delete_db_button_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_comboBox_test_button_clicked();

private:
    Ui::Databases *ui;


    auth& auth_;

    Tables* tables_window_;

    QSqlQueryModel model_;

    void test();

    create_db_name* new_db_window_;

    delete_db* delete_db_window_;

};

#endif // DATABASES_H
