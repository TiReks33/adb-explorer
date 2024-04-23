#include "create_db_name.h"
#include "ui_create_db_name.h"

create_db_name::create_db_name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create_db_name)
{
    ui->setupUi(this);

}

create_db_name::~create_db_name()
{
    delete ui;
}

void create_db_name::on_buttonBox_accepted()
{
    emit create_db_signal("CREATE DATABASE "+ui->name_line->text()+";");
}
