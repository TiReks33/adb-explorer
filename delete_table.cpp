#include "delete_table.h"

delete_table::delete_table(QWidget *parent) :
    delete_form(parent,"table")

{
    ////ui->label->setText("Choose table to <font style=\"color: black;background: red;\">delete</font>");

}


void delete_table::on_buttonBox_accepted()
{
    delete_form::on_buttonBox_accepted();

}
