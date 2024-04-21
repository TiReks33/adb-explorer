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
}

Databases::~Databases()
{
    close_DB();
    delete ui;
    delete tables_window_;
}

void Databases::close_DB()
{
    {
        QSqlDatabase db = QSqlDatabase::database();
        db.close();
    }
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
    return;
}

void Databases::message_from_login(QString message)
{
    ui->statusLine->clear();
    ui->statusLine->insert(message);
}

//void Databases::receive_auth(auth& auth__)
//{
//    db_login_=login;
//    db_passw_=passw;
//    db_host_=host;
//}

void Databases::on_showDB_button_clicked()
{
    //loginWindow connection;
    close_DB();

    //connection.connection_open(db_login_,db_passw_,db_host_);


    QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth_.db_server_);

        db_connection_.setUserName(auth_.login_);

        db_connection_.setPassword(auth_.passw_);

        //db_connection_.setHostName("localhost");//<-remote IP

        if(!db_connection_.open()){
            qDebug() << ("(x)Error connection to database.");
            return;
        }
        else{
            qDebug()<<("Database succesfull connected.");
        }









    QSqlQueryModel* model = new QSqlQueryModel();


    QSqlQuery* qry = new QSqlQuery(db_connection_);

    qry->prepare("SHOW DATABASES;"); //MY_SQL_QUERY

    qry->exec();
    model->setQuery(*qry);

    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    //select_cells(ui->tableView->model()->index(0, 0, QModelIndex()));
    select_cells(0,0);

    auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

    qDebug() << "Number of existinf DBs::" <<(model->rowCount());

    qDebug() << "current auth_.dB_name_ index::" << auth_.db_name_;

    if(!auth_.db_name_.isNull()) ui->showTables_button->setEnabled(true);
}

void Databases::select_cells(const QModelIndex &index)
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

         selectionModel->select(index, QItemSelectionModel::ClearAndSelect);

//         QString val=ui->tableView->model()->data(index).toString();
         //         ui->statusLine->setText("Database current index after select_cells: "+val);

         ////SETUP CURRENT INDEX POSITION
         ui->tableView->setCurrentIndex(ui->tableView->model()->index(index.row(),index.column()));
}

void Databases::select_cells(int ai, int aj)
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

         selectionModel->select(ui->tableView->model()->index(ai, aj/*, QModelIndex()*/), QItemSelectionModel::ClearAndSelect);

         //SETUP CURRENT INDEX POSITION
         ui->tableView->setCurrentIndex(ui->tableView->model()->index(ai,aj));
}

void Databases::select_cells(const QModelIndex &index,const QModelIndex &index2)
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

//    QModelIndex topLeft;
//    QModelIndex bottomRight;

//    topLeft = ui->tableView->model()->index(0, 0, QModelIndex());
//    bottomRight = ui->tableView->model()->index(2, 0, QModelIndex());

    QItemSelection selection(index, index2);
         selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

//         QString val=ui->tableView->model()->data(index).toString();
         //         ui->statusLine->setText("Database current index after select_cells: "+val);

         //SETUP CURRENT INDEX POSITION (1st index)
         ui->tableView->setCurrentIndex(ui->tableView->model()->index(index.row(),index.column()));
}

void Databases::select_cells(int ai, int aj, int bi, int bj)
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

        QModelIndex a = ui->tableView->model()->index(ai, aj, QModelIndex());
        QModelIndex b = ui->tableView->model()->index(bi, bj, QModelIndex());

    QItemSelection selection(a, b);
         selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);

         ////SETUP CURRENT INDEX POSITION (first 'a' index)
         ui->tableView->setCurrentIndex(ui->tableView->model()->index(ai,aj));
}



void Databases::on_showTables_button_clicked()
{
//    tables_window_->setModal(true);
//    tables_window_->show();
    QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth_.db_server_);

        db_connection_.setUserName(auth_.login_);

        db_connection_.setPassword(auth_.passw_);

        db_connection_.setDatabaseName(auth_.db_name_);

        //db_connection_.setHostName("localhost");//<-remote IP

        if(!db_connection_.open()){
            qDebug() << ("(x)Error connection to database.");
            return;
        }
        else{
            qDebug()<<("Database succesfull connected.");
        }









    QSqlQueryModel* model = new QSqlQueryModel();


    QSqlQuery* qry = new QSqlQuery(db_connection_);

    qry->prepare("SHOW TABLES;"); //MY_SQL_QUERY

    qry->exec();
    model->setQuery(*qry);

    ui->tableView->setModel(model);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

//    QModelIndex topLeft;
//    QModelIndex bottomRight;

//    topLeft = ui->tableView->model()->index(0, 0, QModelIndex());
//    bottomRight = ui->tableView->model()->index(2, 0, QModelIndex());

//    QItemSelection selection(topLeft, bottomRight);
//         selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);


    qDebug() << "Number of existinf DBs::" <<(model->rowCount());

    //ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    //ui->tableView->selectionModel()->select( ui->tableView->model()->index(0,0), QItemSelectionModel::ClearAndSelect );
    //ui->tableView->selectionModel()->select(ui->tableView->model()->index(0,0), QItemSelectionModel::Select);
    //ui->tableView->setCurrentIndex(ui->tableView->model()->index(0,0));
    //ui->tableView->selectionModel()->select(ui->tableView->model()->index(0,0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
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
