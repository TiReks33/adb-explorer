#ifndef CUSTOMQUERYRESULT_H
#define CUSTOMQUERYRESULT_H

#include <QDialog>

#include "auth.h"
#include "db_connection.h"

namespace Ui {
class CustomQueryResult;
}

class CustomQueryResult : public QDialog
{
    Q_OBJECT

public:
    explicit CustomQueryResult(auth& auth__,QWidget *parent = nullptr);
    ~CustomQueryResult();

public slots:

    friend class Tables;
    friend class CreateTableConstructor;

    void custom_query_slot(QString);

//    void custom_query_slot(QString,QTableView*);

    void custom_query_slot(QString,QTableView*);

    void custom_query_slot(QString,QTableView*,QString);

//    void custom_query_slot(QString,QSqlQueryModel,QTableView*);

    void custom_query_slot(QString,QComboBox*);

private slots:
    void on_Cancel_button_clicked();

private:
    Ui::CustomQueryResult *ui;

    auth& auth_;

     QSqlQueryModel model_;

    // db_connection* non_static_connection_;

    // QSqlQueryModel sub_model_;
};

#endif // CUSTOMQUERYRESULT_H
