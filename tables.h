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
//#include "createtupleconstructor.h"

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

    void send_custom_query_slot(/*QString,*/Custom_Query*);



    void delete_table_slot(QComboBox*);


    void show_table_describe_form(QString const & db_name__,QString const&table_name__,QString const & con_name__ = auth::con_name_, QWidget * parent__ = nullptr, Qt::WindowType window_type_flag__ = Qt::Dialog, Qt::WindowModality window_modality_flag__ = Qt::NonModal) const;



signals:

    void db_show();



    void custom_query(auth&,QString);



    void custom_query(auth&, QString,QTableView*);

    void custom_query(QString,QSqlQueryModel&,QTableView*);

    void delete_form_request();



    void current_tables_list_signal(QList<QString>);

    void close_custom_query_form();

    void constructor_query_success();

    void constructor_query_fails();

    void tpl_cnstr_upd_tables();

    void custom_query_windows_close(); // 'close/*_stack*/_childs?

private slots:


    void on_showTable_button_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_select_from_table_button_clicked();



    void on_Custom_Query_Button_clicked();


    void on_Query_settings_clicked();

    void on_delete_table_button_clicked();

    void on_create_table_button_clicked();

    void on_insert_inTable_button_clicked();

    void get_custom_query_window_();

    void get_custom_query_window_(QString);

    void on_pushButton_2_clicked();

protected:
    bool event(QEvent* event);

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
