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


protected slots:
    void on_buttonBox_accepted();


};

#endif // DELETE_DB_H
