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
  , new_db_window_(new create_db_name)
  , delete_db_window_(new delete_db)
{
    ui->setupUi(this);

    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    //SIGNALs
//    connect(this,SIGNAL(select_cells_signal(const QModelIndex &,const QModelIndex &)),
//            this,SLOT(select_cells(const QModelIndex &,const QModelIndex &)),Qt::QueuedConnection);
    connect(this,SIGNAL(show_tables_signal()),tables_window_,SLOT(show_tables()));

    connect(tables_window_,SIGNAL(db_show()),this,SLOT(on_showDB_button_clicked()));

    connect(new_db_window_,SIGNAL(create_db_signal(QString)),this,SLOT(create_db_slot(QString)));

    connect(this,SIGNAL(delete_form_request()),delete_db_window_,SLOT(delete_form_request_slot()));

    connect(delete_db_window_,SIGNAL(delete_form_send(QComboBox*)),this,SLOT(delete_form_send_slot(QComboBox*)));

    connect(delete_db_window_,SIGNAL(delete_database(QComboBox*)),this,SLOT(delete_database_slot(QComboBox*)));
}

Databases::~Databases()
{
    db_connection::close();
    delete ui;
    delete tables_window_;
    delete new_db_window_;
    delete delete_db_window_;
}



void Databases::message_from_login(QString message)
{
    ui->statusLine->clear();
    ui->statusLine->insert(message);
    on_showDB_button_clicked();
}

void Databases::create_db_slot(QString query)
{
    db_connection::open(auth_);


    bool query_success=db_connection::set_query(query,model_,ui->tableView,QHeaderView::Stretch);

    if(query_success){

        db_connection::close();
        on_showDB_button_clicked();
        //select_cells(ui->tableView->model()->index(0, 0, QModelIndex()));
        select_cells(0,0, ui->tableView);

    } else {

        QMessageBox::warning(this,"Warning","Database is not created. Please check name and try again.");

    }

}

void Databases::delete_form_send_slot(QComboBox *comboBox__)
{
    db_connection::open(auth_);


    db_connection::set_query("SHOW DATABASES;",model_,comboBox__);

    comboBox__->setCurrentIndex(-1); //for blank cell default
}

void Databases::delete_database_slot(QComboBox *comboBox__)
{
    db_connection::open(auth_);

    bool query_success=db_connection::set_query("DROP DATABASE "+comboBox__->currentText()+";",model_,comboBox__);

    if(query_success){

        db_connection::close();
        on_showDB_button_clicked();
        //select_cells(ui->tableView->model()->index(0, 0, QModelIndex()));
        select_cells(0,0, ui->tableView);

    } else {

        QMessageBox::warning(this,"Warning","Database is not deleted. May be it was been deleted earlier.");

    }
    on_showDB_button_clicked(); // view database after deletion
}


void Databases::on_showDB_button_clicked()
{

    db_connection::open(auth_);


    db_connection::set_query("SHOW DATABASES;",model_,ui->tableView,QHeaderView::Stretch);


    //select_cells(ui->tableView->model()->index(0, 0, QModelIndex()));
    select_cells(0,0, ui->tableView);

    auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

    qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

    qDebug() << "current auth_.dB_name_ index::" << auth_.db_name_;

    if(!auth_.db_name_.isNull()){
        ui->showTables_button->setEnabled(true);
        ui->showTables_button->setStyleSheet("background: green; color: white;");
    }
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

}

void Databases::test()
{


}

void Databases::on_create_db_button_clicked()
{
    new_db_window_->setModal(true);
    new_db_window_->show();
}

void Databases::on_delete_db_button_clicked()
{
    delete_db_window_->setModal(true);
    delete_db_window_->show();
    emit delete_form_request();
}

void Databases::on_comboBox_activated(const QString &arg1)
{
//    db_connection::open(auth_);


//    db_connection::set_query("SHOW DATABASES;",model_,ui->comboBox);
}

void Databases::on_comboBox_test_button_clicked()
{
    db_connection::open(auth_);


    db_connection::set_query("SHOW DATABASES;",model_,ui->comboBox);
}
