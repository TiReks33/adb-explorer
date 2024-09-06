#ifndef CUSTOMQUERYRESULT_H
#define CUSTOMQUERYRESULT_H

#include <QDialog>
#include <QCloseEvent>

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

signals:

    void closed_();

private slots:

    void close_window();

    inline void closeEvent(QCloseEvent *event){ emit closed_(); event->accept();};

private:
    Ui::CustomQueryResult *ui;
public:
    auth& auth_;

    QSqlQueryModel model_;


};

#endif // CUSTOMQUERYRESULT_H
