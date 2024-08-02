#include "delete_table.h"

delete_table::delete_table(QWidget *parent) :
    delete_form(parent,"table")
//   , ui(new Ui::delete_table)
{
    ui->label->setText("Choose table to <font style=\"color: black;background: red;\">delete</font>");

    //connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(on_buttonBox_accepted()));
}

//void delete_table::delete_form_request_slot()
//{
//    qDebug() << "TEST TEST TEST TEST";
//}

void delete_table::on_buttonBox_accepted()
{
    delete_form::on_buttonBox_accepted();

}
