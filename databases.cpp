#include "databases.h"
#include "ui_databases.h"

//Databases::Databases(QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::Databases)
//  , db_server_("QMYSQL")
//  , auth_()
//{
//    ui->setupUi(this);

//    //status_bar_ = new QStatusBar(this);
//    //ui->statusBarLayout->addWidget(status_bar_);
//    //status_bar_->showMessage("status bar");
//    ui->statusLine->setReadOnly(true);

//    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
//                                | Qt::WindowMinimizeButtonHint
//                                | Qt::WindowMaximizeButtonHint
//                                | Qt::WindowCloseButtonHint;
//    this->setWindowFlags(flags);

//    //SIGNALS


//}

Databases::Databases(auth& auth__, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Databases)
  //, db_connection_(info)
  , auth_(auth__)
  , tables_window_(new Tables(auth_))
{
    ui->setupUi(this);

    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    //SIGNALs
    connect(this,SIGNAL(select_cells_signal(const QModelIndex &,const QModelIndex &)),
            this,SLOT(select_cells(const QModelIndex &,const QModelIndex &)),Qt::QueuedConnection);
    connect(this,SIGNAL(show_tables_signal()),tables_window_,SLOT(show_tables()));

    connect(tables_window_,SIGNAL(db_show()),this,SLOT(on_showDB_button_clicked()));
}

Databases::~Databases()
{
    db_close();
    delete ui;
    delete tables_window_;
}



void Databases::message_from_login(QString message)
{
    ui->statusLine->clear();
    ui->statusLine->insert(message);
}


void Databases::on_showDB_button_clicked()
{

    db_close();

    db_connect(auth_);










    //QSqlQueryModel model;


    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(auth_.db_server_));

    qry.prepare("SHOW DATABASES;"); //MY_SQL_QUERY

    qry.exec();
    model_.setQuery(qry);

    ui->tableView->setModel(&model_);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //select_cells(ui->tableView->model()->index(0, 0, QModelIndex()));
    select_cells(3,0,0,0, ui->tableView);

    auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

    qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

    qDebug() << "current auth_.dB_name_ index::" << auth_.db_name_;

    if(!auth_.db_name_.isNull()) ui->showTables_button->setEnabled(true);
}





void Databases::on_showTables_button_clicked()
{
    tables_window_->setModal(true);
    tables_window_->show();
    emit show_tables_signal();
}


void Databases::on_tableView_activated(const QModelIndex &index)
{


    QString val=ui->tableView->model()->data(index).toString();
    ui->statusLine->setText("Database activated: "+val);
}

void Databases::on_tableView_clicked(const QModelIndex &index)
{
    ////SETUP CURRENT INDEX POSITION (first 'a' index)
    ui->tableView->setCurrentIndex(ui->tableView->model()->index(index.row(),index.column()));

    //SET CURRENT DB NAME
    auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();


    QString val=ui->tableView->model()->data(index).toString();
    ui->statusLine->setText("Database clicked: "+val);
    qDebug() << ui->tableView->currentIndex();

}



void Databases::on_pushButton_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

    QModelIndex topLeft;
    QModelIndex bottomRight;

    topLeft = ui->tableView->model()->index(0, 0, QModelIndex());
    bottomRight = ui->tableView->model()->index(2, 0, QModelIndex());

    QItemSelection selection(topLeft, bottomRight);
    selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
}

void Databases::test()
{
        QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

        QModelIndex topLeft;
        QModelIndex bottomRight;

        topLeft = ui->tableView->model()->index(0, 0, QModelIndex());
        bottomRight = ui->tableView->model()->index(2, 0, QModelIndex());

        QItemSelection selection(topLeft, bottomRight);
             selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

}
