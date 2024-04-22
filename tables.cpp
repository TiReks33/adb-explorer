#include <tables.h>
#include "ui_tables.h"

Tables::Tables(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tables)
//  , db_server_("QMYSQL")
  , auth_(auth__)
{
    ui->setupUi(this);

    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    //SIGNALS

}

Tables::~Tables()
{
    delete ui;
}

void Tables::closeEvent(QCloseEvent *event)
{
    emit db_show();
    event->accept();
}

void Tables::show_tables()
{
    db_connect(auth_);

    //QSqlQueryModel model;


        QSqlQuery qry = QSqlQuery(QSqlDatabase::database());
        //QSqlQuery* qry = new QSqlQuery(db_connection_);

        qry.prepare("SHOW TABLES;"); //MY_SQL_QUERY

        qry.exec();
        model_.setQuery(qry);

        ui->tableView->setModel(&model_);

        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


        qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

        //ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        //ui->tableView->selectionModel()->select( ui->tableView->model()->index(0,0), QItemSelectionModel::ClearAndSelect );
        //ui->tableView->selectionModel()->select(ui->tableView->model()->index(0,0), QItemSelectionModel::Select);
        //ui->tableView->setCurrentIndex(ui->tableView->model()->index(0,0));
        //ui->tableView->selectionModel()->select(ui->tableView->model()->index(0,0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

}


void Tables::on_showDB_button_clicked()
{
    show_tables();
}
