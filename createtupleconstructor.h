#ifndef CREATETUPLECONSTRUCTOR_H
#define CREATETUPLECONSTRUCTOR_H

#include <QDialog>
#include "inserttupleconfirm.h"
//#include "auth.h"
#include "customqueryresult.h"
#include "ui_customqueryresult.h"

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

public slots:
    void update_tables_handler();

private:
    Ui::createTupleConstructor *ui;
    auth& auth_;
    insertTupleConfirm* confirm_window_;

    db_connection* non_static_connection_;

};

#endif // CREATETUPLECONSTRUCTOR_H
