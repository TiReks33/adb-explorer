#ifndef CREATETUPLECONSTRUCTOR_H
#define CREATETUPLECONSTRUCTOR_H

#include "inserttupleconfirm.h"

#include <QDialog>
#include "customqueryresult.h"
#include "ui_customqueryresult.h"
#include "twolistselection.h"
#include "helping_stuff.h"
#include "tables.h"

namespace Ui {
class createTupleConstructor;
}

class createTupleConstructor : public QDialog
{
    Q_OBJECT

public:
    explicit createTupleConstructor(auth& auth__,QWidget *parent = nullptr);
    ~createTupleConstructor();

    void reset();

    void show();

private slots:

    void update_tables_list();

//    void on_reset_button_clicked();

//    void columns_selected_handler();

//    void on_addColsButton_clicked();

    void closeEvent(QCloseEvent *event);

    void on_addTupleButton_clicked();

    void on_describeButton_clicked();

//public slots:

//    void import_list(QStringList);

signals:

    void final_query_sig(QString const&);

    void closed();

    void closed(QString const &con_name_that_mustBclosed);

private:
    Ui::createTupleConstructor *ui;

    void signals_init();

    auth& auth_;


    QSqlQueryModel tables_model_;


    int tuples_added_=0;
    QStringList tuples_;
    QPointer<CustomQueryResult> describe_form_;

//    QString subconnection_name_ = "createTupleConstructor_subconnection";
};

#endif // CREATETUPLECONSTRUCTOR_H
