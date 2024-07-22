#ifndef CREATE_TABLE_CONSTRUCTOR_H
#define CREATE_TABLE_CONSTRUCTOR_H

#include <QStackedWidget>
#include "QDebug"
#include <QString>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QList>
#include <QSqlError>

#include "customqueryresult.h"
#include "ui_customqueryresult.h"
#include "db_connection.h"

namespace Ui {
class CreateTableConstructor;
}

class CreateTableConstructor : public QStackedWidget
{
    Q_OBJECT

public:

    explicit CreateTableConstructor(auth& auth__,QWidget *parent = nullptr);
    ~CreateTableConstructor();

    bool decimal_type_more_or_eq();

    bool add_attributes(QPlainTextEdit*);

    bool add_keys(QPlainTextEdit*);

public slots:

    void AttrTypeChanged(QString const &);

    void foreignkeychecked(int);

    void onDeleteChecked(int);

    void onUpdateChecked(int);

//    void add_tbl_constructor_db2table_slot(int);

    void add_tbl_constructor_db2table_slot(QString const&);

    void add_tbl_constructor_table2atribute_slot(QString const&);

//    void close_con(QString const &);

    void closeEvent(QCloseEvent *event);

    void current_exist_tables_slot(QList<QString>);

    void constructor_query_fails_handle();

    void erase();

private slots:
    void on_next_0_clicked();

    void on_next_1_clicked();

    void on_send_button_clicked();

    void on_plus_button_1_clicked();

    void on_erase_button_1_clicked();

    void on_help_button_2_clicked();

    void on_reset_button_2_clicked();

    void on_plus_button_2_clicked();

    void on_describe_tbl_button_2_clicked();

    void on_cancel_2_clicked();

    void on_back_button_2_clicked();

    void on_back_button_1_clicked();

    void on_cancel_0_clicked();

signals:

    void send_custom_query(QString);

private:
    Ui::CreateTableConstructor *ui;
    bool first_attribute_;
    QString sql_query_;
    bool first_key_;
    QList<QString> attributes_;

    auth& auth_;
    auth auth_autonome_;

    db_connection* non_static_connection_;
//    db_connection* non_static_connection_2_;

//    QSqlQueryModel model_;
    QSqlQueryModel submodel_1_;
    QSqlQueryModel submodel_2_;

    QList<QString> non_dflt_conction_names_;

    QList<QString> exist_table_names_;

    int window_counter_=0;

    int attributes_added_=0;

};

#endif // CREATE_TABLE_CONSTRUCTOR_H
