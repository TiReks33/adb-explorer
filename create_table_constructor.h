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
#include <QPointer>

#include "customqueryresult.h"
#include "ui_customqueryresult.h"
#include "db_connection.h"
#include "adb-exp-utility.h"
#include "scrolledstatusbar.h"
#include "reloadbutton.h"


//#include "tables.h"
class Tables;


namespace Ui {
class CreateTableConstructor;
}

class CreateTableConstructor : public QStackedWidget
{
    Q_OBJECT

public:

    explicit CreateTableConstructor(auth& auth__,/*QWidget*/Tables *parent = nullptr);
    ~CreateTableConstructor();

    // SQL decimal validator (is int part of number more or equal than float part?)
    bool decimal_type_more_or_eq();

    // add corteges types in table
    bool add_attributes(QPlainTextEdit*);

    // add foreign keys of table
    bool add_keys(QPlainTextEdit*);

public slots:

    // show/hide buttons depending on selected attribute type
    void AttrTypeChanged(QString const &);

    // form's button highlight switcher
    void foreignkeychecked(int);

    void closeEvent(QCloseEvent *event);

    // unique table name checking
    void current_exist_tables_slot(QList<QString>);

    void constructor_query_fails_handle();

    // erase current form data in next<->back switch
    void erase();

    // warning flag for closeEvent()
    void set_warning_flag_(bool condition){_warning_flag_=condition;}

private slots:
    // 1st next
    void on_next_0_clicked();

    // table description if foreign key form
    void describe_table();

    // reload current connection
    void reloadDatabaseComboBox();

    void /*QDialog **/ get_help_window(QPointer<QDialog>&,QString const&,QString const&,QWidget* = nullptr);

signals:

    void send_custom_query(QString const& query_text__);

    void send_custom_query(QString const& query_text__,QString const& newtable_name__);

    void closed();

private:
    Ui::CreateTableConstructor *ui;

    scrolledStatusBar* statusBar_0;
    scrolledStatusBar* statusBar_1;
    scrolledStatusBar* statusBar_2;

    void signals_init();

    void initForm();

    bool first_attribute_;
    bool first_key_;
    QList<QString> attributes_;

    auth& auth_;

    // foreign key form's sql query models[
    QSqlQueryModel submodel_0_;
    QSqlQueryModel submodel_1_;
    QSqlQueryModel submodel_2_;
    //]

    QList<QString> exist_table_names_;



    int attributes_added_=0;

    bool _warning_flag_=true;



    Tables *parent_;


    //QPointer<CustomQueryResult> describe_form_;
    QScopedPointer<CustomQueryResult> describe_form_;

    QString subconnection_name_ = "CreateTableConstructor_subconnection";

    QString subconnection_name_2_ = "CreateTableConstructor_subconnection2";

//    QString const en_lit = "a-zA-Z";

//    QString const digits_lit = "0-9";

//    QString const spec_chars_lit = "_$";

    //auth* __auth;

    QString current_t_name_ = "";

    reloadButton* reload_con_button_2 = nullptr;

};

#endif // CREATE_TABLE_CONSTRUCTOR_H
