#ifndef DELETE_TABLE_H
#define DELETE_TABLE_H

#include "delete_db.h"
#include "ui_delete_db.h"

#include <QObject>
#include <QDebug>

//namespace Ui {
//class delete_db;
//}

class delete_table : public delete_db
{
    Q_OBJECT
public:
    delete_table();

signals:
    void delete_table_sig(QComboBox*);

public slots:
//    void delete_form_request_slot();

//private slots:
protected slots:
    void on_buttonBox_accepted();
};

#endif // DELETE_TABLE_H
