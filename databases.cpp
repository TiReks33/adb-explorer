#include "databases.h"
#include "ui_databases.h"

Databases::Databases(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Databases)
  , db_server_("QMYSQL")
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

//Databases::Databases(QSqlDatabase info, QWidget *parent) :
//    QDialog(parent),
//    ui(new Ui::Databases)
//  //, db_connection_(info)
//{
//    ui->setupUi(this);

//    ui->statusLine->setReadOnly(true);

//    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
//                                | Qt::WindowMinimizeButtonHint
//                                | Qt::WindowMaximizeButtonHint
//                                | Qt::WindowCloseButtonHint;
//    this->setWindowFlags(flags);
//}

Databases::~Databases()
{
    close_DB();
    delete ui;
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

void Databases::receive_auth(QString login, QString passw, QString host)
{
    db_login_=login;
    db_passw_=passw;
    db_host_=host;
}

void Databases::on_showDB_button_clicked()
{
    //loginWindow connection;
    close_DB();

    //connection.connection_open(db_login_,db_passw_,db_host_);


    QSqlDatabase db_connection_=QSqlDatabase::addDatabase(db_server_);

        db_connection_.setUserName(db_login_);

        db_connection_.setPassword(db_passw_);

        //db_connection_.setHostName("localhost");//<-remote IP

        if(!db_connection_.open()){
            qDebug() << ("(x)Error connection to database.");
            return;
        }
        else{
            qDebug()<<("Database succesfull connected.");
        }









    QSqlQueryModel* modal = new QSqlQueryModel();


    QSqlQuery* qry = new QSqlQuery(db_connection_);

    qry->prepare("SHOW DATABASES;"); //MY_SQL_QUERY

    qry->exec();
    modal->setQuery(*qry);

    ui->tableView->setModel(modal);

    //connection.connection_close();

            //db_connection_.close();

//    {
//        QSqlDatabase db = QSqlDatabase::database();
//        db.close();
//    }
//    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
//    return;



    qDebug() << "Number of existinf DBs::" <<(modal->rowCount());

    //qDebug() << "123";
}

void Databases::on_pushButton_2_clicked()
{
    emit test_signal();
}

void Databases::on_tableView_activated(const QModelIndex &index)
{
    QString val=ui->tableView->model()->data(index).toString();
    ui->statusLine->setText("Database activated: "+val);
}

void Databases::on_tableView_clicked(const QModelIndex &index)
{
    QString val=ui->tableView->model()->data(index).toString();
    ui->statusLine->setText("Database clicked: "+val);

}
