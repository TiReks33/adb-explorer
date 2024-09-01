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


    void custom_query_slot(QString const & query__, QString const & = auth::con_name_, QHeaderView::ResizeMode = QHeaderView::Stretch);



private slots:

    void close_window();



private:
    Ui::CustomQueryResult *ui;

    auth& auth_;

     QSqlQueryModel model_;


};

#endif // CUSTOMQUERYRESULT_H
