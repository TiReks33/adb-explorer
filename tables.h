#ifndef TABLES_H
#define TABLES_H

#include <QDialog>
#include <QStatusBar>
#include <QSqlDatabase>
#include "auth.h"

namespace Ui {
class Tables;
}

class Tables : public QDialog
{
    Q_OBJECT

public:
    explicit Tables(auth&,QWidget *parent = nullptr);

    ~Tables();


public slots:


signals:


private slots:


private:
    Ui::Tables *ui;

//    QString db_server_;
//    QString db_login_;
//    QString db_passw_;
//    QString db_host_;
    auth& auth_;
};

#endif // TABLES_H
