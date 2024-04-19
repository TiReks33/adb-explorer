#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include "databases.h"

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

    void connection_close(){ /*db_connection_.close();*/ QSqlDatabase::database().close(); }

    bool connection_open(QString,QString,QString="localhost");







    QString get_db_name(){return db_name_;}
    QString get_host(){return host_;};
    QString get_login(){return login_;}
    QString get_passw(){return passw_;}
    //QSqlDatabase get_cur_connection_(){return db_connection_;}

public slots:

    void test_slot();

    void receive_login_(QString&);
    void receive_passw_(QString&);

signals:
    void message_to_database(QString);

    void request_login_(QString&);
    void request_passw_(QString&);

    void send_auth(QString,QString,QString="localhost");

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_login_testButton_clicked();

private:
    Ui::loginWindow *ui;
    //QSqlDatabase db_connection_;
    Databases* db_window_;
    QString db_server_;
    QString db_name_;
    QString host_;
    QString login_; // NOMINAL MEMBER
    QString passw_; // NOMINAL MEMBER

};
#endif // LOGINWINDOW_H
