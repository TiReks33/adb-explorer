#ifndef CREATETUPLECONSTRUCTOR_H
#define CREATETUPLECONSTRUCTOR_H

////#include "inserttupleconfirm.h"

#include <QDialog>
#include "customqueryresult.h"
#include "ui_customqueryresult.h"
#include "twolistselection.h"
#include "helping_stuff.h"
#include "tables.h"

#include "scrolledstatusbar.h"

namespace Ui {
class createTupleConstructor;
}

class createTupleConstructor : public QWidget/*QDialog*/
{
    Q_OBJECT

public:
    explicit createTupleConstructor(auth& auth__,QWidget *parent = nullptr);
    ~createTupleConstructor();

    // clear all entered data from form
    void reset();

    // overload (show window + tables list query exec)
    void show();

private slots:

    void update_tables_list();

    void closeEvent(QCloseEvent *event);

    // add value to table
    void on_addTupleButton_clicked();

    // describe table
    void on_describeButton_clicked();

signals:

    void final_query_sig(QString const&);

    void closed();

    void closed(QString const &con_name_that_mustBclosed);

private:
    Ui::createTupleConstructor *ui;

    scrolledStatusBar* statusBar;

    void signals_init();

    auth& auth_;


    QSqlQueryModel tables_model_;


    int tuples_added_=0;
    QStringList tuples_;
    QPointer<CustomQueryResult> describe_form_;

//    QString subconnection_name_ = "createTupleConstructor_subconnection";
};

#endif // CREATETUPLECONSTRUCTOR_H
