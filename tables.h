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
#include <QScreen>
#include <QMenuBar>

#include "auth.h"
#include "db_connection.h"
#include "select_cells.hpp"
//#include "custom_query.h"
#include "customqueryresult.h"
#include "customquerysettings.h"
#include "ui_customquerysettings.h"
#include "delete_db.h"
#include "delete_table.h"
#include "create_table_constructor.h"
#include <customqueryresult.h>
#include <ui_customqueryresult.h>
#include "reloadbutton.h"
#include "scrolledstatusbar.h"
#include "signaltableview.h"
#include "fontembeddedwidget.h"
#include "hidemenu.h"

class Custom_Query;

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

    // load/reload tables list
    void show_tables();

    // send result of query from Custom_Query form in 1) separate or 2) main form of 'tables' window
    void send_custom_query_slot(/*QString,*/Custom_Query *);


signals:

    void db_show();


    void custom_query(auth&,QString const&);


    void custom_query(auth&, QString const&,QTableView*);

    void custom_query(QString const&,QSqlQueryModel&,QTableView*);

    void delete_form_request();


    void current_tables_list_signal(QList<QString>);

//    void constructor_query_success();
    void constructor_query_success(QString const&);

//    void constructor_query_fails();
    void constructor_query_fails(QString const&);

    void tpl_cnstr_upd_tables();

    void custom_query_windows_close(); // 'close/*_stack*/_childs?

    void tables_reloaded();

    void disable_select_until_reload();

    void empty_set();


private slots:


    // form for user custom query (pre-added text optional)
    void get_custom_query_window_(QString const& = "", bool closeMessage=false);

    // SELECT TABLE
    void show_table_content();

    // exec. table interactive constructor
    void get_table_constructor();

    // exec. interactive constructor for adding values to table
    void get_tuple_constructor_instance();

    // DESCRIBE TABLE
    void show_table_description();

public:

    bool separate_content_window = true;

    bool separate_describe_window = true;

    bool separate_query_window = true;
    bool showNoteIfNotSeparateWindowResult = true;

    bool BLANK_RESULT = false;

    bool MSG_SHOW_IF_BLANK_RESULT = false;

    bool MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL = false;
    bool queryFailNote = true;

private:

    // imply data changes from 'CustomQuerySettings' form
    void importSettings(QMap<QString,int>, QMap<QString,QString> = QMap<QString,QString>{/*EMPTY QMAP BY DEFAULT*/});

private:
    Ui::Tables *ui;

    signalTableView* tableView = nullptr;

    scrolledStatusBar* statusBar = nullptr;

    QString const settings_f_name_ = adb_utility ::filepath_+"/tablesQuery.cfg";


    reloadButton* showTable_button = nullptr;

    void init_connections();

    void init_form();

    void fileOps();

    void defaultSettings();

    auth& auth_;

    QSqlQueryModel model_;


    QPointer<CustomQuerySettings> settings_;

    delete_table* delete_table_window_ = nullptr;

    CreateTableConstructor* constructor_ = nullptr;


    // preventing application to exit by escape (QDialog close by esc)
    void keyPressEvent(QKeyEvent *e);


    bool read4rom_settings_file();

    void write2_settings_file();

    void mousePressEvent(QMouseEvent* event);

    // rescale stuff
    QWidget* rescaleBoxWidget = nullptr;
    QPointer<QCheckBox>rescaleDefaultCheckBox;
    static int defaultScaleIndex_;

    QPushButton* backButton_ = nullptr;

    QPointer<fontEmbeddedWidget> fontWidget_;
    static QString defaultFont_;

    QMenuBar* menuBar_ = nullptr;
    hideMenu* menuFile_ = nullptr;
    QAction* exitEntrie_ = nullptr;
    QAction* prevEntrie_ = nullptr;

};

#endif // TABLES_H
