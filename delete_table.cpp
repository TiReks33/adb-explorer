#include "delete_table.h"

delete_table::delete_table()
{
    ui->label->setText("Choose table to <font style=\"color: black;background: red;\">delete</font>");

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(on_buttonBox_accepted()));
}

//void delete_table::delete_form_request_slot()
//{
//    qDebug() << "TEST TEST TEST TEST";
//}

                                    void delete_table::on_buttonBox_accepted()
                                    {
                                        emit delete_table_sig(ui->comboBox);
//                                        emit delete_database(ui->comboBox);
                                        qDebug() <<"CURRENT TEXT COMBO BOX::::"<< ui->comboBox->currentText();

                                    }
