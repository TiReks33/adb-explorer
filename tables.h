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

#include "auth.h"
#include "db_connection.h"
#include "select_cells.hpp"
#include "custom_query.h"
#include "customqueryresult.h"
#include "customquerysettings.h"
#include "ui_customquerysettings.h"

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

signals:

    void db_show();

    //void canceled();

    void custom_query(QString);

private slots:


    void on_showDB_button_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_select_from_table_button_clicked();

//    void on_pushButton_clicked();

    void on_Custom_Query_Button_clicked();

    //void cancel_slot();

    void on_Query_settings_clicked();

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

};

#endif // TABLES_H
