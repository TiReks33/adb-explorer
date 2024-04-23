#include "loginwindow.h"
#include "ui_loginwindow.h"

loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
    , db_window_(new Databases(auth_))
    //, db_server_("QMYSQL")
{
    ui->setupUi(this);
    this->setFixedSize(QSize(450, 225));
    move(screen()->geometry().center() - frameGeometry().center());
    ui->checkBox->setText("Hide");
    ui->checkBox->setChecked(true);
    ui->Host_Form->setText("localhost");

    ui->cc_label->setToolTip("<pre style=\"white-space: pre-wrap;\"><img src=':/pic/9rnvobslce_cc.png' height='16' width='16'><b>"
                        "[Non-Commercial License<br>   Requires Attribution]</b><br>This logo's "
                        "original image<br>was "
                        "designed by <b>Wannapik</b.><br>"
                        "       [URL:]<br>"
                        "<a>https://www.wannapik.com<br>"
                        "/vectors/22949</a></pre>");
    ui->cc_label->setToolTipDuration(60000);


    connect(this,SIGNAL(message_to_database(QString)),db_window_,SLOT(message_from_login(QString)));
    connect(db_window_,SIGNAL(test_signal()),this,SLOT(test_slot()));

}

loginWindow::~loginWindow()
{
    delete ui;
    delete db_window_;
}

void loginWindow::closeEvent(QCloseEvent *event)
{

        event->accept();
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



void loginWindow::on_pushButton_clicked()
{
    auth_.login_=this->ui->Login_Form->text();
    auth_.passw_=this->ui->Password_Form->text();
    auth_.host_=this->ui->Host_Form->text();

        if(!db_connection::open(auth_))
            ui->statusbar->showMessage("(x)Authorization wrong. Please check your login details.");
        else{

            ui->statusbar->showMessage("(✓)Successful authorization");
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


//void loginWindow::on_login_testButton_clicked()
//{
//    //emit message_to_database("test from login");
//    QSqlDatabase qs=QSqlDatabase::addDatabase("QMYSQL");
//    qDebug() << qs.isValid();
//}

