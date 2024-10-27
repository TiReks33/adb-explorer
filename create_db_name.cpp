#include "create_db_name.h"
#include "ui_create_db_name.h"

create_db_name::create_db_name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::create_db_name)
{
    ui->setupUi(this);

    setWindowTitle("Create Database dialog");

    connect(ui->buttonBox,&QDialogButtonBox::accepted,[=]{

        QString const __newdb_name = ui->name_line->text();

        QString const create_query_text = QString("CREATE DATABASE `%1`").arg(QString(adb_utility::escape_sql_backticks(/*ui->name_line->text()*/__newdb_name)));

        emit create_db_signal(create_query_text,__newdb_name);
    });

        QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
            foreach (auto obj, ButtonsInFormlist) {
                if(obj==ui->buttonBox->button(QDialogButtonBox::Ok)||obj==ui->buttonBox->button(QDialogButtonBox::Cancel)){
                    obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                        .arg(adb_style::getbuttonKhakiHiglightSS()));
                } else {
                    obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                        .arg(obj->styleSheet())
                        .arg(adb_style::getbuttonKhakiHiglightSS()));
                        }
            }

}

create_db_name::~create_db_name()
{
    delete ui;
}

