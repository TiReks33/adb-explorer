#ifndef CREATE_TABLE_CONSTRUCTOR_H
#define CREATE_TABLE_CONSTRUCTOR_H

#include <QStackedWidget>
#include "QDebug"
#include <QString>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QList>
#include <QSqlError>
#include <QScrollArea>

#include "customqueryresult.h"
#include "ui_customqueryresult.h"
#include "db_connection.h"
#include "helping_stuff.h"
#include "multi_connection.h"
#include "QPointer"

//#include "tables.h"
class Tables;
//enum class safe_close
//{
//    no_ = 0,
//    yes_ = 1
//};


namespace Ui {
class CreateTableConstructor;
}

class CreateTableConstructor : public QStackedWidget
{
    Q_OBJECT

public:

    explicit CreateTableConstructor(auth& auth__,/*QWidget*/Tables *parent = nullptr);
    ~CreateTableConstructor();

    bool decimal_type_more_or_eq();

    bool add_attributes(QPlainTextEdit*);

    bool add_keys(QPlainTextEdit*);

public slots:

    void AttrTypeChanged(QString const &);

    void foreignkeychecked(int);

    void closeEvent(QCloseEvent *event);

    void current_exist_tables_slot(QList<QString>);

    void constructor_query_fails_handle();

    void erase();

    void set_warning_flag_(bool condition){_warning_flag_=condition;}

private slots:
    void on_next_0_clicked();

    void describe_table();

    void on_reload_con_button_2_clicked();

    void get_help_window(QString const&,QString const&,QWidget* = nullptr);

    void /*QDialog **/ get_help_window(QPointer<QDialog>&,QString const&,QString const&,QWidget* = nullptr);

signals:

    void send_custom_query(QString const&);

    void closed();

private:
    Ui::CreateTableConstructor *ui;

    void signals_init();

    bool first_attribute_;
    QString sql_query_;
    bool first_key_;
    QList<QString> attributes_;

    auth& auth_;
    auth auth_autonome_;


    QSqlQueryModel submodel_0_;
    QSqlQueryModel submodel_1_;
    QSqlQueryModel submodel_2_;

    QList<QString> non_dflt_conction_names_;

    QList<QString> exist_table_names_;



    int attributes_added_=0;

    bool _warning_flag_=true;



    Tables *parent_;


    //QPointer<CustomQueryResult> describe_form_;
    QScopedPointer<CustomQueryResult> describe_form_;

    QString subconnection_name_ = "CreateTableConstructor_subconnection";

    QString subconnection_name_2_ = "CreateTableConstructor_subconnection2";

    QString const en_lit = "a-zA-Z";

    QString const digits_lit = "0-9";

    QString const spec_chars_lit = "_$";

    //auth* __auth;

};

#endif // CREATE_TABLE_CONSTRUCTOR_H
