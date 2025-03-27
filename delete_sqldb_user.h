#ifndef DELETE_SQLDB_USER_H
#define DELETE_SQLDB_USER_H

#include "delete_form.h"

class delete_sqldb_user : public delete_form
{
public:
    explicit delete_sqldb_user(QWidget *parent = nullptr);

    virtual void show();

protected slots:
    void on_buttonBox_accepted();
};

#endif // DELETE_SQLDB_USER_H
