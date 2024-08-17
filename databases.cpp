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

    delete_db_window_->setWindowTitle("Remove database dialog");

    //SIGNALs
//    connect(this,SIGNAL(select_cells_signal(const QModelIndex &,const QModelIndex &)),
//            this,SLOT(select_cells(const QModelIndex &,const QModelIndex &)),Qt::QueuedConnection);
    connect(this,SIGNAL(show_tables_signal()),tables_window_,SLOT(show_tables()));

    connect(tables_window_,SIGNAL(db_show()),this,SLOT(on_showDB_button_clicked()));

    connect(tables_window_,&Tables::db_show, [=]{ this->show();});

    connect(new_db_window_,SIGNAL(create_db_signal(QString)),this,SLOT(create_db_slot(QString)));

    connect(this,SIGNAL(delete_form_request()),delete_db_window_,SLOT(delete_form_request_slot()));

    connect(delete_db_window_,SIGNAL(delete_form_send(QComboBox*)),this,SLOT(delete_form_send_slot(QComboBox*)));

    connect(delete_db_window_,SIGNAL(delete_entity/*database*/(QComboBox*)),this,SLOT(delete_database_slot(QComboBox*)));
}

Databases::~Databases()
{
    db_connection::close();
    db_connection::remove();
    delete ui;
    delete tables_window_;
    delete new_db_window_;
    delete delete_db_window_;
}

void Databases::message_to_status(const QString & message__) const
{
    ui->statusLine->clear();
    ui->statusLine->insert(message__);
}



void Databases::message_from_login(QString message)
{
//    ui->statusLine->clear();
//    ui->statusLine->insert(message);
    message_to_status(message);
    on_showDB_button_clicked();
}

void Databases::create_db_slot(QString query)
{
    db_connection::open(auth_);


    if(db_connection::set_query(query,&model_,ui->tableView,QHeaderView::Stretch)){

      ////  db_connection::close(auth::con_name_);
//        on_showDB_button_clicked();
        //select_cells(ui->tableView->model()->index(0, 0, QModelIndex()));
            //select_cells(0,0, ui->tableView);

    } else {

        QMessageBox::warning(this,"Warning","Database is not created. Please check name and try again.");

    }
 on_showDB_button_clicked();
}

void Databases::delete_form_send_slot(QComboBox *comboBox__)
{
    db_connection::open(auth_);


    db_connection::set_query("SHOW DATABASES;",&model_,comboBox__);

    comboBox__->setCurrentIndex(-1); //for blank cell default
}

void Databases::delete_database_slot(QComboBox *comboBox__)
{
    db_connection::open(auth_);


    if(!db_connection::set_query("DROP DATABASE "+comboBox__->currentText()+";",&model_,comboBox__)){

        QMessageBox::warning(this,"Warning","Database is not deleted. May be it was been deleted earlier.");

    }

    on_showDB_button_clicked(); // view database after deletion
}

void Databases::show_databases()
{
    db_connection::open(auth_);


    db_connection::set_query("SHOW DATABASES;",&model_,ui->tableView,QHeaderView::Stretch);

qDebug() << "Contains?:"<<QSqlDatabase::contains(auth::con_name_);
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

//void Databases::db_show_slot()
//{
//    this->show();
//}


void Databases::on_showDB_button_clicked()
{

//    db_connection::open(auth_);


//    db_connection::set_query("SHOW DATABASES;",model_,ui->tableView,QHeaderView::Stretch);

//qDebug() << "Contains?:"<<QSqlDatabase::contains(auth::con_name_);
//    //select_cells(ui->tableView->model()->index(0, 0, QModelIndex()));
//    select_cells(0,0, ui->tableView);

//    auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

//    qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

//    qDebug() << "current auth_.dB_name_ index::" << auth_.db_name_;

//    if(!auth_.db_name_.isNull()){
//        ui->showTables_button->setEnabled(true);
//        ui->showTables_button->setStyleSheet("background: green; color: white;");
//    }
    show_databases();
}





void Databases::on_showTables_button_clicked()
{
    //tables_window_->setModal(true);
    tables_window_->show();
    emit show_tables_signal();
    this->hide();
}


void Databases::on_tableView_activated(const QModelIndex &index)
{

qDebug()<<"ACTIVATED::index=="<<index;
    QString val=ui->tableView->model()->data(index).toString();
    ui->statusLine->setText("Database activated: "+val);
}

void Databases::on_tableView_clicked(const QModelIndex &index)
{
    ////SETUP CURRENT INDEX POSITION (first 'a' index)
    //ui->tableView->setCurrentIndex(ui->tableView->model()->index(index.row(),index.column()));
    ui->tableView->setCurrentIndex(index);
    //ui->tableView->model()->data(index);

    //SET CURRENT DB NAME
    //auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();
    auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();


////    QSqlDatabase::database(auth::con_name_).addDatabase(auth_.db_name_);


    //QString val=ui->tableView->model()->data(index).toString();
    //ui->statusLine->setText("Database clicked: "+val);
    qDebug() << "CLICKED::::::::::::" << ui->tableView->currentIndex() << "::" << auth_.db_name_;


}



//void Databases::on_pushButton_clicked()
//{

//}

//void Databases::test()
//{


//}

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

//void Databases::on_comboBox_activated(const QString &arg1)
//{
////    db_connection::open(auth_);


////    db_connection::set_query("SHOW DATABASES;",model_,ui->comboBox);
//}

//void Databases::on_comboBox_test_button_clicked()
//{
//    db_connection::open(auth_);


//    db_connection::set_query("SHOW DATABASES;",model_,ui->comboBox);
//}

void Databases::on_mysqldump_button_clicked()
{
//    if(auth_.db_server_!="QMYSQL"){
//        QMessageBox::warning(this,"This database dump currently unavailable","ADB Explorer's dump function"
//                                    " doesn't support yet this database server (current driver: \""+auth_.db_server_+"\").",
//                                    QMessageBox::Ok);
//        qDebug()<<"After QMessageBox";
//        return;
//    }
    QScopedPointer<QDialog> dump_auth_choose{new QDialog{this}};

    dump_auth_choose->setWindowTitle("SQL dump");

    dump_auth_choose->setModal(true);

    dump_auth_choose->setMinimumSize(320,240);

    dump_auth_choose->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    dump_auth_choose -> setLayout(new QVBoxLayout);

    QLabel * label = new QLabel;

    label->setText("Please, choose credential for dumping.");

    dump_auth_choose->layout()->addWidget(label);

    QPushButton * current_credential_button = new QPushButton;

    dump_auth_choose->layout()->addWidget(current_credential_button);

    current_credential_button->setText("current_credential_button");

    connect (current_credential_button, &QPushButton::clicked,[&](){

        QScopedPointer<QDialog> dump_name{new QDialog{this}};

        dump_name->setWindowTitle("Dump name setting");

        dump_name->setModal(true);

        connect(dump_name.get(), &QDialog::destroyed,[=](){ qDebug() << "dump_name button destroyed(~)";});

        dump_name->setMinimumSize(int(dump_auth_choose->width()),int(dump_auth_choose->height()/2));

        dump_name->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

        dump_name -> setLayout(new QVBoxLayout);

        QLabel * label = new QLabel;

        label->setText("Set name for dump:");

        dump_name->layout()->addWidget(label);

        QLineEdit * line_edit = new QLineEdit(this);

        dump_name->layout()->addWidget(line_edit);

        line_edit->setPlaceholderText("example.sql");

        QDialogButtonBox * button_box = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

        connect(button_box, &QDialogButtonBox::rejected,[&](){ dump_name->close(); });

        dump_name->layout()->addWidget(button_box);

        connect(button_box, &QDialogButtonBox::accepted,[&](){
                QProcess dumpProcess(this);
                QStringList args;
                args << QString("-u"+auth_.login_) << QString("-p"+auth_.passw_) << "aatest1";
                dumpProcess.setStandardOutputFile("Qtaatest1.sql");

                qDebug() << "signal accepted emited";

//                QObject::connect(&dumpProcess,QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),[&](int exitCode, QProcess::ExitStatus exitStatus){
                  QObject::connect(&dumpProcess,static_cast<void (QProcess::*) (int, QProcess::ExitStatus)>(&QProcess::finished),[&](int exitCode, QProcess::ExitStatus exitStatus){
                    qDebug() << "signal QProcess::finished emited";
                    QPointer<Databases> grand_parent = qobject_cast<Databases *>(dump_auth_choose->parent());
                    if(exitStatus==QProcess::NormalExit){
                        QString const status_message = "(✓) Dumping process finished successfully. Exit code: " + QString::number(exitCode) ;
                    qDebug() << status_message;
                    dump_name->close();

                    //QPointer<Databases> grand_parent = qobject_cast<Databases *>(dump_auth_choose->parent());
                    if(grand_parent) grand_parent->message_to_status(status_message) ;

                    dump_auth_choose->close();
                    } else {
                        QString const status_message =  "(x) Dumping process failed. Exit code: " + QString::number(exitCode) ;
                        QMessageBox::warning(this,"Dumping process failed",status_message);
                        if(grand_parent) grand_parent->message_to_status(status_message) ;
                    }

                });


                  dumpProcess.start("mysqldump", args);
                  dumpProcess.waitForFinished();

        });

        dump_name->show();
        dump_name->exec();
    });

    QPushButton * another_credential_button = new QPushButton;

    dump_auth_choose->layout()->addWidget(another_credential_button);

    QPushButton * exit_button = new QPushButton;

    connect(exit_button,&QPushButton::clicked,[&](){dump_auth_choose->close();});

    exit_button->setText("Cancel");

    dump_auth_choose->layout()->addWidget(exit_button);

    dump_auth_choose->show();
    dump_auth_choose->exec();

//    QProcess dumpProcess(this);
//    QStringList args;
//    args << "-uroot" << "-pmysql" << "test";
//    dumpProcess.setStandardOutputFile("test.sql");
//    dumpProcess.start("mysqldump", args);
}
