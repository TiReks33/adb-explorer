#include "create_db_name.h"
#include "ui_create_db_name.h"

create_db_name::create_db_name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create_db_name)
{
    ui->setupUi(this);

    connect(ui->buttonBox,&QDialogButtonBox::accepted,[=]{

        QString const create_query_text = QString("CREATE DATABASE `%1`").arg(QString(escape_sql_backticks(ui->name_line->text())));

        emit create_db_signal(create_query_text);
    });

}

create_db_name::~create_db_name()
{
    delete ui;
}

