#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "reloadbutton.h"

//int timeout_reconnect = 15000; //60000

//bool CONNECTION_LOST_MESSAGE = true;

loginWindow::loginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::loginWindow)
    , db_window_(new Databases(auth_))
    , timer{new QTimer{this}}
//    , cryptoModule_{AdbCrypt::data(ui->Login_Form,ui->Password_Form,adb_utility::filepath_)}
{

    form_init();

    signals_init();

    fileOps();

    cryptoModuleInit();

}

void loginWindow::form_init()
{
    ui->setupUi(this);
    this->setFixedSize(QSize(600, 360));
    move(screen()->geometry().center() - frameGeometry().center());
    ui->checkBox->setText("Hide");
    ui->checkBox->setChecked(true);

    ui->cc_label->setToolTip("<pre style=\"white-space: pre-wrap;\"><img src=':/pic/9rnvobslce_cc.png' height='16' width='16'><b>"
                        "[Non-Commercial License<br>   Requires Attribution]</b><br>This logo's "
                        "original image<br>was "
                        "designed by <b>Wannapik</b.><br>"
                        "       [URL:]<br>"
                        "<a>https://www.wannapik.com<br>"
                        "/vectors/22949</a></pre>");
    ui->cc_label->setToolTipDuration(60000);

    ui->loginLbl->setStyleSheet("QToolTip {font-family:'Noto Sans','Black';color: yellow;font-weight:bold;background: brown;border:0px;}");
//    "<span style=\" font-family:'Noto Sans','Black';\"><font style=\"color: yellow;font-weight:bold;background: brown;\">[A]DB_Explorer</font></span>"

//    QStringList drivers_list = QSqlDatabase::drivers();

//    std::sort(drivers_list.begin(), drivers_list.end());

    QStringList drivers_list;

    for(auto it = auth::SQLDBtype2SQLdriver.begin();
             it!= auth::SQLDBtype2SQLdriver.end();
           ++it){
        drivers_list.append(it.key());
    }

    ui->dbtypeComboBox->addItems(drivers_list);



    ui->Login_Form->setFocus();

    setTabOrder(ui->Login_Form, ui->Password_Form);
    setTabOrder(ui->Password_Form, ui->checkBox);
    setTabOrder(ui->checkBox, ui->Host_Form);
    setTabOrder(ui->Host_Form, ui->conOptsButton);
    setTabOrder(ui->conOptsButton, ui->port_checkBox);
    setTabOrder(ui->port_checkBox, ui->portForm);
    setTabOrder(ui->portForm, ui->dbtypeComboBox);
    setTabOrder(ui->dbtypeComboBox, ui->pushButton);



    ui->dbtypeComboBox->setStyleSheet(/*comboSS*/adb_style::getComboBoxKhakiHighlightSS("","","url(:/pic/adbarrowdwn2.png)"));

    ui->conOptsButton->setStyleSheet(QStringLiteral("QPushButton{%1} %2").arg(ui->conOptsButton->styleSheet()).arg(adb_style::getbuttonKhakiHiglightSS()));

    ui->pushButton->setStyleSheet(QStringLiteral("QPushButton{%1} QPushButton:disabled {color:floralwhite; background:gray;} %2").arg(ui->pushButton->styleSheet()).arg(adb_style::getbuttonKhakiHiglightSS()));


    ui->hidePassCheckBoxLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->portCheckBoxVerticalLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }

}


void loginWindow::signals_init()
{
    // message to 2nd window after successful login
    connect(this,SIGNAL(message_to_database_window(QString const&)),db_window_,SLOT(message_to_status(QString const&)));


    connect(ui->port_checkBox,&QCheckBox::stateChanged,[=](int state){
        if(!state){
            ui->portForm->setEnabled(false);
        } else {
            ui->portForm->setEnabled(true);
            ui->portForm->setFocus();
        }
    });

//    // toDo
//    connect(this,&loginWindow::current_driver_check_,[=]{

//        QPointer <QMessageBox> messageBox{new QMessageBox(QMessageBox::Information,"Current SQL driver issue",
//                                                          39+auth_.db_driver_+39+" driver currently not tested.",
//                                                          QMessageBox::Ok,this/*0*/)};

//        ////connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
//        messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
//        //messageBox->setModal(false);
//        messageBox->show();

//    });

    // start SQL-server alive-check
    connect(this,&loginWindow::start_connection_timer_stuff,[=]{
            timer->start(timeout_reconnect);
    });

    // check that SQL server alive every 'timeout_reconnect'
    connect(timer, &QTimer::timeout, this, &loginWindow::connection_timer_slot);

    // get options dialog (settings read from file)
    connect(ui->conOptsButton, &QPushButton::clicked,[=]{
        gset_connection_options();
    });

    // write settings changes to file
    connect(this, &loginWindow::reconnect_data_changed, this, &loginWindow:: write2recon_opts_file);

    // toDo
    connect(ui->dbtypeComboBox,&QComboBox::currentTextChanged,[this](QString const& curText__){
        if(auth::SQLdriverMatch(auth::SQLDBtype2SQLdriver[curText__],SQLDBtype::PSQL)){
            ui->pushButton->setEnabled(false);
            ui->pushButton->setToolTip("*Current version of ADB-Explorer not supported PostgreSQL*");
        } else{
            ui->pushButton->setEnabled(true);
            ui->pushButton->setToolTip("");
        }
    });

}

void loginWindow::fileOps()
{
    // if no file -- create it and write default settings to it
    if(!read4rom_recon_opts_file())
        write2recon_opts_file();
}

void loginWindow::cryptoModuleInit()
{

    cryptoModule_ = AdbCrypt::data(ui->Login_Form,ui->Password_Form,adb_utility::filepath_);
    ui->Login_Form->installEventFilter(this);

}

loginWindow::~loginWindow()
{
    delete ui;
    delete db_window_;
}

void loginWindow::connection_timer_slot()
{

    static QPointer <QMessageBox> messageBox;

    //flags to report about error connection and reconnect only once (prevent log rewriting with useless info);
    static bool disconnect_once_msg = false, reconnect_once_msg = false;

    QSqlDatabase database = QSqlDatabase::database(auth::con_name_,true);

    QSqlQuery qry = QSqlQuery(database);

    //simple dummy SQL query
    if(qry.exec("SELECT 1")){

        disconnect_once_msg=false;

        if(!reconnect_once_msg){
        std::cout << "Connection is alive!" << std::endl;
        qDebug() << "Connection is alive!";
        reconnect_once_msg=true;
        }

        if(messageBox){

            messageBox->close();

        }
    } else{

       static bool error_change = false;

       static QString err="";
       QString temp = qry.lastError().text();

       if(temp!=err) error_change = true;
       err=temp;

       static QString err_number = "";
       temp = qry.lastError().nativeErrorCode();

       if(temp!=err_number) error_change = true;
       err_number=temp;


        reconnect_once_msg=false;

        if(!disconnect_once_msg || error_change){

        qDebug() << err;
        std::cout << err.toStdString() << std::endl;
        qDebug() << err_number;
        std::cout << err_number.toStdString() << std::endl;


        qDebug()<<"Connection lost.. trying to reopen..";
        std::cout << "Connection lost.. trying to reopen.." << std::endl;

        disconnect_once_msg=true;
        }

        // show warning window if flag is set in settings
        if(CONNECTION_LOST_MESSAGE){
            // allocate memory and create new window if pointer is nullptr OR error code is changed from previous
             if(!messageBox || error_change){

                 if(messageBox) messageBox->close();

                 if((messageBox = new QMessageBox{QMessageBox::Critical,"Connection lost",QString("Connection to SQL database lost. Details: ")+
                                                                                "<FONT COLOR='#ff0000'>"+'\"'+err+'\"'+"</FONT>"+
                                                                                ". Error number: "+"<FONT COLOR='#ff0000'>"+err_number+"</FONT>"+'.',
                                                                                    QMessageBox::NoButton,this})){
                     messageBox->setAttribute( Qt::WA_DeleteOnClose, true );

//                     connect(messageBox,&QMessageBox::destroyed,[=]{
//                         qDebug() << "~MessageBox::Critical";
//                     });

                     // reconnect by clicking reload button
                     QPushButton* recon_but = messageBox->addButton("Force reconnect",QMessageBox::ActionRole);

                     recon_but->disconnect();

                     connect(recon_but,&QPushButton::clicked,[=]{
                         connection_timer_slot();
                     });

                     messageBox->open();
                 }
             }
        }
        error_change = false;
    }

}

void loginWindow::gset_connection_options()
{
    QPointer<QDialog> options_dialog = new QDialog{this};

    // flag to dealloc memory after closing
    options_dialog->setAttribute(Qt::WA_DeleteOnClose,true);

//    connect(options_dialog,&QDialog::destroyed,[=]{
//        qDebug() << "~Options_dialog";
//    });


    QVBoxLayout* options_layout = new QVBoxLayout;

    options_dialog->setLayout(options_layout);

    options_dialog->setWindowTitle("Server connection settings");

    QHBoxLayout* rec_timeout_layout = new QHBoxLayout;

    QLabel* rec_timeout_lbl = new QLabel{"Server reconnect delay"};

    rec_timeout_layout->addWidget(rec_timeout_lbl);

    QLineEdit* timeout_line = new QLineEdit;

//    QStatusBar* statusBar = new QStatusBar;

    QLabel* statusBarLbl = new QLabel{/*statusbar*/};
    statusBarLbl->setWordWrap(true);
    statusBarLbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    statusBarLbl->setAlignment(Qt::AlignCenter);

//    statusBar->addWidget(statusBarLbl);

    connect(options_dialog,&QDialog::destroyed,[statusBarLbl]{ delete statusBarLbl; });

    connect(timeout_line,&QLineEdit::textChanged,[statusBarLbl](){ statusBarLbl->hide(); });

//    connect(statusbar,&QStatusBar::messageChanged,[options_dialog]{ options_dialog->adjustSize();});

    rec_timeout_layout->addWidget(timeout_line);

    timeout_line->setText(QString::number(timeout_reconnect));

    // only digits validation
    QIntValidator* int_validator = new QIntValidator(1000, 2073600000);

    timeout_line->setValidator( int_validator );

    connect(timeout_line, &QLineEdit::destroyed,[=]{ delete int_validator; /*qDebug() << "~int_validator";*/ });

    options_layout->addLayout(rec_timeout_layout);

    QHBoxLayout* warning_message_lay = new QHBoxLayout;

    options_layout->addLayout(warning_message_lay);

    QLabel* warning_message_lbl = new QLabel{"Show warning pop-up while connection is lost"};

    warning_message_lay->addWidget(warning_message_lbl);

    QCheckBox* warning_message_checkbox = new QCheckBox;

    warning_message_lay->addWidget(warning_message_checkbox);

    //
    QHBoxLayout* clearDataSubLay = new QHBoxLayout{};
    QPushButton* clearDataButton = new QPushButton{"Clear users data cache"};
    clearDataButton->setStyleSheet("font-weight:bold;");
    clearDataButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    clearDataSubLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    clearDataSubLay->addWidget(clearDataButton);
    clearDataSubLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    options_layout->addLayout(clearDataSubLay);

    connect(clearDataButton,&QPushButton::clicked,[this, options_dialog]{
        if(cryptoModule_){
            auto answer = QMessageBox::warning(options_dialog,"Clear users data", "Are you sure want to remove all saved passwords? This action is irreversible.",QMessageBox::Ok | QMessageBox::Cancel);

            if(answer==QMessageBox::Ok){
                cryptoModule_->reCreate();
            }
        }
    });
    //

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



    statusBarLbl->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout* statusbar_layout = new QVBoxLayout(status_frame);

    statusbar_layout->setSpacing(0);

    statusbar_layout->setContentsMargins(0,0,0,0);

    statusbar_layout->setSizeConstraint(QLayout::SetDefaultConstraint);

    //statusbar->showMessage("aBc");

    statusbar_layout->addWidget(statusBarLbl);

    statusBarLbl->setStyleSheet("color:red; border :2px solid black; border-style : dashed");

    statusBarLbl->hide();

    options_layout->addWidget(status_frame);


    options_layout->setSpacing(3);
    options_layout->setContentsMargins(3,3,3,3);
    options_layout->setSizeConstraint(QLayout::SetFixedSize);


//    qDebug() << statusbar->font();

    connect(buttonBox, &QDialogButtonBox::accepted, [=]{
            QString str = timeout_line->text();
            int pos=0;
            auto timeout_validate = timeout_line->validator()->validate(str,pos);

            // check correctness for timeout value
            if(timeout_validate!=QValidator::Acceptable){

                statusBarLbl->show();

                auto validMin = int_validator->bottom();
                auto validMax = int_validator->top();
                auto min2sec = validMin / 1000;
                auto max2days = validMax / (8.64 * qPow(10,7));

                statusBarLbl->setText(QStringLiteral("Timeout value is incorrect.\n[min. %1 ms (%2 sec)] - [max. %3 ms (%4 days)]")
                                      .arg(validMin)
                                      .arg(min2sec)
                                      .arg(validMax)
                                      .arg(max2days));

                return;
            }

            timeout_reconnect=timeout_line->text().toInt();

            CONNECTION_LOST_MESSAGE=warning_message_checkbox->isChecked();

            // synchronize .cfg file
            emit reconnect_data_changed();

            options_dialog->accept();
    });


    QList<QPushButton*> ButtonsInFormlist = options_dialog->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj==buttonBox->button(QDialogButtonBox::Ok)||obj==buttonBox->button(QDialogButtonBox::Cancel)){
                obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            } else {
                obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }
        }

    QList<QCheckBox*> checkBoxInFormlist = options_dialog->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }

    options_dialog->setModal(true);
    options_dialog->show();
    options_dialog->exec();
}



void loginWindow::on_pushButton_clicked()
{
    this->ui->statusbar->showMessage("Please wait..");
    // display message immediately
    QApplication::processEvents();

    this->setCursor(Qt::WaitCursor);

    // auth_ structure with credentials to transfer across multiply childs via reference
    auth_.login_=this->ui->Login_Form->text();
    auth_.passw_=this->ui->Password_Form->text();

    QString const hostInf = ui->Host_Form->text();
    (hostInf.isEmpty()) ? auth_.host_="localhost" : auth_.host_ = hostInf;

    auth_.db_driver_=auth::SQLDBtype2SQLdriver[this->ui->dbtypeComboBox->currentText()];

    if(ui->port_checkBox->isChecked()&&!ui->portForm->text().isEmpty())
        auth_.port_=this->ui->portForm->text().toInt();


    if(!db_connection::open(auth_))
        ui->statusbar->showMessage("(x)Authorization wrong. Please check your login details and network connection.");
    else{

        ui->statusbar->showMessage("(✓)Successful authorization");

        // saveLoginEncrypted
        if(cryptoModule_)
            cryptoModule_->encryptCredentials2File();

        if(db_window_->show_databases()){
            db_window_->setModal(/*true*/false);
            db_window_->show();

            // 'Database' window status bar message
            emit message_to_database_window("SQL database autorization for user ::"+auth_.login_+":: succesfull.");

            emit reconnect_data_changed();


            ////QTimer::singleShot(0,this,SLOT(connection_timer_slot()));



            this->hide();
        } else {
            ui->statusbar->showMessage(ui->statusbar->currentMessage()
                                       .append(".. but something goes wrong, and query to SQL DB failed.:("));
        }

        // start SQL connection checking
        if(auth_.host_!="127.0.0.1" && auth_.host_!="localhost")
            emit start_connection_timer_stuff();

    }

    this->setCursor(Qt::ArrowCursor);
}


void loginWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
        ui->Password_Form->setEchoMode(QLineEdit::Password);
    else
        ui->Password_Form->setEchoMode(QLineEdit::Normal);

}

void loginWindow::write2recon_opts_file()
{
    QFile outFile(config_f_name);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << "timeout_reconnect" << '=' << QString::number(timeout_reconnect) << Qt::endl;
    textStream << "CONNECTION_LOST_MESSAGE" << '=' << QString::number(CONNECTION_LOST_MESSAGE) << Qt::endl;
    textStream << "DB_TYPE"<<'='<< QString::number(ui->dbtypeComboBox->currentIndex()) << Qt::endl;
    QString const __host = (ui->Host_Form->text().isEmpty())? "localhost" : ui->Host_Form->text();
    textStream << "HOSTNAME"<<'='<< '\"'+__host+'\"' << Qt::endl;
}

bool loginWindow::read4rom_recon_opts_file()
{
//    QMap<QString,int> __settings_map;

//    // fill the QMap container with key-values pairs
//    if(adb_utility::get_settings_4rom_file(config_f_name,__settings_map)){
//        int temp;

//        // find necessary settings
//        if((temp = __settings_map.value("timeout_reconnect"))!=-1)
//            timeout_reconnect = temp;

//        if((temp = __settings_map.value("CONNECTION_LOST_MESSAGE"))!=-1)
//            CONNECTION_LOST_MESSAGE = temp;

//        if((temp = __settings_map.value("DB_TYPE"))!=-1)
//            this->ui->dbtypeComboBox->setCurrentIndex(temp);

//        return true;
//    }

//    qWarning() << "Error while read from"<<config_f_name;

//    return false;
    QMap<QString,int> __settings_map;

    // fill the QMap container with key-values pairs
    if(adb_utility::get_settings_4rom_file(config_f_name,__settings_map)){
        int temp;

        // find necessary settings
        if((temp = __settings_map.value("timeout_reconnect"))!=-1)
            timeout_reconnect = temp;

        if((temp = __settings_map.value("CONNECTION_LOST_MESSAGE"))!=-1)
            CONNECTION_LOST_MESSAGE = temp;

        if((temp = __settings_map.value("DB_TYPE"))!=-1)
            this->ui->dbtypeComboBox->setCurrentIndex(temp);

        //return true;
    } else{

        qWarning() << "Error while read from"<<config_f_name;
        return false;
    }
    //------------------------------------------------------------------
    QMap<QString,QString> __settings_map_str;

    if(adb_utility::get_settings_4rom_file(config_f_name,__settings_map_str)){
        QString temp_s;

        if((temp_s = __settings_map_str.value("HOSTNAME"))!="")
            this->ui->Host_Form->setText(temp_s);

        return true;
    } else{

        qWarning() << "Error while read from"<<config_f_name<<"(Strings)";
    }
    return false;
}



void loginWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){

        if(cryptoModule_)
            cryptoModule_->getSavedLogins();
     }
        QMainWindow::keyPressEvent(event);
}

bool loginWindow::eventFilter(QObject *object, QEvent *event)
{
        if(object == ui->Login_Form && event->type() == QEvent::MouseButtonPress) {

            if(cryptoModule_)
                cryptoModule_->getSavedLogins();
        }

        return QMainWindow::eventFilter(object,event);
}


