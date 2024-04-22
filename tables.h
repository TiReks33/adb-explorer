#ifndef TABLES_H
#define TABLES_H

#include <QDialog>
#include <QStatusBar>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QCloseEvent>

#include "auth.h"
#include "db_connection.h"

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

signals:

    void db_show();

private slots:


    void on_showDB_button_clicked();

private:
    Ui::Tables *ui;

//    QString db_server_;
//    QString db_login_;
//    QString db_passw_;
//    QString db_host_;
    auth& auth_;

    QSqlQueryModel model_;
};

#endif // TABLES_H
