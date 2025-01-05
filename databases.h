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
#include <QProcess>
#include <QVariant>
#include <QElapsedTimer>
#include <QScreen>
#include <QMenuBar>

#include "tables.h"
#include "auth.h"
#include "select_cells.hpp"
#include "db_connection.h"
#include "create_db_name.h"
#include "delete_db.h"
#include "adb-exp-utility.h"
#include "sqldump.h"
#include "clickablelabel.h"
#include "scrolledstatusbar.h"
#include "signaltableview.h"
#include "notifycombobox.h"
#include "delete_sqldb_user.h"
#include "sqlusermanagement.h"
#include "fontembeddedwidget.h"
#include "reloadbutton.h"
#include "hidemenu.h"

#include <iostream>


namespace Ui {
class Databases;
}

class Databases : public QDialog
{
    Q_OBJECT

public:
    // constructor requires credentials ref
    explicit Databases(auth&, QWidget *parent = nullptr);
    ~Databases();



public slots:

    // set message to status-bar
    void message_to_status(QString const &) const;

    // load-reload databases list on form
    bool show_databases();

//    // [utility func]
//    void get_information_window(QString const&,QString const&, QWidget* = nullptr, enum QMessageBox::Icon messageBoxType__ = QMessageBox::Information);

    void createUser();

    void grantUserPermissionsButtonHandler();

    void deleteUser();

    void getUsersList();

    void getPasswordMgmtForm();

signals:

    void select_cells_signal(const QModelIndex &,const QModelIndex &);

    void show_tables_signal();

    void delete_form_request();

    void close_all_custom_windows_();

    void window_rise_signal();

private slots:

    // allocate and call mysqldump object
    void on_mysqldump_button_clicked();

    // form to send custom SQL queries to server
    void get_query_wndw();

    // call window with result of custom query
    void send_custom_query_slot(Custom_Query*);

private:
    Ui::Databases *ui;

    signalTableView* tableView;

    reloadButton* showDB_button = nullptr;

    scrolledStatusBar* statusBar;

    void init_signals();

    void init_form();

    void fileOps();

    void defaultSettings();

    // user credentials struct ref
    auth& auth_;

    // child
    Tables* tables_window_ = nullptr;

    // model to place queries
    QSqlQueryModel model_;

    // additional window
    create_db_name* new_db_window_;

    // additional window(2)
    delete_db* delete_db_window_;

    // keyboard keys logic overload
    void keyPressEvent(QKeyEvent *);


    // quit from app
    void closeEvent(QCloseEvent *event);

    // additional connection name for queries to SQL server via 'custom query' form
    QString const subconnection_name_="Database::custom_query_connection";

    // flag for closing all child non_modal windows after database chosing;
    bool close_chld_wndws_on_next = true;

    QString const settings_f_name_ = adb_utility ::filepath_+"/databases.cfg";

    void showSettings();

    bool read4rom_settings_file();

    void write2_settings_file();

    bool query2server_note=true;

    void mousePressEvent(QMouseEvent * event);

    // rescale stuff
    QWidget* rescaleBoxWidget;
    ////bool tableScaleChanged = false;
    QPointer<QCheckBox>rescaleDefaultCheckBox;
    static int defaultScaleIndex_;


    QPushButton* createUserButton_;
    QPushButton* getUsersListButton_;

    QPushButton* grantPermissionsButton_;
    QPushButton* changePassButton_;

    // utility funcs for grant permissions form
    QWidget* getGrantPermissionsWidget(QWidget* parent__ = nullptr);
    bool grantPermissions2user(QString const&,QStringList const,/*QString const&,*/bool GRANT_OPT=false);

    QPushButton* deleteUserButton_ = nullptr;
    QPushButton* quitButton_ = nullptr;

    QPointer<fontEmbeddedWidget> fontWidget_ = nullptr;
    static QString defaultFont_;

    QMenuBar* menuBar_ = nullptr;
    /*QMenu*/hideMenu* menuFile_ = nullptr;
    QAction* exitEntrie_ = nullptr;

    //QSpinBox* fontPointSizeSpin_ = nullptr;
};

#endif // DATABASES_H
