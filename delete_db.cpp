#include "delete_db.h"

delete_db::delete_db(QWidget *parent) :
    delete_form(parent,"database")
//  , ui(new Ui::delete_db)
{
//    ui->setupUi(this);
//    ui->comboBox->setCurrentIndex(-1);
    ui->label->setText("Choose database to <font style=\"color: black;background: red;\">delete</font>");

   // connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(on_buttonBox_accepted()));
}

//delete_db::~delete_db()
//{
//    //delete ui;
//}

//void delete_db::delete_form_request_slot()
//{
//    emit delete_form_send(ui->comboBox);
//    qDebug() << "DELETE FORM REQUEST SLOT";
//}



void delete_db::on_buttonBox_accepted()
{
//    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", QString("Do you really want to delete chosen database \"%1\"?").arg(ui->comboBox->currentText()),
//                                                             QMessageBox::Yes|QMessageBox::No);
//      if (reply == QMessageBox::Yes) {
//          emit delete_database(ui->comboBox);
//        qDebug() << "Yes was clicked in delete window";
//      } else {
//        qDebug() << "cancel deletion";
//      }
    delete_form::on_buttonBox_accepted();
}
