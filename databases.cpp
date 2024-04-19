#include "databases.h"
#include "ui_databases.h"
#include <QStatusBar>
#include <loginwindow.h>

Databases::Databases(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Databases)
{
    ui->setupUi(this);

    //status_bar_ = new QStatusBar(this);
    //ui->statusBarLayout->addWidget(status_bar_);
    //status_bar_->showMessage("status bar");
    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    //SIGNALS


}

Databases::Databases(QSqlDatabase info, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Databases)
  , db_connection_(info)
{
    ui->setupUi(this);

    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
}

Databases::~Databases()
{
    delete ui;
}

void Databases::message_from_login(QString message)
{
    ui->statusLine->clear();
    ui->statusLine->insert(message);
}

void Databases::on_showDB_button_clicked()
{
    loginWindow connection;
    QSqlQueryModel* modal = new QSqlQueryModel();

    connection.connection_open(db_connection_);








    QSqlQuery qry(db_connection_);

    qry.prepare("SHOW DATABASES;"); //MY_SQL_QUERY

    qry.exec();
    modal->setQuery(qry);

    ui->tableView->setModel(modal);

    //connection.connection_close(db_connection_);

    qDebug() << (modal->rowCount());
}

void Databases::on_pushButton_2_clicked()
{
    emit test_signal();
}
