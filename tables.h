﻿#ifndef TABLES_H
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
#include <QStandardItemModel>
#include <QCheckBox>
#include <QLabel>
#include <QGraphicsEffect>
#include <QElapsedTimer>

#include "auth.h"
#include "db_connection.h"
#include "select_cells.hpp"
#include "custom_query.h"
#include "customqueryresult.h"
#include "customquerysettings.h"
#include "ui_customquerysettings.h"
#include "delete_db.h"
#include "delete_table.h"
#include "create_table_constructor.h"
#include <customqueryresult.h>
#include <ui_customqueryresult.h>

#include "blinkinbutton.h"

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

//    void send_custom_query_slot(QString const&);

    void send_custom_query_slot(/*QString,*/Custom_Query *);


    void show_table_describe_form(
            QString const & db_name__,
            QString const&table_name__,
            QString const & con_name__ = auth::con_name_,
            QWidget * parent__ = nullptr,
            Qt::WindowType window_type_flag__ = Qt::Dialog,
            Qt::WindowModality window_modality_flag__ = Qt::NonModal
            ) const;

    void get_information_window(QString const&,QString const&,QWidget* = nullptr);


signals:

    void db_show();



    void custom_query(auth&,QString const&);



    void custom_query(auth&, QString const&,QTableView*);

    void custom_query(QString const&,QSqlQueryModel&,QTableView*);

    void delete_form_request();



    void current_tables_list_signal(QList<QString>);

    void close_custom_query_form();

    void constructor_query_success();

    void constructor_query_fails();

    void tpl_cnstr_upd_tables();

    void custom_query_windows_close(); // 'close/*_stack*/_childs?

    void tables_reloaded();

    void disable_select_until_reload();

    void empty_set();

private slots:

    void get_custom_query_window_();

    void get_custom_query_window_(QString const&);

    void show_table_content();

    void get_table_constructor();

    void get_tuple_constructor_instance();

    void /*get_describe_table_instance*/show_table_description();

protected:
//    bool event(QEvent* event);

private:
    Ui::Tables *ui;

    BlinkinButton* showTable_button;

    void init_connections();

//    void disable_select_until_reload();

    auth& auth_;

    QSqlQueryModel model_;





    CustomQueryResult* custom_query_result_window_;

    CustomQuerySettings* settings_;

    delete_table* delete_table_window_;

    CreateTableConstructor* constructor_;



    int tuples_windows_counter_=0;



    inline void keyPressEvent(QKeyEvent *e) {
        if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {/* minimize */
            qDebug()<<"escape pressed (tables)";
            close();
        }
    }



};

#endif // TABLES_H
