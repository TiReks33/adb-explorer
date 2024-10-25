#include "delete_form.h"

delete_form::delete_form(QWidget *parent,QString const &entity_name__) :
    QDialog(parent)
  , ui(new Ui::delete_form)
  , entity_name_(entity_name__)
{
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(-1);

    connect(ui->comboBox,&QComboBox::textActivated,[=](const QString & cur_text__){
        qDebug() <<QString("%0 to delete:: %1").arg(entity_name_).arg(cur_text__);
    });

    ui->label->setText("Choose "+entity_name_+ " to <font style=\"color: black;background: red;\">delete</font>");
    setWindowTitle("Remove "+entity_name_+" dialog");
}

delete_form::~delete_form()
{
    delete ui;
}

void delete_form::delete_form_request_slot(){
    emit delete_form_send(ui->comboBox);
    //qDebug() << "Delete form signal was send.";
}

void delete_form::on_buttonBox_accepted()
{

    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", QString("Do you really want to delete chosen %0 \"%1\"?").arg(entity_name_).arg(ui->comboBox->currentText()),
                                                             QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
          emit delete_entity(ui->comboBox);
        //qDebug() << QString("Yes was clicked in delete %0 form").arg(entity_name_);
      } else {
        //qDebug() << "cancel deletion";
      }

}



