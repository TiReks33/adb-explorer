#include "sqldump.h"

#include "databases.h"

SqlDump_credentials::SqlDump_credentials(auth& auth__,/*QWidget*/Databases *parent)
    : QDialog/*QWidget*/(parent)
    , auth_{auth__}

    //, settings_dialog_{new SqlDump_settings/*{this}*/}
{
    init_form();
    init_connections();

//    setAttribute(Qt::WA_DeleteOnClose, true);
}

SqlDump_credentials::~SqlDump_credentials()
{
    qDebug()<<"~SqlDump_credentials";

    //delete settings_dialog_;
}

void SqlDump_credentials::init_form()
{
    setWindowTitle("SQL dump");

    //setModal(true);
//    setWindowModality(Qt::WindowModal);

    setMinimumSize(320,240);

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout* layout{new QVBoxLayout{this}};

    setLayout(layout);

    label_ = new QLabel;

    label_->setText("Please, choose credential for dumping.");

    layout->addWidget(label_);

    current_credential_button_ = new QPushButton;
    another_credential_button_ = new QPushButton;


    layout->addWidget(current_credential_button_);

    current_credential_button_->setText("current_credential_button");

    layout->addWidget(another_credential_button_);

    another_credential_button_->setText("another_credential_button");

    exit_button_ = new QPushButton;

    exit_button_->setText("Cancel");

    layout->addWidget(exit_button_);

}

void SqlDump_credentials::init_connections()
{

    connect(exit_button_,&QPushButton::clicked,[=](){
        this->close();
    });

    connect(current_credential_button_,&QPushButton::clicked,[=](){
        //settings_dialog_->setModal(true);
        ///if(!settings_dialog_)
            settings_dialog_.reset(new SqlDump_settings{auth_,this}); // doesn't need to '(std::nothrow)' because smartpointer nulled it already.
        if(settings_dialog_){

            settings_dialog_->setModal(true);
            settings_dialog_->show();
            ///this->hide();
//            connect(settings_dialog_,&SqlDump_settings::closed,[=]{
//                this->show();
//            });

            //this->hide();
        } /*else {
            qDebug() << "Error[Allocating memory failed]";
            this->close();
        }*/
    });

    connect(this,&SqlDump_credentials::message,[=]{
        qDebug()<<"message signal from SqlDump_credentials";
    });

}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

SqlDump_settings::SqlDump_settings(auth& auth__,/*QWidget*/SqlDump_credentials *parent)
    : QDialog(parent)

    , parent_(parent)

    , auth_(auth__)

{
    init_form();
    init_connections();

//    setAttribute(Qt::WA_DeleteOnClose,true);
}

SqlDump_settings::~SqlDump_settings()
{
    qDebug()<<"~SqlDump_settings";
    //delete dump_name_window_;
}

void SqlDump_settings::init_form()
{
    setWindowTitle("SQL dump");

    //setModal(true);
//    setWindowModality(Qt::WindowModal);

    setMinimumSize(320,240);

    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QVBoxLayout* layout{new QVBoxLayout{this}};

    setLayout(layout);

    label_ = new QLabel;

    label_->setText("Please, choose what you want to backup(dump).");

    layout->addWidget(label_);

    radio_but_0 = new QRadioButton{"all_databases",this};
    radio_but_1 = new QRadioButton{"choose_dbs",this};
    radio_but_2 = new QRadioButton{"tables_of_chosen_db",this};

    layout->addWidget(radio_but_0);
    layout->addWidget(radio_but_1);
    layout->addWidget(radio_but_2);

    QHBoxLayout* actions_layout{new QHBoxLayout};


    close_button_ = new QPushButton;
    next_button_ = new QPushButton;

    actions_layout->addWidget(close_button_);

    close_button_->setText("back");

    actions_layout->addWidget(next_button_);

    next_button_->setText("next");

    layout->addLayout(actions_layout);

    radio_but_0->setChecked(true);
}

void SqlDump_settings::init_connections()
{
    connect(close_button_,&QPushButton::clicked,[=]{
        this->close();
    });

    connect(next_button_,&QPushButton::clicked, this, &SqlDump_settings::choose_settings_interactive);

}

void SqlDump_settings::choose_settings_interactive()
{
    // all_database
    if(radio_but_0->isChecked()){
        QStringList args;
        args << "--all-databases";
        set_dump_name(auth_,args,this);

        // choose_dbs
    } else if(radio_but_1->isChecked()){
        if(QScopedPointer<TwoListSelection> double_list{new TwoListSelection{auth_,this}}){

            connect(double_list.get(),&TwoListSelection::export_list,[&](QStringList list__){
                qDebug() << "List from double list::" << list__;
//                    QStringList args{"--databases " + escape_sql_backticks(pack_(list__," "))};
        QStringList args;
                args << "--databases" << escape_sql_backticks( list__ ); //!!!!

                qDebug()<<"LIST PACKED::"<<args;

                set_dump_name(auth_,args,this);

            });

            QString const query = QString("SHOW DATABASES");
            double_list->update_doublelist(query);

            double_list->setModal(true);
            double_list->show();
            double_list->exec();
        }
        // tables_of_chosen_Db
    } else if(radio_but_2->isChecked()){

        dump_db_choose_window_.reset(new SqlDump_db_choose{auth_});

                    if(dump_db_choose_window_){

                           connect(dump_db_choose_window_.get(),&SqlDump_db_choose::chosen_db_signal,this, &SqlDump_settings::chose_tables_from_db);

                           dump_db_choose_window_->setModal(true);
                           dump_db_choose_window_->show_db();
                           dump_db_choose_window_->show();


            }

    }

}

void SqlDump_settings::chose_tables_from_db(const QString &chosen_db__)
{
    db_connection::close(subconnection_name_); //1

    auth* auth_temp {nullptr}; // not static obj because 'customqueryresult' gets reference of auth, not value (without additional exec() local stack obj out-of-range ==>> UB (crashing app)
qDebug() << (auth_temp==nullptr);
    auth_temp = new auth{auth_};

    auth_temp->db_name_ = chosen_db__;

    QSqlDatabase::database(subconnection_name_,false).setDatabaseName(chosen_db__); //2

    //if(QScopedPointer<TwoListSelection> double_list{new TwoListSelection{(*auth_temp),/*this*/dump_db_choose_window_.get()}}){
    double_list_.reset(new TwoListSelection{(*auth_temp),dump_db_choose_window_.get()});
    if(double_list_){
//                                    double_list->setAttribute(Qt::WA_DeleteOnClose,true);

        connect(double_list_.get(),&TwoListSelection::export_list,[=](QStringList list__){
            qDebug() << "List from double list::" << list__;
//                                        QString const db_args_list = db_name + " " + escape_sql_backticks(pack_(list__," "));
//                                        qDebug() << "db_name==Null?" << (db_name==)
            qDebug() << "11111";

            QStringList args;
            args << chosen_db__ << escape_sql_backticks(list__);
            qDebug()<<"LIST PACKED::"<< args;

            //delete dump_name_window_;

            set_dump_name((*auth_temp),args,this);
        });

        QString const query = QString("SHOW TABLES");
        double_list_->update_doublelist(query,subconnection_name_);

        double_list_->setModal(true);
        double_list_->show();


        connect(double_list_.get(),&TwoListSelection::destroyed,[=]{
            delete auth_temp;
            qDebug() << "~auth_temp (in SqlDump_settings::radio_but2 handler)";
        });


        double_list_->exec();
    }


}

void SqlDump_settings::set_dump_name(auth & auth__, QStringList& args__, QWidget *parent__)
{
    dump_name_window_.reset(new SqlDump_name{auth__,args__,parent__});
    connect(dump_name_window_.get(),SIGNAL(message(QString const&)),parent_,SIGNAL(message(QString const&)));

                        if(dump_name_window_){
                            dump_name_window_->setModal(true);
                            dump_name_window_->show();

                        }
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


SqlDump_name::SqlDump_name(auth& auth__,/*QString const& args__,*/QStringList args__,QWidget* parent)
    : QDialog(parent)
    , auth_(auth__)
    , args_(args__)

{
    init_form();
    init_connections();

//    setAttribute(Qt::WA_DeleteOnClose,true);
}

SqlDump_name::~SqlDump_name()
{
    qDebug()<<"~SqlDump_name";

}

void SqlDump_name::init_form()
{
    setWindowTitle("Dump name setting");

    //setModal(true);

    setMinimumSize(320,120);

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout* layout = new QVBoxLayout;

    setLayout(layout);

    label_ = new QLabel;

    label_->setText("Set name for dump:");

    layout->addWidget(label_);

    line_edit_ = new QLineEdit(this);

    layout->addWidget(line_edit_);

    line_edit_->setPlaceholderText("dump.sql");

    button_box_ = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    layout->addWidget(button_box_);
}

void SqlDump_name::init_connections()
{
    connect(button_box_, &QDialogButtonBox::rejected,[=](){
        this->close();
    });

    connect(button_box_, &QDialogButtonBox::accepted,[=]{
//        emit message("test");
//        qDebug()<<"testtest";

        get_dump();
    });
}


void SqlDump_name::get_dump()
{
    QProcess dumpProcess(this);
    QStringList args_list;
    args_list << QString("-u"+auth_.login_) << QString("-h"+auth_.host_) << QString("-p"+auth_.passw_) << args_/*"--databases"<< "aatest1" << "abitura"*/;
    qDebug() << "args StringList->>" << args_list << "<--args StringList";
    QString output_filename = line_edit_->text().isEmpty() ? "dump.sql" : (line_edit_->text().right(4)==".sql") ? line_edit_->text() : line_edit_->text()+".sql" ;
    qDebug() << "right 4==" << line_edit_->text().right(4);

    dumpProcess.setStandardOutputFile(output_filename);


      QObject::connect(&dumpProcess,static_cast<void (QProcess::*) (int, QProcess::ExitStatus)>(&QProcess::finished),[&](int exitCode, QProcess::ExitStatus exitStatus){

          qDebug() << "signal QProcess::finished emited";

        if(exitStatus==QProcess::NormalExit && exitCode == 0){
            QString const status_message = "(✓) Dumping process finished successfully. Exit code: " + QString::number(exitCode) ;
        qDebug() << status_message;

        emit message(status_message);

//        dump_name->close();

        } else {
            QString const status_message =  "(x) Dumping process failed. Exit code: " + QString::number(exitCode)+". ";
            QString const detailed_err_desc ="Detailed error description: ";
            QString const stderr_text = dumpProcess.readAllStandardError() ;
            QString const full_err_message = status_message+detailed_err_desc+stderr_text;

            QPointer <QMessageBox> messageBox{new QMessageBox(QMessageBox::Warning,"Dumping process failed",status_message+detailed_err_desc+"<FONT COLOR='#ff0000'>"+'\"'+stderr_text+'\"'+"</FONT>"+'.',
                                                              QMessageBox::Ok/*,this*/)};

            connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
            messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
            messageBox->show();

            qCritical()<< "Error string: "<<dumpProcess.errorString();

            qWarning()<<"ReadAllStandardError::"<<stderr_text;

            emit message(full_err_message);

        }
        qDebug()<<"readAllStandardOutput::"<<dumpProcess.readAllStandardOutput();
    });

      connect(&dumpProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
      {
          qWarning() << "Occured error: " << error ;
      });

      dumpProcess.setReadChannel(QProcess::StandardOutput);

      connect(&dumpProcess, &QProcess::readyReadStandardError,[=](){});

      dumpProcess.start("mysqldump", args_list);
      dumpProcess.waitForFinished();




}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


SqlDump_db_choose::SqlDump_db_choose(auth& auth__,QWidget *parent)
    : QDialog(parent)

    //, parent_(parent)

    , auth_(auth__)

//    , model_{new QSqlQueryModel}

{
    init_form();
    init_connections();

//    show_db();

}

SqlDump_db_choose::~SqlDump_db_choose()
{
    qDebug()<<"~SqlDump_db_choose";
//    delete model_;
}

bool SqlDump_db_choose::show_db()
{
    if(db_connection::try_to_reopen(auth_)){

//        if(QPointer<QSqlQueryModel> model{new QSqlQueryModel}){

//            connect(this,&SqlDump_db_choose::destroyed,[=]{
//                delete model;
//                qDebug() << "~model(from SqlDump_db_choose)";
//            });

            if(db_connection::set_query("SHOW DATABASES",/*model*/&model_,comboBox_)){
                return true;
            }
//        }
    }
    return false;
}

void SqlDump_db_choose::init_form()
{
    setWindowTitle("Database choosing");

    QVBoxLayout* layout{new QVBoxLayout{}};

    setLayout(layout);

    label_ = new QLabel{"Choose database you want dump table/s from."};

    layout->addWidget(label_);

    comboBox_ = new QComboBox;

    comboBox_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    QHBoxLayout* connection_layout{new QHBoxLayout};

    connection_layout->addWidget(comboBox_);

    reload_button_ = new QPushButton;

    QVBoxLayout* button_rich_text_layout = new QVBoxLayout{reload_button_};

    button_rich_text_layout->setSizeConstraint(QLayout::SetMinimumSize);

    reload_button_->setLayout(button_rich_text_layout);

    QHBoxLayout *button_sublayout = new QHBoxLayout;

    button_rich_text_layout->addLayout(button_sublayout);

    QLabel* button_rich_text_label = new QLabel{"<u>R</u>eload"};

    button_rich_text_label->setAlignment(Qt::AlignVCenter | Qt::AlignVCenter); // or center
    button_rich_text_label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    button_rich_text_label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QIcon reload_icon = QApplication::style()->standardIcon(QStyle::SP_BrowserReload);

    QLabel* button_icon_label = new QLabel{};
    button_icon_label->setPixmap(reload_icon.pixmap(QSize(16,16)));
//    reload_button_->layout()->addWidget(button_rich_text_label);
    button_sublayout->addWidget(button_icon_label);
    button_sublayout->addWidget(button_rich_text_label);

    reload_button_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    connection_layout->addWidget(reload_button_);

//    layout->addWidget(comboBox_);

    layout->addLayout(connection_layout);

    buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok ,this);

    layout->addWidget(buttonBox_);

//    comboBox_->addItem("ABCabc123");

//    comboBox_->addItem("456DeFghIi");

}

void SqlDump_db_choose::init_connections()
{
    connect(buttonBox_,&QDialogButtonBox::rejected, this,&SqlDump_db_choose::reject);

    connect(reload_button_,&QPushButton::clicked,[=]{
        show_db();
        qDebug() << "reload_button clicked";
    });

    connect(buttonBox_,&QDialogButtonBox::accepted,[=]{
        emit chosen_db_signal(escape_sql_backticks( this->comboBox_->currentText()) );
        accept();
    });
}
