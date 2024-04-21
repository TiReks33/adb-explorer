#ifndef DATABASES_H
#define DATABASES_H

#include <QDialog>
#include <QStatusBar>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
//#include <loginwindow.h>
#include "tables.h"

namespace Ui {
class Databases;
}

class Databases : public QDialog
{
    Q_OBJECT

public:
    explicit Databases(QWidget *parent = nullptr);
//             Databases(QSqlDatabase, QWidget *parent = nullptr);
    ~Databases();


             void close_DB();

public slots:
    void message_from_login(QString);

    void receive_auth(QString,QString,QString);

signals:
    void test_signal();

private slots:
    void on_showDB_button_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Databases *ui;
    //QStatusBar* status_bar_;
    //QSqlDatabase db_connection_;
    QString db_server_;
    QString db_login_;
    QString db_passw_;
    QString db_host_;
    Tables* tables_window_;

};

#endif // DATABASES_H
