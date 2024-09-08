#include "databases.h"
#include "ui_databases.h"



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

    Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                        | Qt::WindowMinimizeButtonHint
                                        | Qt::WindowMaximizeButtonHint
                                        | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    delete_db_window_->setWindowTitle("Remove database dialog");

    //SIGNALS

    init_signals();


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


void Databases::init_signals()
{

    connect(ui->showDB_button,&QPushButton::clicked,[=]{
        show_databases();
    });



    connect(ui->create_db_button,&QPushButton::clicked,[=]{
        new_db_window_->setModal(true);
        new_db_window_->show();
    });

    //connect(new_db_window_,SIGNAL(create_db_signal(QString const&)),this,SLOT(create_db_slot(QString const&)));
    connect(new_db_window_,&create_db_name::create_db_signal,[=](QString const & query__){

        db_connection::open(auth_);

        if(!db_connection::set_query(query__,&model_,ui->tableView,QHeaderView::Stretch)){

            QMessageBox::warning(this,"Warning","Database is not created. Please check name and try again.");

        }

        show_databases();
    });




    connect(this,SIGNAL(show_tables_signal()),tables_window_,SLOT(show_tables()));

    connect(tables_window_,&Tables::db_show,[=]{
        show_databases();
        this->show();
    });



    connect(this,SIGNAL(delete_form_request()),delete_db_window_,SLOT(delete_form_request_slot()));


    connect(delete_db_window_,&delete_db::delete_form_send,this,[=](QComboBox* comboBox__){
        db_connection::open(auth_);

        db_connection::set_query("SHOW DATABASES;",&model_,comboBox__);

        comboBox__->setCurrentIndex(-1); //for blank cell default

    });



    connect(delete_db_window_,&delete_db::delete_entity,[=](QComboBox*comboBox__){
            db_connection::open(auth_);


            QString const chosen_db = comboBox__->currentText();

            QString const query_text = QString("DROP DATABASE `%1`").arg(QString(escape_sql_backticks(chosen_db)));


            if(!db_connection::set_query(query_text,&model_,comboBox__)){
                ui->statusLine->setText(QString("(x)Database `%1` is not dropped.").arg(chosen_db));
                qDebug()<< QString("(x)Database `%1` is not dropped.").arg(chosen_db);
                //QMessageBox::warning(this,"Warning","Database is not deleted. May be it was been deleted earlier.");

            } else {
                qDebug() << QString("(✓)Database `%1` successfully deleted.").arg(chosen_db);
                ui->statusLine->setText(QString("(✓)Database `%1` successfully deleted.").arg(chosen_db));
            }

            show_databases(); // view database after deletion
    });


    connect(ui->showTables_button,&QPushButton::clicked,[=]{
        tables_window_->show();
        emit show_tables_signal();
        this->hide();
    });

    connect(ui->tableView,&QTableView::activated,[=](const QModelIndex &index){

        qDebug()<<"ACTIVATED::index=="<<index;

        QString val=ui->tableView->model()->data(index).toString();

        ui->statusLine->setText("Database activated: "+val);
    });

    connect(ui->tableView,&QTableView::clicked,[=](const QModelIndex &index){

        ui->tableView->setCurrentIndex(index);

        auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

        qDebug() << "CLICKED::::::::::::" << ui->tableView->currentIndex() << "::" << auth_.db_name_;
    });

    connect(ui->delete_db_button,&QPushButton::clicked,[=]{
        delete_db_window_->setModal(true);
        delete_db_window_->show();
        emit delete_form_request();
    });

}


void Databases::message_to_status(const QString & message__) const
{
    ui->statusLine->clear();
    ui->statusLine->insert(message__);
}


void Databases::message_from_login(QString const& message)
{
    message_to_status(message);
    show_databases();
}



void Databases::show_databases()
{
    db_connection::open(auth_);


    db_connection::set_query("SHOW DATABASES;",&model_,ui->tableView,QHeaderView::Stretch);

    qDebug() << "Contains?:"<<QSqlDatabase::contains(auth::con_name_);

    select_cells(0,0, ui->tableView);

    auth_.db_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

    qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

    qDebug() << "current auth_.dB_name_ index::" << auth_.db_name_;

    if(!auth_.db_name_.isNull()){
        ui->showTables_button->setEnabled(true);
        ui->showTables_button->setStyleSheet("background: green; color: white;");
    }
}



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

        line_edit->setPlaceholderText("dump.sql");

        QDialogButtonBox * button_box = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

        connect(button_box, &QDialogButtonBox::rejected,[&](){ dump_name->close(); });

        dump_name->layout()->addWidget(button_box);

        connect(button_box, &QDialogButtonBox::accepted,[&](){
                QProcess dumpProcess(this);
                QStringList args;
                args << QString("-u"+auth_.login_) << QString("-h"+auth_.host_) << QString("-p"+auth_.passw_) << "--databases" << "aatest1" << "abitura";
                qDebug() << "args StringList->>" << args << "<<-args StringList";
                QString output_filename = line_edit->text().isEmpty() ? "dump.sql" : (line_edit->text().right(4)==".sql") ? line_edit->text() : line_edit->text()+".sql" ;
                qDebug() << "right 4==" << line_edit->text().right(4);

                dumpProcess.setStandardOutputFile(/*"Qtaatest1.sql"*/output_filename);

                qDebug() << "signal accepted emited";

//                QObject::connect(&dumpProcess,QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),[&](int exitCode, QProcess::ExitStatus exitStatus){
                  QObject::connect(&dumpProcess,static_cast<void (QProcess::*) (int, QProcess::ExitStatus)>(&QProcess::finished),[&](int exitCode, QProcess::ExitStatus exitStatus){
                    qDebug() << "signal QProcess::finished emited";
                    QPointer<Databases> grand_parent = qobject_cast<Databases *>(dump_auth_choose->parent());
                    if(exitStatus==QProcess::NormalExit && exitCode == 0){
                        QString const status_message = "(✓) Dumping process finished successfully. Exit code: " + QString::number(exitCode) ;
                    qDebug() << status_message;
                    dump_name->close();

                    //QPointer<Databases> grand_parent = qobject_cast<Databases *>(dump_auth_choose->parent());
                    if(grand_parent) grand_parent->message_to_status(status_message) ;

                    dump_auth_choose->close();
                    } else {
                        QString const status_message =  "(x) Dumping process failed. Exit code: " + QString::number(exitCode)+". ";
                        QString const detailed_err_desc ="Detailed error description: ";
                        QString const stderr_text = dumpProcess.readAllStandardError() ;
                        QString const full_err_message = status_message+detailed_err_desc+stderr_text;
                        //QMessageBox::warning(this,"Dumping process failed",status_message);
                        QPointer <QMessageBox> messageBox{new QMessageBox(QMessageBox::Warning,"Dumping process failed",status_message+detailed_err_desc+"<FONT COLOR='#ff0000'>"+'\"'+stderr_text+'\"'+"</FONT>"+'.',
                                                                          QMessageBox::Ok,this)};

                        connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
                        messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
                        messageBox->show();
                        ////messageBox->setAttribute( Qt::WA_DeleteOnClose, true );

                        if(grand_parent) grand_parent->message_to_status(full_err_message) ;
                        qCritical()<< "Error string: "<<dumpProcess.errorString();

                        qWarning()<<"ReadAllStandardError::"<<stderr_text;
                    }
                    qDebug()<<"readAllStandardOutput::"<<dumpProcess.readAllStandardOutput();
                });

                  connect(&dumpProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
                  {
                      qWarning() << "Occured error: " << error ;
                  });

                  dumpProcess.setReadChannel(QProcess::StandardOutput);

                  connect(&dumpProcess, &QProcess::readyReadStandardError,[=](){});

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
