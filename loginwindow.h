#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QScreen>
#include <QCloseEvent>
#include <QToolTip>

#include "databases.h"
#include "tables.h"
#include "auth.h"
#include "db_connection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class loginWindow; }
QT_END_NAMESPACE


class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    loginWindow(QWidget *parent = nullptr);
    ~loginWindow();

    // close-event override for save/db closing connection-purpose after app closing
    void closeEvent(QCloseEvent *event);


//    QString get_db_name(){return auth_.db_name_;}
//    QString get_host(){return auth_.host_;};
//    QString get_login(){return auth_.login_;}
//    QString get_passw(){return auth_.passw_;}

public slots:

    void test_slot();

    void receive_login_(QString&);
    void receive_passw_(QString&);

signals:
    void message_to_database(QString);

    void request_login_(QString&);
    void request_passw_(QString&);

    void send_auth(auth&);

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

//    void on_login_testButton_clicked();

private:
    Ui::loginWindow *ui;
    //QSqlDatabase db_connection_;
    Databases* db_window_;


    auth auth_;

    //Tables* tables_window_;

};
#endif // LOGINWINDOW_H
