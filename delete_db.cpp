#include "delete_db.h"
#include "ui_delete_db.h"

delete_db::delete_db(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::delete_db)
{
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(-1);
    ui->label->setText("Choose database to <font style=\"color: black;background: red;\">delete</font>");
}

delete_db::~delete_db()
{
    delete ui;
}

void delete_db::delete_form_request_slot()
{
    emit delete_form_send(ui->comboBox);
    qDebug() << "DELETE FORM REQUEST SLOT";
}



void delete_db::on_buttonBox_accepted()
{
    emit delete_database(ui->comboBox);
}
