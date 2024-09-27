#include "delete_db.h"

delete_db::delete_db(QWidget *parent) :
    delete_form(parent,"database")

{

    ////ui->label->setText("Choose database to <font style=\"color: black;background: red;\">delete</font>");

}





void delete_db::on_buttonBox_accepted()
{

    delete_form::on_buttonBox_accepted();
}
