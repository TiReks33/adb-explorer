#ifndef DELETE_DB_H
#define DELETE_DB_H

#include <QDialog>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>

#include "delete_form.h"
#include "ui_delete_form.h"

namespace Ui {
class delete_db;
}

class delete_db : public delete_form
{
    Q_OBJECT

public:
    explicit delete_db(QWidget *parent = nullptr);
//    virtual ~delete_db();

//signals:
//    void delete_form_send(QComboBox*);

//    void delete_database(QComboBox*);


//public slots:
////    void delete_form_request_slot();

//private slots:
protected slots:
    void on_buttonBox_accepted();

////private:
//protected:
//    Ui::delete_db *ui;
};

#endif // DELETE_DB_H
