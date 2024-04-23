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
    if(QSqlDatabase::database().databaseName().isNull())
        db_connection::close(); // if database name in connection is not added
    // ==> close current and open new connect

    db_connection::open(auth_);


    db_connection::set_query("SHOW TABLES;",model_,ui->tableView,QHeaderView::Stretch);


    select_cells(0,0, ui->tableView);

    auth_.table_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();


        qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

        //ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        //ui->tableView->selectionModel()->select( ui->tableView->model()->index(0,0), QItemSelectionModel::ClearAndSelect );
        //ui->tableView->selectionModel()->select(ui->tableView->model()->index(0,0), QItemSelectionModel::Select);
        //ui->tableView->setCurrentIndex(ui->tableView->model()->index(0,0));
        //ui->tableView->selectionModel()->select(ui->tableView->model()->index(0,0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

        if(!auth_.table_name_.isNull()){
            ui->select_from_table_button->setEnabled(true);
            ui->select_from_table_button->setStyleSheet("background: green; color: white;");
        }
}


void Tables::on_showDB_button_clicked()
{
    show_tables();
}
