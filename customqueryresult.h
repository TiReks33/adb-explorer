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

    void custom_query_slot(QString);


private slots:
    void on_Cancel_button_clicked();

private:
    Ui::CustomQueryResult *ui;

    auth& auth_;

    QSqlQueryModel model_;
};

#endif // CUSTOMQUERYRESULT_H
