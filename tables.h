#ifndef TABLES_H
#define TABLES_H

#include <QDialog>
#include <QStatusBar>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QCloseEvent>
#include <QProgressDialog>
#include <QCheckBox>
#include <QMessageBox>

#include "auth.h"
#include "db_connection.h"
#include "select_cells.hpp"
#include "custom_query.h"
#include "customqueryresult.h"
#include "customquerysettings.h"
#include "ui_customquerysettings.h"
#include "delete_db.h"

namespace Ui {
class Tables;
}

class Tables : public QDialog
{
    Q_OBJECT

public:
    explicit Tables(auth&,QWidget *parent = nullptr);

    ~Tables();

    void closeEvent(QCloseEvent *event);


public slots:

    void show_tables();

    void send_custom_query_slot(QString);

    void delete_form_send_slot(QComboBox*);

    void delete_table_slot(QComboBox*);

signals:

    void db_show();

    //void canceled();

    void custom_query(QString);

    void custom_query(QString,QTableView*);

    void custom_query(QString,QSqlQueryModel&,QTableView*);

    void delete_form_request();

private slots:


    void on_showDB_button_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_select_from_table_button_clicked();

//    void on_pushButton_clicked();

    void on_Custom_Query_Button_clicked();

    //void cancel_slot();

    void on_Query_settings_clicked();

    void on_pushButton_clicked();

protected:
    bool event(QEvent* event);

private:
    Ui::Tables *ui;

//    QString db_server_;
//    QString db_login_;
//    QString db_passw_;
//    QString db_host_;
    auth& auth_;

    QSqlQueryModel model_;

    Custom_Query* table_query_window_;

    //QCheckBox* checkbox_;

    CustomQueryResult* custom_query_result_window_;

    CustomQuerySettings* settings_;

    delete_db* delete_table_window_;

};

#endif // TABLES_H
