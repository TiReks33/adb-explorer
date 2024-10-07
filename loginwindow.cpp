#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "blinkinbutton.h"

int timeout_reconnect = 15000; //60000

bool CONNECTION_LOST_MESSAGE = true;

loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
    , db_window_(new Databases(auth_))
    , timer{new QTimer{this}}
{
    ui->setupUi(this);
    this->setFixedSize(QSize(600, 325));
    move(screen()->geometry().center() - frameGeometry().center());
    ui->checkBox->setText("Hide");
    ui->checkBox->setChecked(true);
//    ui->Host_Form->setText("localhost");

    ui->cc_label->setToolTip("<pre style=\"white-space: pre-wrap;\"><img src=':/pic/9rnvobslce_cc.png' height='16' width='16'><b>"
                        "[Non-Commercial License<br>   Requires Attribution]</b><br>This logo's "
                        "original image<br>was "
                        "designed by <b>Wannapik</b.><br>"
                        "       [URL:]<br>"
                        "<a>https://www.wannapik.com<br>"
                        "/vectors/22949</a></pre>");
    ui->cc_label->setToolTipDuration(60000);



    connect(this,SIGNAL(message_to_database_window(QString const&)),db_window_,SLOT(message_to_status(QString const&)));


    connect(ui->port_checkBox,&QCheckBox::stateChanged,[=](int state){
        if(!state){
            ui->portForm->setEnabled(false);
        } else {
            ui->portForm->setEnabled(true);
            ui->portForm->setFocus();
        }
    });


//    QDialog dialog;

//    QVBoxLayout* layout = new QVBoxLayout;
//    dialog.setLayout(layout);

//    QPushButton* button1 = new QPushButton("Button 1");
//    BlinkinButton* button2 = new BlinkinButton("Button 2");

//    layout->addWidget(button2);

//    layout->insertWidget(0,button1);

//    connect(button1,&QPushButton::clicked,[=]{
//        (!button2->is_active()) ? button2->start_blinkin(890) : button2->stop_blinkin();
//    });
////    button2->start_blinkin();

//    dialog.show();


//    dialog.exec();



    QStringList drivers_list = QSqlDatabase::drivers();

//    std::sort(drivers_list.begin(), drivers_list.end(), [](const QString &lhs, const QString &rhs)
//    {
//        QString num_lhs = lhs.split(' ').last();
//        QString num_rhs = rhs.split(' ').last();
//        return num_lhs < num_rhs;
//    });

    std::sort(drivers_list.begin(), drivers_list.end());

    ui->driverComboBox->addItems(drivers_list);


    ////ui->pushButton->setFocus();
    ui->Login_Form->setFocus();

    setTabOrder(ui->Login_Form, ui->Password_Form);
    setTabOrder(ui->Password_Form, ui->checkBox);
    setTabOrder(ui->checkBox, ui->Host_Form);
    setTabOrder(ui->Host_Form, ui->conOptsButton);
    setTabOrder(ui->conOptsButton, ui->port_checkBox);
    setTabOrder(ui->port_checkBox, ui->portForm);
    setTabOrder(ui->portForm, ui->driverComboBox);
    setTabOrder(ui->driverComboBox, ui->pushButton);





    connect(this,&loginWindow::current_driver_check_,[=]{

        QPointer <QMessageBox> messageBox{new QMessageBox(QMessageBox::Information,"Current SQL driver issue",
                                                          39+auth_.db_driver_+39+" driver currently not tested.",
                                                          QMessageBox::Ok,this/*0*/)};

        connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
        messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
        //messageBox->setModal(false);
        messageBox->show();

    });


    connect(this,&loginWindow::start_connection_timer_stuff,[=]{
            timer->start(timeout_reconnect);
    });

    connect(timer, &QTimer::timeout, this, &loginWindow::connection_timer_slot);


    connect(ui->conOptsButton, &QPushButton::clicked,[=]{
        gset_connection_options();
    });

}



loginWindow::~loginWindow()
{
    delete ui;
    delete db_window_;
}

void loginWindow::connection_timer_slot()
{
//    db_connection::reopen_exist(auth::con_name_);
//    if(!QSqlQuery(QSqlDatabase::database(auth::con_name_,false)).exec("SELECT 1")){
//        //QMessageBox::critical(this,"Connection lost","Connection to SQL database lost.",QMessageBox::Ok);
//        qDebug()<<"Connection lost.. trying to reopen..";
//    } else{
//        qDebug()<<"Connection reopened!";
//    }

    static QPointer <QMessageBox> messageBox;

    if(db_connection::reopen_exist(auth::con_name_)){

        QSqlDatabase database = QSqlDatabase::database(auth::con_name_,true);

        QSqlQuery qry = QSqlQuery(database);
    //    QSqlQuery qry = QSqlQuery(db__);

//        if(qry.prepare("SELECT 1")){ //MY_SQL_QUERY

            if(qry.exec("SELECT 1")){
                qDebug()<<"Connection opened!";
                if(CONNECTION_LOST_MESSAGE && messageBox){
                    if(messageBox->isVisible()){
                        messageBox->close();
                    }
                }
            } else{
                QString const err = qry.lastError().text();
                QString const err_number = qry.lastError().nativeErrorCode();
//                QString const err_dbtext = qry.lastError().databaseText();
//                QString const err_driver_text = qry.lastError().driverText();

                qDebug() << err;
                qDebug() << err_number;
//                qDebug() << err_dbtext;
//                qDebug() << err_driver_text;

                 qDebug()<<"Connection lost.. trying to reopen..";
                 if(CONNECTION_LOST_MESSAGE && !messageBox){
                     if((messageBox = new QMessageBox{QMessageBox::Critical,"Connection lost",QString("Connection to SQL database lost. Details: ")+
                                                                                    "<FONT COLOR='#ff0000'>"+'\"'+err+'\"'+"</FONT>"+
                                                                                    ". Error number: "+"<FONT COLOR='#ff0000'>"+err_number+"</FONT>"+'.',
                                                                                        QMessageBox::NoButton,this})){
                         messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
                         connect(messageBox,&QMessageBox::destroyed,[=]{
                             qDebug() << "~MessageBox::Critical";
                         });

                         QPushButton* recon_but = messageBox->addButton("Force reconnect",QMessageBox::ActionRole);

                         recon_but->disconnect();

                         connect(recon_but,&QPushButton::clicked,[=]{
                             connection_timer_slot();
                         });

                         messageBox->open();
                     }
                 }
            }
//        } else{
//            qDebug() << "Query not prepared";
//            qDebug() << "Try to open again..";

//        }
    }
}

void loginWindow::gset_connection_options()
{
    QPointer<QDialog> options_dialog = new QDialog{this};

    options_dialog->setAttribute(Qt::WA_DeleteOnClose,true);

    connect(options_dialog,&QDialog::destroyed,[=]{
        qDebug() << "~Options_dialog";
    });


    QVBoxLayout* options_layout = new QVBoxLayout;

    options_dialog->setLayout(options_layout);

    options_dialog->setWindowTitle("Server connection settings");

    QHBoxLayout* rec_timeout_layout = new QHBoxLayout;

    QLabel* rec_timeout_lbl = new QLabel{"Server reconnect delay"};

    rec_timeout_layout->addWidget(rec_timeout_lbl);

    QLineEdit* timeout_line = new QLineEdit;

    rec_timeout_layout->addWidget(timeout_line);

    timeout_line->setText(QString::number(timeout_reconnect));

    //QIntValidator* validator = new QIntValidator(1000, 2073600000);

    timeout_line->setValidator( new QIntValidator(1000, 2073600000) );

    options_layout->addLayout(rec_timeout_layout);

    QHBoxLayout* warning_message_lay = new QHBoxLayout;

    options_layout->addLayout(warning_message_lay);

    QLabel* warning_message_lbl = new QLabel{"Show warning pop-up while connection is lost"};

    warning_message_lay->addWidget(warning_message_lbl);

    QCheckBox* warning_message_checkbox = new QCheckBox;

    warning_message_lay->addWidget(warning_message_checkbox);

    if(CONNECTION_LOST_MESSAGE)
        warning_message_checkbox->setChecked(true);
    else
        warning_message_checkbox->setChecked(false);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);


    connect(buttonBox, &QDialogButtonBox::rejected, options_dialog, &QDialog::reject);

    options_layout->addWidget(buttonBox);


    QFrame* status_frame = new QFrame;

    status_frame->setFrameShape(QFrame::NoFrame);

    status_frame->setFrameShadow(QFrame::Raised);

    status_frame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QStatusBar* statusbar = new QStatusBar;

    statusbar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout* statusbar_layout = new QVBoxLayout(status_frame);

    statusbar_layout->setSpacing(0);

    statusbar_layout->setContentsMargins(0,0,0,0);

    statusbar_layout->setSizeConstraint(QLayout::SetDefaultConstraint);

    //statusbar->showMessage("aBc");

    statusbar_layout->addWidget(statusbar);

    options_layout->addWidget(status_frame);


    options_layout->setSpacing(3);
    options_layout->setContentsMargins(3,3,3,3);
    options_layout->setSizeConstraint(QLayout::SetFixedSize);


    qDebug() << statusbar->font();

    connect(buttonBox, &QDialogButtonBox::accepted, [=]{
            QString str = timeout_line->text();
            int pos=0;
            auto timeout_validate = timeout_line->validator()->validate(str,pos);
            if(timeout_validate!=QValidator::Acceptable){

                statusbar->setStyleSheet("color:red;");
                statusbar->showMessage("timeout value incorrect.[min. 1000]");
                return;
            }

            timeout_reconnect=timeout_line->text().toInt();

            CONNECTION_LOST_MESSAGE=warning_message_checkbox->isChecked();

            options_dialog->accept();
    });

    options_dialog->setModal(true);
    options_dialog->show();
    options_dialog->exec();
}



void loginWindow::on_pushButton_clicked()
{
    auth_.login_=this->ui->Login_Form->text();
    auth_.passw_=this->ui->Password_Form->text();
    auth_.host_=this->ui->Host_Form->text();
    auth_.db_driver_=this->ui->driverComboBox->currentText();
    auth_.port_=this->ui->portForm->text().toInt();


        if(!db_connection::open(auth_))
            ui->statusbar->showMessage("(x)Authorization wrong. Please check your login details.");
        else{

            ui->statusbar->showMessage("(✓)Successful authorization");

            if(db_window_->show_databases()){
                db_window_->setModal(true);
                db_window_->show();
                emit message_to_database_window("SQL database autorization for user ::"+auth_.login_+":: succesfull.");

                if(auth_.db_driver_!="QMARIADB" && auth_.db_driver_!="QMYSQL" && auth_.db_driver_!="QMYSQL3")
                emit current_driver_check_();

                this->hide();
            } else {
                ui->statusbar->showMessage(ui->statusbar->currentMessage()
                                           .append(".. but something goes wrong, and query to SQL DB failed.:("));
            }

            emit start_connection_timer_stuff();

        }

}


void loginWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->Password_Form->setEchoMode(QLineEdit::Password);
    else
        ui->Password_Form->setEchoMode(QLineEdit::Normal);

}


