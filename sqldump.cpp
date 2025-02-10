#include "sqldump.h"

#include "databases.h"

SqlDump_credentials::SqlDump_credentials(auth& auth__,/*QWidget*/Databases *parent)
    : QDialog/*QWidget*/(parent)
    , auth_{auth__}

{
    init_form();
    init_connections();

}

SqlDump_credentials::~SqlDump_credentials()
{
//    qDebug()<<"~SqlDump_credentials";

}

void SqlDump_credentials::init_form()
{
    setWindowTitle("SQL dump");

    setMinimumSize(320,120);

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout* layout{new QVBoxLayout{this}};

    setLayout(layout);

    label_ = new QLabel;

    label_->setText("Please, choose credential for dumping.");
    label_->setStyleSheet("min-height:16px;color:darkslategray; border :2px solid black; padding: 6px; border-style : dashed");

    label_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    label_->setAlignment(Qt::AlignCenter);

    layout->addWidget(label_);

    current_credential_button_ = new QPushButton;
    another_credential_button_ = new QPushButton;


    layout->addWidget(current_credential_button_);

    current_credential_button_->setText("Current credential data");

    layout->addWidget(another_credential_button_);

    another_credential_button_->setText("Another credential data");

    exit_button_ = new QPushButton;

    exit_button_->setText("Cancel");

    layout->addWidget(exit_button_);

    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {

                obj->setStyleSheet(QStringLiteral("QPushButton {background: floralwhite; color:darkslategray; font-weight:bold;} %2")
//                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
        }
}

void SqlDump_credentials::init_connections()
{

    connect(exit_button_,&QPushButton::clicked,[=](){
        this->close();
    });

    // select current sql user and go to the dump settings choose
    connect(current_credential_button_,&QPushButton::clicked,[=](){

            settings_dialog_.reset(new SqlDump_settings{auth_,this}); // doesn't need to '(std::nothrow)' because smartpointer nulled it already.
        if(settings_dialog_){

            settings_dialog_->setModal(true);
            settings_dialog_->show();

        } /*else {
            qDebug() << "Error[Allocating memory failed]";
            this->close();
        }*/
    });

    // enter another credentials for dump
    connect(another_credential_button_,&QPushButton::clicked,this,&SqlDump_credentials::get_another_credentials_window);

}

void SqlDump_credentials::get_another_credentials_window()
{
    QPointer<QDialog> ac_dialog = new QDialog{this};

    ac_dialog->setAttribute(Qt::WA_DeleteOnClose,true);
//    connect(ac_dialog,&QDialog::destroyed,[=]{qDebug()<<"~ac_dialog";});

    QVBoxLayout* main_layout = new QVBoxLayout;

    QGridLayout* ac_layout = new QGridLayout;

    ac_dialog->setLayout(main_layout);

    main_layout->addLayout(ac_layout);

    QLabel* ac_login_lbl = new QLabel("Login");
    QLabel* ac_passw_lbl = new QLabel("Password");
    QLineEdit* login_form = new QLineEdit;
    QLineEdit* passw_form = new QLineEdit;
    /*ac_login_lay*/ac_layout->addWidget(ac_login_lbl,0,0);
    ac_login_lbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    /*ac_login_lay*/ac_layout->addWidget(login_form,0,1);
    /*ac_passw_lay*/ac_layout->addWidget(ac_passw_lbl,1,0);
    ac_passw_lbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    /*ac_passw_lay*/ac_layout->addWidget(passw_form,1,1);

    QCheckBox* checkBox = new QCheckBox{"hide"};

    connect(checkBox,&QCheckBox::stateChanged,[=](int state_arg__){
        if (state_arg__ == Qt::Checked)
            passw_form->setEchoMode(QLineEdit::Password);
        else
            passw_form->setEchoMode(QLineEdit::Normal);
    });

    checkBox->setChecked(true);



    ac_layout->addWidget(checkBox,1,2);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    ac_layout->addWidget(buttonBox,2,1);

    QLabel* status_lbl = new QLabel;

    status_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    status_lbl->setStyleSheet("color:red; border :2px solid black; border-style : dashed");

    status_lbl->setWordWrap(true);

    main_layout->addWidget(status_lbl);

    status_lbl->hide();

    connect(login_form,&QLineEdit::textChanged,[=]{if(/*!status_lbl->text().isEmpty()*/status_lbl->isVisible()){status_lbl->hide();ac_dialog->adjustSize();/*status_lbl->clear();*//*qDebug() << "clear1";*/}});
    connect(passw_form,&QLineEdit::textChanged,[=]{if(/*!status_lbl->text().isEmpty()*/status_lbl->isVisible()){status_lbl->hide();ac_dialog->adjustSize();/*status_lbl->clear();*//*qDebug() << "clear2";*/}});


    main_layout->setSpacing(3);


    connect(buttonBox, &QDialogButtonBox::rejected, ac_dialog, &QDialog::reject);


    connect(buttonBox, &QDialogButtonBox::accepted,[=]{

        auth* _auth{nullptr};

        _auth = new auth{auth_};

        _auth->login_ = login_form->text();
        _auth->passw_ = passw_form->text();

        connect(this,&SqlDump_credentials::destroyed,[=]{
            delete _auth;
            //qDebug() << "~_auth";
        });

    this->setCursor(Qt::WaitCursor);
        if(db_connection::open((*_auth),alt_con_name)){

            settings_dialog_.reset(new SqlDump_settings{(*_auth),this}); // doesn't need to '(std::nothrow)' because smartpointer nulled it already.
            if(settings_dialog_){

                settings_dialog_->setModal(true);
                settings_dialog_->show();
                ac_dialog->close();

                connect(settings_dialog_.get(),&SqlDump_settings::destroyed,[=]{
                    db_connection::close(alt_con_name);
                    db_connection::remove(alt_con_name);
                });
            }
        } else{
            qDebug() << "Alt connection not opened.";
            ////QMessageBox::information(ac_dialog,"Logging failed","Error while connecting to SQL Server with entered credentials.",QMessageBox::Ok);
            status_lbl->setText("Error while logging to SQL Server. Please, check entered credentials and try again.");
            status_lbl->show();
        }
    this->setCursor(Qt::ArrowCursor);

    });


// Styling
    QList<QPushButton*> ButtonsInFormlist = ac_dialog->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {

                obj->setStyleSheet(QStringLiteral("QPushButton {background: floralwhite; color:darkslategray; font-weight:bold;} %2")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
        }

    QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }
//

    ac_dialog->setWindowTitle("Enter credentials");

    ac_dialog->setModal(true);
    ac_dialog->show();
    ac_dialog->exec();
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

SqlDump_settings::SqlDump_settings(auth& auth__,/*QWidget*/SqlDump_credentials *parent)
    : QDialog(parent)

    , parent_(parent)

    , auth_(auth__)

{
    init_form();
    init_connections();

}

SqlDump_settings::~SqlDump_settings()
{
   // qDebug()<<"~SqlDump_settings";
}

void SqlDump_settings::init_form()
{
    setWindowTitle("SQL dump");


    setMinimumSize(320,240);

    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    QVBoxLayout* layout{new QVBoxLayout{this}};

    setLayout(layout);

    label_ = new QLabel;

    label_->setText("Please, choose what you want to backup(dump):\n1) all databases in current SQL server;\n2) select specific DBs;\n3) select specific tables in chosen DB.");

    label_->setWordWrap(true);

    label_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    label_->setAlignment(Qt::AlignCenter);
    label_->setStyleSheet("color:darkslategray; border :2px solid black; padding: 6px; border-style : dashed");

    layout->addWidget(label_);

    radio_but_0 = new QRadioButton{"All databases",this};
    radio_but_1 = new QRadioButton{"Choose databases",this};
    radio_but_2 = new QRadioButton{"Select tables from chosen DB",this};

    auto rButtonsSubFrame = new QFrame{this};
    auto radioSubLay = new QVBoxLayout{rButtonsSubFrame};
    rButtonsSubFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    radioSubLay->addWidget(radio_but_0);
    radioSubLay->addWidget(radio_but_1);
    radioSubLay->addWidget(radio_but_2);


    layout->addWidget(rButtonsSubFrame);

    QHBoxLayout* actions_layout{new QHBoxLayout};


    close_button_ = new QPushButton;
    next_button_ = new QPushButton;

    actions_layout->addWidget(close_button_);

    close_button_->setText("back");

    actions_layout->addWidget(next_button_);

    next_button_->setText("next");

    layout->addLayout(actions_layout);

    radio_but_0->setChecked(true);

    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {

                obj->setStyleSheet(QStringLiteral("QPushButton {background: floralwhite; color:darkslategray; font-weight:bold;} %2")
//                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
        }

    QList<QRadioButton*> RButtonsInFormlist = this->findChildren<QRadioButton*>();
        foreach (auto obj, RButtonsInFormlist) {

                obj->setStyleSheet(adb_style::adbRadioStyleSheet);
        }
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
               // qDebug() << "List from double list::" << list__;

        QStringList args;
                args << "--databases" << adb_utility::escape_sql_backticks( list__ ); //!!!!

               // qDebug()<<"LIST PACKED::"<<args;

                set_dump_name(auth_,args,this);

            });

            double_list->setWindowTitle("Choose databases for dump");

            QString const query = QString("SHOW DATABASES");
            double_list->update_doublelist(query);


            // styling
            QList<QPushButton*> ButtonsInDoublelist = double_list->findChildren<QPushButton*>();
                foreach (auto obj, ButtonsInDoublelist) {

                    obj->setStyleSheet(QStringLiteral("QPushButton:!disabled { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                        .arg(adb_style::getbuttonKhakiHiglightSS()));

                }

            QList<QListWidget*> listsInDoublelist = double_list->findChildren<QListWidget*>();
                foreach (auto obj, listsInDoublelist) {

                    obj->setAlternatingRowColors(true);
                    obj->setPalette(QPalette(adb_style::whiteUndGrayColor));

                }
            //


            double_list->setModal(true);
            double_list->show();
            double_list->exec();
        }
        // tables_of_chosen_Db
    } else if(radio_but_2->isChecked()){

        dump_db_choose_window_.reset(new SqlDump_db_choose{auth_,this});//{auth_});

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
    db_connection::close(subconnection_name_);

    auth* auth_temp {nullptr}; // not static obj because 'customqueryresult' gets reference of auth, not value (without additional exec() local stack obj out-of-range ==>> UB (crashing app)
//qDebug() << (auth_temp==nullptr);
    auth_temp = new auth{auth_};

    auth_temp->db_name_ = chosen_db__;

    QSqlDatabase::database(subconnection_name_,false).setDatabaseName(chosen_db__);

    double_list_.reset(new TwoListSelection{(*auth_temp),dump_db_choose_window_.get()});
    if(double_list_){

        connect(double_list_.get(),&TwoListSelection::export_list,[=](QStringList list__){
            //qDebug() << "List from double list::" << list__;

            QStringList args;
            args << chosen_db__ << adb_utility::escape_sql_backticks(list__);
           // qDebug()<<"LIST PACKED::"<< args;

            set_dump_name((*auth_temp),args,this);
        });

        QString const query = QString("SHOW TABLES");
        double_list_->update_doublelist(query,subconnection_name_);

        double_list_->setWindowTitle("Choose tables from selected database");


        // styling
        QList<QPushButton*> ButtonsInDoublelist = double_list_->findChildren<QPushButton*>();
            foreach (auto obj, ButtonsInDoublelist) {

                obj->setStyleSheet(QStringLiteral("QPushButton:!disabled { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));

            }

        QList<QListWidget*> listsInDoublelist = double_list_->findChildren<QListWidget*>();
            foreach (auto obj, listsInDoublelist) {

                obj->setAlternatingRowColors(true);
                obj->setPalette(QPalette(adb_style::whiteUndGrayColor));

            }
        //


        double_list_->setModal(true);
        double_list_->show();


        connect(double_list_.get(),&TwoListSelection::destroyed,[=]{
            delete auth_temp;
           // qDebug() << "~auth_temp (in SqlDump_settings::radio_but2 handler)";
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

}

SqlDump_name::~SqlDump_name()
{
   // qDebug()<<"~SqlDump_name";

}

void SqlDump_name::init_form()
{
    setWindowTitle("Dump name setting");

    setMinimumSize(320,120);

    setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QVBoxLayout* layout = new QVBoxLayout;

    setLayout(layout);

    label_ = new QLabel;

    label_->setText("Set name for dump:");
    label_->setStyleSheet("min-height:16px;color:darkslategray; border :2px solid black; padding: 6px; border-style : dashed");

    label_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    label_->setAlignment(Qt::AlignCenter);


    layout->addWidget(label_);

    line_edit_ = new QLineEdit(this);

    layout->addWidget(line_edit_);

    line_edit_->setPlaceholderText("dump.sql");

    button_box_ = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    layout->addWidget(button_box_);

    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {

                obj->setStyleSheet(QStringLiteral("QPushButton {background: floralwhite; color:darkslategray; font-weight:bold;} %2")
//                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
        }
}

void SqlDump_name::init_connections()
{
    connect(button_box_, &QDialogButtonBox::rejected,[=](){
        this->close();
    });

    connect(button_box_, &QDialogButtonBox::accepted,[=]{

        get_dump();
    });
}


void SqlDump_name::get_dump()
{

    QStringList args_list;
    args_list << QString("-u"+auth_.login_) << QString("-h"+auth_.host_) << QString("-p"+auth_.passw_) << args_/*"--databases"<< "aatest1" << "abitura"*/;
    //qDebug() << "args StringList->>" << args_list << "<--args StringList";

    // validation of format '.sql' in output file name
    QString output_filename = line_edit_->text().isEmpty() ? "dump.sql" : (line_edit_->text().right(4)==".sql") ? line_edit_->text() : line_edit_->text()+".sql" ;

    QString const outputDir = adb_utility::filepath_+'/'+"SQL_dumps";


    QDir dir(outputDir);
    if(!dir.exists())
        dir.mkpath(".");


    output_filename = outputDir+'/'+output_filename;

//    qDebug() << "OUTPUTFILENAME::" << output_filename;


    // check if this file name is already exist
    if(adb_utility::fileExists_(output_filename)){

        adbMessageBox overwriteExistMessageBox{QMessageBox::Warning,"File name already exist", "File with entered name already exist in dump's folder. "
                                                                                            " Do you want to replace existing dump or choose another name?",
                                                      /*QMessageBox::Cancel,*/QMessageBox::NoButton,this};

        QAbstractButton* overwriteConfirmButton = overwriteExistMessageBox.addButton("Overwrite existing dump file",QMessageBox::YesRole);

        overwriteConfirmButton->setStyleSheet(QStringLiteral("QPushButton {background: darkred;color:white;font-weight:bold;} %1")
                                                    .arg(adb_style::getbuttonKhakiHiglightSS()));

        QAbstractButton* cancelButton = overwriteExistMessageBox.addButton("Enter another name",QMessageBox::NoRole);

        cancelButton->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                                                    .arg(adb_style::getbuttonKhakiHiglightSS()));

        cancelButton->setFocus();

        overwriteExistMessageBox.setModal(true);
        overwriteExistMessageBox.show();
        overwriteExistMessageBox.exec();

        if(overwriteExistMessageBox.clickedButton()!=overwriteConfirmButton){

            return;
        }

    }


    //qDebug() << "right 4==" << line_edit_->text().right(4);

    QProcess dumpProcess(this);

    dumpProcess.setStandardOutputFile(output_filename);


      QObject::connect(&dumpProcess,static_cast<void (QProcess::*) (int, QProcess::ExitStatus)>(&QProcess::finished),[&](int exitCode, QProcess::ExitStatus exitStatus){

         // qDebug() << "signal QProcess::finished emited";

        // DUMPING IS SUCCESSFUL ONLY IF::
        if(exitStatus==QProcess::NormalExit && exitCode == 0){

            QString const status_message = "(✓) Dumping process finished successfully. Exit code: " + QString::number(exitCode) +". Output file: "+output_filename ;
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

//            connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
            messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
            messageBox->show();

            qCritical()<< "Error string: "<<dumpProcess.errorString();

            qWarning()<<"ReadAllStandardError::"<<stderr_text;

            emit message(full_err_message);

            qDebug()<<"readAllStandardOutput::"<<dumpProcess.readAllStandardOutput();
        }

    });


      QString __dumpProgramName = "";

      if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){

          __dumpProgramName = "mariadb-dump";

      } else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){

          __dumpProgramName = "mysqldump";

      }

      connect(&dumpProcess, &QProcess::errorOccurred, [=](QProcess::ProcessError error)
      {
          qWarning() << "(x)Dumping Error occured: " << error ;

          QString __packagesHelpMsgDeb = "deb-based distributions:",
                  __packagesHelpMsgRpm = "rpm-based distributions:",
                  __additionalHelpInfo = "";

          if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){

              __packagesHelpMsgDeb += " <b>mariadb-client</b> / <b>mariadb-server</b> ";

              __packagesHelpMsgRpm += " <b>mariadb</b> / <b>mariadb-server</b> ";

          } else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){

              __packagesHelpMsgDeb += " <b>default-mysql-client</b>/ <b>default-mysql-server</b> ";

              __packagesHelpMsgRpm += " <b>community-mysql</b> / <b>community-mysql-server</b> ";

              __additionalHelpInfo = QStringLiteral("Note: you can download standalone MySQL installation meta-package manually from website (not from your distro's repo), "
                                      "that contains neccessary [<b>%1</b>] program binary. "
                                       "Download link: [<b>https://dev.mysql.com/downloads/</b>].").arg(__dumpProgramName);

          } //else if(...){...;}

          QString __messageText = QStringLiteral("Dumping process cannot start. "
           "Check that neccessary package with dumping program binary [<b>%1</b>] is installed on your system, "
            "or dependend symlink is valid or presented in '/bin' folder. "
             "On various package-managing systems this can be provided by next 'meta' packages, 4ex.:<br>%2<br>%3<br>%4")
                  .arg(__dumpProgramName)
                  .arg(__packagesHelpMsgDeb)
                  .arg(__packagesHelpMsgRpm)
                  .arg(__additionalHelpInfo);

          std::cout << __messageText.toStdString() << std::endl;
          qWarning() << __messageText;

          QPointer <QMessageBox> messageBox{new QMessageBox(QMessageBox::Critical,"Process cannot start", __messageText,
                                                            QMessageBox::Ok,this)};

          /*connect(messageBox,&QMessageBox::destroyed,[&](){
              //qDebug() << "~messageBox activated (destroyed).";
              emit message("(x)Dumping Error occured. Check log for additional info.");
          });*/

          messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
          messageBox->show();
          //messageBox->exec();

      });

      dumpProcess.setReadChannel(QProcess::StandardOutput);

      connect(&dumpProcess, &QProcess::readyReadStandardError,[=](){});


      dumpProcess.start(__dumpProgramName, args_list);


      this->setCursor(Qt::WaitCursor);

      dumpProcess.waitForFinished();

      this->setCursor(Qt::ArrowCursor);

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


SqlDump_db_choose::SqlDump_db_choose(auth& auth__,QWidget *parent)
    : QDialog(parent)

    , auth_(auth__)


{
    init_form();
    init_connections();


}

SqlDump_db_choose::~SqlDump_db_choose()
{
   // qDebug()<<"~SqlDump_db_choose";
}

bool SqlDump_db_choose::show_db()
{
    if(db_connection::try_to_reopen(auth_)){

            if(db_connection::set_query("SHOW DATABASES",/*model*/&model_,comboBox_)){
                return true;
            }
    }
    return false;
}

void SqlDump_db_choose::init_form()
{
    setWindowTitle("Database choosing");

    QVBoxLayout* layout{new QVBoxLayout{}};

    setLayout(layout);

    label_ = new QLabel{"Choose database you want dump table/s from."};
    label_->setStyleSheet("min-height:16px;color:darkslategray; border :2px solid black; padding: 6px; border-style : dashed");
    label_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    label_->setAlignment(Qt::AlignCenter);

    layout->addWidget(label_);

    comboBox_ = new QComboBox;

    comboBox_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    QFrame* connectionSubFrame = new QFrame{this};
    QHBoxLayout* connection_layout{new QHBoxLayout{connectionSubFrame}};
    connectionSubFrame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //connectionSubFrame->setContentsMargins(0,0,0,0);
    //connection_layout->setSpacing(0);
    connection_layout->setContentsMargins(0,0,0,0);

    connection_layout->addWidget(comboBox_);


    reload_button_ = new reloadButton{nullptr,"darkslategray","snow",true};
    reload_button_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    connection_layout->addWidget(reload_button_);

//    layout->addLayout(connection_layout);
    layout->addWidget(connectionSubFrame);

    layout->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

    buttonBox_ = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok ,this);

    layout->addWidget(buttonBox_);

    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            // if button is inherited class object -> cast pointer to inherit type
            if(obj->objectName()=="reloadButtonObj"){
                auto reloadButtonObj = qobject_cast<reloadButton*>(obj);
                reloadButtonObj->setKhakiHighlight();

            }else {
                obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }

    QList<QComboBox*> comboBoxInFormlist = this->findChildren<QComboBox*>();
        foreach (auto obj, comboBoxInFormlist) {

                obj->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#fffffa","darkslategray"));
        }

}

void SqlDump_db_choose::init_connections()
{
    connect(buttonBox_,&QDialogButtonBox::rejected, this,&SqlDump_db_choose::reject);

    connect(reload_button_,&QPushButton::clicked,[=]{
        show_db();
    });

    connect(buttonBox_,&QDialogButtonBox::accepted,[=]{
        emit chosen_db_signal(adb_utility::escape_sql_backticks( this->comboBox_->currentText()) );
        accept();
    });
}
