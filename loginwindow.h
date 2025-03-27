#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QScreen>
#include <QCloseEvent>
#include <QToolTip>
#include <QPainter>
#include <QtMath>
#include <QCompleter>

#include "databases.h"
#include "tables.h"
#include "auth.h"
#include "db_connection.h"
#include "blinkinbutton.h"
#include "iqtplugins.h"
#include "passwordlineedit.h"

#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class loginWindow; }
QT_END_NAMESPACE

//class AdbCrypt;

#define _TIMEOUT_RECON_DFLT_ 30000
#define _CON_LOST_MSG_DFLT_ true

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    loginWindow(QWidget *parent = nullptr);
    ~loginWindow();


public slots:

    // slot for timer (reconnect-disconnect from server info)
    void connection_timer_slot();

    // initialization of settings dialog
    void gset_connection_options();

signals:

    void message_to_database_window(QString const&);

    void start_connection_timer_stuff();

    void reconnect_data_changed();

    void setCustomSettings();

    void rememberMeSig();

private slots:

    // connect to SQL and explore DBs
    void on_pushButton_clicked();

    // password hide
//    void on_checkBox_stateChanged(int arg1);

    // .cfg file synchronize
    void write2recon_opts_file();

    // .cfg file synchronize(2)
    bool read4rom_recon_opts_file();

    // get login list from cryptoModule by mouse
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::loginWindow *ui;

    QCheckBox* rememberCB_ = nullptr;

    PasswordLineEdit* passwLine_ = nullptr;

    // user info struct declare
    auth auth_;

    //[ .cfg file variables
    QString const config_f_name = adb_utility::filepath_+"/rec.cfg";

    //30 sec
    int timeout_reconnect = _TIMEOUT_RECON_DFLT_;

    bool CONNECTION_LOST_MESSAGE = _CON_LOST_MSG_DFLT_;
    //]

    // window initialization (.ui design file+code)
    void form_init();

    void signals_init();

    void fileOps();

    void cryptoModuleInit();


    // child (after login -->> main window in ierarchy, this->hide)
    Databases* db_window_ = nullptr;

    QPointer<QTimer> timer;

};
#endif // LOGINWINDOW_H


