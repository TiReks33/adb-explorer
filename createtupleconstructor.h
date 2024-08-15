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

private slots:
    void on_okButton_clicked();

    void on_update_tables_button_clicked();

    void on_reset_button_clicked();

    void table_changed_handler(QString const&);

    void columns_selected_handler();

    void on_addColsButton_clicked();

    void closeEvent(QCloseEvent *event);

    void on_addTupleButton_clicked();

    void on_describeButton_clicked();

public slots:
    void /*update_tables_handler*/sql_connection_initialize(); // because qt meta-object method restriction in constructor

    void import_list(QStringList);

signals:

    void final_query_sig(QString);

    void closed();

private:
    Ui::createTupleConstructor *ui;
    auth& auth_;
    ////insertTupleConfirm* confirm_window_;

//    db_connection* non_static_connection_;

                                                    //TwoListSelection* list_selection_window_;

    // connection-query solution to multiply signals-->
//    static QStringList multi_con_names_;

//    static QList<QSqlQueryModel*> multi_con_models_;
    QSqlQueryModel tables_model_;
//    QString con_name_;
    multi_connection multi_con_;
//    static int con_counter_;
//    static int unique_number_;
//    inline static int con_counter_ = 1;
    //<<-
    int tuples_added_=0;
    QStringList tuples_;
    QPointer<CustomQueryResult> describe_form_;
};

#endif // CREATETUPLECONSTRUCTOR_H
