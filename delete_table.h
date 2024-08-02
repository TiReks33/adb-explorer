#ifndef DELETE_TABLE_H
#define DELETE_TABLE_H

//#include "delete_db.h"
//#include "ui_delete_db.h"
#include "delete_form.h"

#include <QObject>
#include <QDebug>

//namespace Ui {
//class delete_db;
//}

class delete_table : public delete_form
{
    Q_OBJECT
public:
    explicit delete_table(QWidget *parent = nullptr);
//    virtual ~delete_table();

//signals:
//    //void delete_table_sig(QComboBox*);

//public slots:
//    void delete_form_request_slot();

//private slots:
protected slots:
    void on_buttonBox_accepted();
};

#endif // DELETE_TABLE_H
