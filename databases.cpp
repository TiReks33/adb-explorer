#include "databases.h"
#include "ui_databases.h"



Databases::Databases(auth& auth__, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Databases)
  , statusBar(new scrolledStatusBar)
  //, db_connection_(info)
  , auth_(auth__)
  , tables_window_(new Tables(auth_))
  , new_db_window_(new create_db_name)
  , delete_db_window_(new delete_db)
{
    ui->setupUi(this);

    ui->statusBarLayout->addWidget(statusBar);

    statusBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    setWindowIcon(QIcon(":/pic/anthead2.png"));

    statusBar->get_line()->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                        | Qt::WindowMinimizeButtonHint
                                        | Qt::WindowMaximizeButtonHint
                                        | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    init_signals();

    ui->tableView->setSelectionMode(QTableView::SingleSelection);

    // get actual configs from files:

    // "databases.cfg"
    if(!read4rom_settings_file())
        write2_settings_file();

    // "adb_utility.cfg"
    if(!adb_utility::read4rom_settings_file())
        adb_utility::write2_settings_file();

}

Databases::~Databases()
{
    db_connection::close();   // close SQL-connection 'auth::con_name_'
    db_connection::remove(); // remove SQL-connection 'auth::con_name_'
    delete ui;
    delete tables_window_;
    delete new_db_window_;
    delete delete_db_window_;
}

void Databases::keyPressEvent(QKeyEvent *e) {
    auto key = e->key();
    if(key == Qt::Key_Escape){
        close();
    } else if(key == Qt::Key_Down || key == Qt::Key_Up) {
        emit ui->tableView->clicked(/*QModelIndex()*/ui->tableView->currentIndex());
    } /*else if(key == Qt::Key_Enter || key == Qt::Key_Return){

    }*/ else {
        QDialog::keyPressEvent(e);
    }
}


//void Databases::get_information_window(const QString & header_text__, const QString & main_text__, QWidget *parent__, enum QMessageBox::Icon messageBoxType__)
//{
//    QPointer <QMessageBox> messageBox{new QMessageBox(messageBoxType__,header_text__,
//                                                      main_text__,
//                                                      QMessageBox::Ok,parent__)};

////    connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
//    messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
//    messageBox->setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
//    messageBox->setModal(false);
//    messageBox->show();
//}


void Databases::init_signals()
{

    // reload database list
    connect(ui->showDB_button,&QPushButton::clicked,[=]{
        show_databases();
    });

    // create new database form call
    connect(ui->create_db_button,&QPushButton::clicked,[=]{
        new_db_window_->setModal(true);
        new_db_window_->show();
    });

    // create new database with chosen name
    connect(new_db_window_,
            static_cast<void (create_db_name::*)(QString const&,QString const&)>(&create_db_name::create_db_signal),
            [=](QString const & query__,QString const& newdb_name__){

        db_connection::open(auth_);

        if(!db_connection::set_query(query__,&model_,ui->tableView,QHeaderView::Stretch)){

            QString const warning_text = QString("Database `%1` is not created. Please check name and try again.").arg(newdb_name__);

            //adb_utility::get_information_window("Database is not created",warning_text, windowFlags(), this,QMessageBox::Warning);

            statusBar->get_line()->setText(warning_text);

        }

        show_databases();
    });

    // call child, set current selected DB and show tables on child's form
    connect(this,&Databases::show_tables_signal,this,[=]{
        db_connection::close();
        QSqlDatabase::database(auth::con_name_,false).setDatabaseName(auth_.db_name_);
        tables_window_->show_tables();

    },Qt::QueuedConnection);

    // handler for closing 'table' window signal
    connect(tables_window_,&Tables::db_show,[=]{
        show_databases();
        this->show();
    });

    // signal-slot chain for delete chosen database
    connect(this,SIGNAL(delete_form_request()),delete_db_window_,SLOT(delete_form_request_slot()));

    // reload database list after deletion
    connect(delete_db_window_,&delete_db::delete_form_send,this,[=](QComboBox* comboBox__){
        db_connection::open(auth_);

        db_connection::set_query("SHOW DATABASES;",&model_,comboBox__);

        comboBox__->setCurrentIndex(-1); //for blank cell default

    });

    // signal-slot chain for delete chosen database(2)
    connect(delete_db_window_,&delete_db::delete_entity,[=](QComboBox*comboBox__){
            db_connection::open(auth_);


            QString const chosen_db = comboBox__->currentText();

            QString const query_text = QString("DROP DATABASE `%1`").arg(QString(escape_sql_backticks(chosen_db)));


            if(!db_connection::set_query(query_text,&model_,comboBox__)){
                statusBar->get_line()->setText(QString("(x)Database `%1` is not dropped.").arg(chosen_db));
                qDebug()<< QString("(x)Database `%1` is not dropped.").arg(chosen_db);
                //QMessageBox::warning(this,"Warning","Database is not deleted. May be it was been deleted earlier.");

            } else {
                qDebug() << QString("(✓)Database `%1` successfully deleted.").arg(chosen_db);
                statusBar->get_line()->setText(QString("(✓)Database `%1` successfully deleted.").arg(chosen_db));
            }

            show_databases(); // view database after deletion
    });


    connect(ui->showTables_button,&QPushButton::clicked,[=]{

            if(close_chld_wndws_on_next){
                auto confirm = QMessageBox::information(this,"Confirm choosing","Are you want to choose database \""+auth_.db_name_+"\" and go to the next form?",
                                            QMessageBox::Yes | QMessageBox::No);
                qDebug()<<"After QMessageBox";
                if(confirm!=QMessageBox::Yes)
                    return;
            }

        tables_window_->show();
        this->hide();

        // close all queries form/results from 'database' window
        emit close_all_custom_windows_();

        emit show_tables_signal();
    });

    // mouse double-click handler
    connect(ui->tableView,&QTableView::activated,[=](const QModelIndex &index){

        ////qDebug()<<"ACTIVATED::index=="<<index;

        ////QString val=ui->tableView->model()->data(index).toString();

        ////statusLine->setText("Database activated: "+val);

        ui->tableView->setCurrentIndex(index);

        auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

        statusBar->get_line()->clear();

        //////ui->showTables_button->click();
    });

    // mouse left-click handler
    connect(ui->tableView,&QTableView::clicked,[=](const QModelIndex &index){

        ui->tableView->setCurrentIndex(index);

        auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

        statusBar->get_line()->clear();

        ////qDebug() << "CLICKED::::::::::::" << ui->tableView->currentIndex().row() << "::" << auth_.db_name_;
    });

    connect(ui->tableView,&QTableView::doubleClicked,[=](const QModelIndex &index){

        ui->tableView->setCurrentIndex(index);

        auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

        statusBar->get_line()->clear();

        ui->showTables_button->click();
    });

    connect(ui->delete_db_button,&QPushButton::clicked,[=]{
        delete_db_window_->setModal(true);
        delete_db_window_->show();
        emit delete_form_request();
    });

    // custom query to SQL server button handler
    connect(ui->queryButton, &QPushButton::clicked,this, &Databases::get_query_wndw);

    connect(ui->settingsButton,&QPushButton::clicked, this, &Databases::showSettings);

}


void Databases::message_to_status(const QString & message__) const
{
    statusBar->get_line()->clear();
    statusBar->get_line()->insert(message__);
}


/*void*/bool Databases::show_databases()
{

    if(db_connection::open(auth_)){

        if(db_connection::set_query("SHOW DATABASES;",&model_,ui->tableView,QHeaderView::Stretch)){

            // 1st cell in model default selected
            select_cells(0,0, ui->tableView);

            auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

            ////qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

            ////qDebug() << "current auth_.dB_name_ index::" << auth_.db_name_;

            if(!auth_.db_name_.isEmpty()){
                ui->showTables_button->setEnabled(true);
                ui->showTables_button->setStyleSheet("background: green; color: white;");
            }

            return true;
        }
    }

    return false;
}



void Databases::on_mysqldump_button_clicked()
{
    //wip
    if(auth_.db_driver_!="QMARIADB"&& auth_.db_driver_!="QMYSQL" && auth_.db_driver_!="QMYSQL3"){
        QMessageBox::warning(this,"This database dump currently unavailable","ADB Explorer's dump function"
                                    " doesn't support yet this database server (current driver: \""+auth_.db_driver_+"\").",
                                    QMessageBox::Ok);
        qDebug()<<"After QMessageBox";
        return;
    }

    QScopedPointer<SqlDump_credentials> dump{new SqlDump_credentials{auth_,this}};
//dump->setAttribute(Qt::WA_DeleteOnClose,true);

    // message with resultation info of successful/unsuccessful dump
    connect(dump.get(),&SqlDump_credentials::message,[&](QString const& message__){
        statusBar->get_line()->setText(message__);
        qDebug()<<message__;
        dump->close();
    });

    dump->setModal(true);
    dump->show();
    dump->exec();

//    QEventLoop loop;
//        connect(dump, &SqlDump_credentials::closed, & loop, &QEventLoop::quit);
//        loop.exec();

    return;

}

void Databases::get_query_wndw()
{

    QPointer<Custom_Query> custom_query_window{new Custom_Query{this}};

    if(query2server_note){
        custom_query_window->add_note("*Note: if you need to operate with certain database or tables in it, "
                         "you must choose database from 'Database' window and click 'Show tables in selected database'.");

        connect(custom_query_window,&Custom_Query::dont_show_note,[=]{ query2server_note=false; write2_settings_file();  });
    }

    // call overload signal on overload slot
    connect(custom_query_window,static_cast<void(Custom_Query::*)(/*QString,*/Custom_Query *)>(&Custom_Query::send_custom_query),
            this,static_cast<void(Databases::*)(/*QString,*/Custom_Query *)>(&Databases::send_custom_query_slot));


    if(close_chld_wndws_on_next){
    // closing all forms by parent signal
    connect(this,&Databases::close_all_custom_windows_, custom_query_window , &Custom_Query::close, Qt::QueuedConnection);
    }


    connect(this,&Databases::window_rise_signal,[=]{
        // if object exist, raise it in front of other windows
        if(custom_query_window)
            custom_query_window->raise();

    });

    custom_query_window->setAttribute( Qt::WA_DeleteOnClose, true );
    custom_query_window->setModal(false);
    custom_query_window->show();
//    custom_query_window->exec();

}


void Databases::send_custom_query_slot(Custom_Query *custom_query_window__)
{
// dummy counter for multiply windows
static int wndw_cnt=0;
//wndw_cnt++;

    // dynamic allocated copy of parent auth for preventing dangling (when local var goes out of scope)
    ////auth* auth_copy{new auth};
    std::unique_ptr<auth> auth_copy{new (std::nothrow) auth};

    if(auth_copy.get()){

        auth_copy->copy_(auth_);

        // delete db_name_ for logical reason
        auth_copy->db_name_.clear();

    ////    connect(this,&Databases::destroyed,[&]{delete auth_copy;auth_copy=nullptr;});

        QScopedPointer<CustomQueryResult> new_result_window{new CustomQueryResult{{*auth_copy},this}};

        if(new_result_window.get()){

            wndw_cnt++;

            connect(new_result_window.get(),&CustomQueryResult::destroyed,[&]{

    ////            delete auth_copy;
    ////            auth_copy = nullptr;

                // if all windows was closed -- close and remove sub-connection to SQL server
                if(!(--wndw_cnt)){
                    db_connection::close(subconnection_name_);
                    db_connection::remove(subconnection_name_);
                }
//                qDebug() << "wndw_cnt" << wndw_cnt;
//                qDebug()<< "~new_result_window (destructor)";
            });


            if(close_chld_wndws_on_next){
            //closing all forms by parent signal(2)
            connect(this,&Databases::close_all_custom_windows_, new_result_window.get() , &CustomQueryResult::close, Qt::QueuedConnection);
            }


            // send query and place result to form of new allocated object
            new_result_window->custom_query_slot(custom_query_window__->get_text(),subconnection_name_);

            // check error of query
            if((new_result_window->ui->tableView->model())!=nullptr) {

                ////qDebug() << "Number of columns in tableView->model()::"<<new_result_window->ui->tableView->model()->columnCount();
                ////qDebug() << "Number of rows in tableView->model()::"<<new_result_window->ui->tableView->model()->rowCount();

                custom_query_window__->close_window();

                // check if result query is empty
                if(!new_result_window->ui->tableView->model()->rowCount()){

                    qDebug() << "(✓) rowCount() in model_==0::display result ignored.";

                    statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                            " contain of displayable result.");

                } else{

                    statusBar->get_line()->clear();

                    new_result_window->show();
                    new_result_window->exec();
                }

            } else{
                // if query exec failed, show form again (to correct query)
                emit window_rise_signal();
            }

        }
    }
}



void Databases::showSettings()
{
    if(QPointer<QDialog> settings_dial{new(std::nothrow) QDialog{this}}){

        settings_dial->setAttribute(Qt::WA_DeleteOnClose,true);

        settings_dial->setWindowTitle("Settings");

        QVBoxLayout* main_lay = new QVBoxLayout;

        settings_dial->setLayout(main_lay);

        QHBoxLayout* closeNonModal_lay = new QHBoxLayout;

        main_lay->addLayout(closeNonModal_lay);

        QCheckBox* closeNonModalCB = new QCheckBox;
        closeNonModalCB->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

        ClickableLabel* CloseNonModalCB_lbl = new ClickableLabel("Close all query forms after database chosing");
        CloseNonModalCB_lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        CloseNonModalCB_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        closeNonModal_lay->addWidget(closeNonModalCB);
        closeNonModal_lay->addWidget(CloseNonModalCB_lbl);

        closeNonModalCB->setChecked(close_chld_wndws_on_next);

        connect(CloseNonModalCB_lbl,&ClickableLabel::clicked,[=]{
            closeNonModalCB->setChecked(!closeNonModalCB->isChecked());
        });


        QHBoxLayout* errorTimeoutLay = new QHBoxLayout;

        ClickableLabel* errorTimeoutLbl = new ClickableLabel("Timeout for SQL queries errors pop-up messages");
        errorTimeoutLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        errorTimeoutLbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        errorTimeoutLay->addWidget(errorTimeoutLbl);

        QLineEdit* errorTimeoutLine = new QLineEdit;
        errorTimeoutLine->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

        // only digits validation
        QIntValidator* int_validator = new QIntValidator(1000, 2073600000);

        errorTimeoutLine->setValidator( int_validator );

        connect(errorTimeoutLine, &QLineEdit::destroyed,[=]{ delete int_validator; /*qDebug() << "~int_validator";*/ });

        errorTimeoutLay->addWidget(errorTimeoutLine);

        errorTimeoutLine->setText(QString::number(adb_utility::_CUSTOM_MESSAGE_BOX_TIMER_));

        main_lay->addLayout(errorTimeoutLay);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

        main_lay->addWidget(buttonBox);

        connect(buttonBox, &QDialogButtonBox::rejected,settings_dial,&QDialog::reject);

        connect(buttonBox, &QDialogButtonBox::accepted,[=]{
            close_chld_wndws_on_next = closeNonModalCB->isChecked();
            bool ok;
            int temp = errorTimeoutLine->text().toInt(&ok);
            if(ok){
                /*new*/ adb_utility::_CUSTOM_MESSAGE_BOX_TIMER_ = temp;
                    adb_utility::write2_settings_file();
            }
            write2_settings_file();

            settings_dial->accept();
        });

        settings_dial->setModal(true);
        settings_dial->show();
    }
}

bool Databases::read4rom_settings_file()
{
    QMap<QString,int> __settings_map;

    if(get_settings_4rom_file(settings_f_name_,__settings_map)){
        int temp;

        if((temp = __settings_map.value("close_chld_wndws_on_next"))!=-1)
            close_chld_wndws_on_next = temp;

        if((temp = __settings_map.value("query2server_note"))!=-1)
            query2server_note = temp;

        return true;
    }

    qWarning() << "Error while read from"<<settings_f_name_;

    return false;
}

void Databases::write2_settings_file()
{
    QFile outFile(settings_f_name_);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << "close_chld_wndws_on_next" << '=' << QString::number(close_chld_wndws_on_next) << Qt::endl;
    textStream << "query2server_note" << '=' << QString::number(query2server_note) << Qt::endl;
}


