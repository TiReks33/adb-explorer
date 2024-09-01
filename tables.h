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

    void send_custom_query_slot(QString const&);

    void send_custom_query_slot(/*QString,*/Custom_Query *);


    void show_table_describe_form(
            QString const & db_name__,
            QString const&table_name__,
            QString const & con_name__ = auth::con_name_,
            QWidget * parent__ = nullptr,
            Qt::WindowType window_type_flag__ = Qt::Dialog,
            Qt::WindowModality window_modality_flag__ = Qt::NonModal
            ) const;



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

private slots:

    void get_custom_query_window_();

    void get_custom_query_window_(QString const&);

    void show_table_content();

    void get_table_constructor();

    void get_tuple_constructor_instance();

    void get_describe_table_instance();

protected:
//    bool event(QEvent* event);

private:
    Ui::Tables *ui;

    void init_connections();


    auth& auth_;

    QSqlQueryModel model_;





    CustomQueryResult* custom_query_result_window_;

    CustomQuerySettings* settings_;

    delete_table* delete_table_window_;

    CreateTableConstructor* constructor_;



    int tuples_windows_counter_=0;

};

#endif // TABLES_H
