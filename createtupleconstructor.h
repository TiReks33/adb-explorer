#ifndef CREATETUPLECONSTRUCTOR_H
#define CREATETUPLECONSTRUCTOR_H

#include <QDialog>
#include "inserttupleconfirm.h"
//#include "auth.h"
#include "customqueryresult.h"
#include "ui_customqueryresult.h"
#include "twolistselection.h"

namespace Ui {
class createTupleConstructor;
}

class createTupleConstructor : public QDialog
{
    Q_OBJECT

public:
    explicit createTupleConstructor(auth& auth__,QWidget *parent = nullptr);
    ~createTupleConstructor();

private slots:
    void on_okButton_clicked();

    void on_update_tables_button_clicked();

    void on_reset_button_clicked();

    void table_changed_handler(QString const&);

    void on_addColsButton_clicked();

    void closeEvent(QCloseEvent *event);

public slots:
    void update_tables_handler();

private:
    Ui::createTupleConstructor *ui;
    auth& auth_;
    insertTupleConfirm* confirm_window_;

//    db_connection* non_static_connection_;

    TwoListSelection* list_selection_window_;

    // connection-query solution to multiply signals-->
    static QStringList multi_con_names_;

//    static QList<QSqlQueryModel*> multi_con_models_;
    QSqlQueryModel tables_model_;
    QString con_name_;

    static int con_counter_;
    static int unique_number_;
//    inline static int con_counter_ = 1;
    //<<-
};

#endif // CREATETUPLECONSTRUCTOR_H
