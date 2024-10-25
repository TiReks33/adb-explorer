#ifndef DELETE_TABLE_H
#define DELETE_TABLE_H


#include "delete_form.h"

#include <QObject>
#include <QDebug>

// this class inherits pure virtual to delete entities in 'table' form
class delete_table : public delete_form
{
    Q_OBJECT
public:
    explicit delete_table(QWidget *parent = nullptr);


//private slots:
protected slots:
    void on_buttonBox_accepted();
};

#endif // DELETE_TABLE_H
