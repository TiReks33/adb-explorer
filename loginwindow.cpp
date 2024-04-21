#include "loginwindow.h"
#include "ui_loginwindow.h"

loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
    , db_window_(new Databases(auth_))
    //, db_server_("QMYSQL")
{
    ui->setupUi(this);
    this->setFixedSize(QSize(400, 225));
    move(screen()->geometry().center() - frameGeometry().center());
    ui->checkBox->setText("Hide");
    ui->checkBox->setChecked(true);
    ui->Host_Form->setText("localhost");

//db_connection_=QSqlDatabase::addDatabase("QMYSQL");
    //db_connection_.setHostName("xxx.xxx.xxx.xxx");//<-remote IP
    //db_connection_.setUserName("root");
    //db_connection_.setPassword("");
    /*if(db_connection_.open())
        ui->statusbar->showMessage("Database succesfull connected.");
    else
        ui->statusbar->showMessage("(x)Error connection to database.")*/;

    //::SIGNALS::
        //connect(this,SIGNAL(message_to_database(QString)),db_window_,SLOT(message_from_login(QString)));


//ui->statusbar->showMessage(QString::number(db_connection_.isValid()));


    //db_window_ = new Databases(/*db_connection_*/);
    connect(this,SIGNAL(message_to_database(QString)),db_window_,SLOT(message_from_login(QString)));
    connect(db_window_,SIGNAL(test_signal()),this,SLOT(test_slot()));
//    connect(this,SIGNAL(send_auth(auth&)),db_window_,SLOT(receive_auth(auth&)));
}

loginWindow::~loginWindow()
{
    delete ui;
    delete db_window_;
}

void loginWindow::closeEvent(QCloseEvent *event)
{
    //if(db_connection_.isOpen())db_connection_.close();
    //event->accept();


//            QSqlDatabase db = QSqlDatabase::database();
//            if(db.isOpen())
//        {    db.close();

//        QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
//        }
            //return;
        event->accept();
}

bool loginWindow::connection_open(auth& auth__)
{
    //if(db.isValid()){


//        db_connection_=QSqlDatabase::addDatabase(db_server_);
    QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth__.db_server_);
        //login_="root";
//        login_=login;
        //login_=ui->Login_Form->text();
        db_connection_.setUserName(auth__.login_);
        //passw_="18715";
//        passw_=password;
        //passw_=ui->Password_Form->text();
        db_connection_.setPassword(auth__.passw_);
        //db_connection_.setHostName("localhost");//<-remote IP

        if(!db_connection_.open()){
            qDebug() << ("(x)Error connection to database.");
            return false;
        }
        else{
            qDebug()<<("Database succesfull connected.");
            return true;
        }
    //}
        return false;
}


void loginWindow::test_slot()
{
    emit message_to_database(ui->Password_Form->text());
}

void loginWindow::receive_login_(QString&login_ref)
{
    login_ref=ui->Login_Form->text();
}

void loginWindow::receive_passw_(QString&passw_ref)
{
    passw_ref=ui->Password_Form->text();
}


/*void loginWindow::on_pushButton_clicked()
{
//          db_connection_=QSqlDatabase::addDatabase("QMYSQL");
          db_connection_=QSqlDatabase::addDatabase(db_name_);
    //    db_connection_.setHostName("xxx.xxx.xxx.xxx");//<-remote IP
//        db_connection_.setUserName(ui->Login_Form->text());                           //!!!!!!!!!!!!!!!!!!
          login_="root";
          db_connection_.setUserName(login_);

//        db_connection_.setPassword(ui->Password_Form->text());                        //!!!!!!!!!!!!!!!!!!
          passw_="18715";
          db_connection_.setPassword(passw_);

        if(!db_connection_.open())
            ui->statusbar->showMessage("(x)Error connection to database.");
        else{
            //ui->statusbar->showMessage("Database succesfull connected.");

            db_window_ = new Databases(db_connection_);
            connect(this,SIGNAL(message_to_database(QString)),db_window_,SLOT(message_from_login(QString)));
            connect(db_window_,SIGNAL(test_signal()),this,SLOT(test_slot()));
            db_window_->show();
            emit message_to_database("Database succesfull connected.");
            this->hide();
            //emit message_to_database(QString::number(db_connection_.isValid()));
        }

}*/


void loginWindow::on_pushButton_clicked()
{
    auth_.login_=this->ui->Login_Form->text();
    auth_.passw_=this->ui->Password_Form->text();
    auth_.host_=this->ui->Host_Form->text();

        if(!this->connection_open(auth_))
            ui->statusbar->showMessage("(x)Error connection to database.");
        else{


            db_window_->setModal(true);
            db_window_->show();
            emit message_to_database("Database succesfull connected.");

            //this->hide();

//            this->ui->Login_Form->setReadOnly(true);
//            this->ui->Password_Form->setReadOnly(true);
//            this->ui->pushButton->setEnabled(false);
//            this->ui->checkBox->setChecked(true);
//            this->ui->checkBox->setEnabled(false);
//            this->ui->Host_Form->setEnabled(false);

            //emit message_to_database(QString::number(db_connection_.isValid()));
        }

}


void loginWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->Password_Form->setEchoMode(QLineEdit::Password);
    else
        ui->Password_Form->setEchoMode(QLineEdit::Normal);

}


void loginWindow::on_login_testButton_clicked()
{
    //emit message_to_database("test from login");
    QSqlDatabase qs=QSqlDatabase::addDatabase("QMYSQL");
    qDebug() << qs.isValid();
}

