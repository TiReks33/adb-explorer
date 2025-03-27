#include "databases.h"
#include "ui_databases.h"

#include "custom_query.h"

int Databases::defaultScaleIndex_ = 0;
QString Databases::defaultFont_ = "Noto Sans,10,-1,0,50,0,0,0,0,0,Regular";

Databases::Databases(auth& auth__, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Databases)
  , tableView_(new signalTableView)
  , showDB_button{new reloadButton{0,/*"black","red"*/"floralwhite","darkslategray",true,false,"<u>R</u>eload databases list"}}
  , statusBar(new scrolledStatusBar)

  , auth_(auth__)
  , tables_window_(new Tables(auth_))
  , new_db_window_(new create_db_name)
  , delete_db_window_(new delete_db)

  , rescaleBoxWidget(adb_utility::getRescaleBox(tableView_))
  , rescaleDefaultCheckBox{new QCheckBox{"set default"}}

  , createUserButton_{new QPushButton{"Create SQL Server User/Role"}}
  , getUsersListButton_{new QPushButton{"Users/Roles list"}}
  , grantPermissionsButton_{new QPushButton{"Grant privileges to SQL Server User/Role"}}
  , changePassButton_{new QPushButton{}}
  , deleteUserButton_{new QPushButton{"DELETE USER"}}
  , quitButton_{new QPushButton(/*"⌧"*//*"✕"*/QString::fromUtf8("\U00002715"))/*("⮿")*/}
  , fontWidget_{new fontEmbeddedWidget{tableView_}}

  , menuBar_{new QMenuBar{this}}
  , menuFile_{new /*QMenu*/hideMenu{Qt::Key_F10,menuBar_}}
  , serverInfoEntrie_{new QAction{"Server connection info"}}
  , getSqlCurUsEntrie_{new QAction{"Current SQL-Server user entry"}}
  , exitEntrie_{new QAction{"Exit",menuFile_}}
  //, fontPointSizeSpin_{new QSpinBox{}}
{

    ui->setupUi(this);

    init_form();

    init_signals();

    fileOps();

    defaultSettings();

}

Databases::~Databases()
{
    db_connection::close();   // close SQL-connection 'auth::con_name_'
    db_connection::remove(); // remove SQL-connection 'auth::con_name_'
    delete ui;
    delete tableView_;

    delete tables_window_;
    delete new_db_window_;
    delete delete_db_window_;

    delete rescaleBoxWidget;
    delete rescaleDefaultCheckBox;

    delete createUserButton_;
    delete grantPermissionsButton_;
}

void Databases::keyPressEvent(QKeyEvent *e) {
    auto key = e->key();
    if(key == Qt::Key_Escape){
        close();
    } else if(key == Qt::Key_Down || key == Qt::Key_Up) {
        emit tableView_->clicked(/*QModelIndex()*/tableView_->currentIndex());
    } /*else if(key == Qt::Key_Enter || key == Qt::Key_Return){

    }*/ else if(key == Qt::Key_F10){
        if(!menuBar_->isVisible()){
            menuBar_->show();
        } else{
            menuBar_->hide();
        }
    } else{
        QDialog::keyPressEvent(e);
    }
}


void Databases::closeEvent(QCloseEvent *event)
{

    if(!adb_utility::showExitAppDialog(this)){

        event->ignore();
        return;
    }

    emit close_all_custom_windows_();

    event->accept();
}


void Databases::init_form()
{
    ui->mainLayout->insertWidget(0,tableView_);


    QFrame* reloadFrame = new QFrame;
    reloadFrame->setFrameShape(QFrame::StyledPanel);
    reloadFrame->setFrameShadow(QFrame::Raised);
    QHBoxLayout* reloadLay = new QHBoxLayout{reloadFrame};
    reloadLay->setContentsMargins(1,1,1,1); reloadLay->setSpacing(0);
    reloadLay->addWidget(showDB_button);
    ui->reloadVerticalLayout->addWidget(reloadFrame);


    showDB_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    showDB_button->setFontPointSize(12);

    ui->showTables_button->setStyleSheet("background: darkslategray; color: snow; font-weight:bold;");

    // since rescaleBoxWidget added -- it control the tableView's rescale
    ui->rescaleLayout->addWidget(rescaleBoxWidget);
    ui->rescaleLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    move(screen()->geometry().center() - frameGeometry().center());

    ui->statusBarLayout->addWidget(statusBar);

    statusBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    setWindowIcon(QIcon(":/pic/adb-explorer_logo.png"));

    statusBar->get_line()->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                        | Qt::WindowMinimizeButtonHint
                                        | Qt::WindowMaximizeButtonHint
                                        | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    tableView_->setSelectionMode(QTableView::SingleSelection);

    QHBoxLayout* sqlUserLay = new QHBoxLayout;

    QFrame* sqlUserSubFrame0 = new QFrame;
    sqlUserSubFrame0->setFrameShape(QFrame::StyledPanel);
    sqlUserSubFrame0->setFrameShadow(QFrame::Raised);

    QVBoxLayout* sqlUserSubLay0 = new QVBoxLayout{sqlUserSubFrame0};
    sqlUserSubLay0->setContentsMargins(1,1,1,1);
    sqlUserSubLay0->setSpacing(0);

    QFrame* sqlUserSubFrame1 = new QFrame;
    sqlUserSubFrame1->setFrameShape(QFrame::StyledPanel);
    sqlUserSubFrame1->setFrameShadow(QFrame::Raised);

    QVBoxLayout* sqlUserSubLay1 = new QVBoxLayout{sqlUserSubFrame1};
    sqlUserSubLay1->setContentsMargins(1,1,1,1);
    sqlUserSubLay1->setSpacing(0);

    sqlUserLay->addWidget(sqlUserSubFrame0);//addLayout(sqlUserSubLay0);

    QLabel* sqlUserSeparateLbl = new QLabel{"::"};
    sqlUserSeparateLbl->setStyleSheet("font-weight:bold;");
    sqlUserSeparateLbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    sqlUserLay->addWidget(sqlUserSeparateLbl);

    sqlUserLay->addWidget(sqlUserSubFrame1);//addLayout(sqlUserSubLay1);

    sqlUserSubLay0->addWidget(createUserButton_);

    QFrame* sqlUserSubLay0HLine = new QFrame;
    sqlUserSubLay0HLine->setFrameShape(QFrame::HLine);
    sqlUserSubLay0HLine->setFrameShadow(QFrame::Raised);
    sqlUserSubLay0->addWidget(sqlUserSubLay0HLine);

    createUserButton_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    sqlUserLay->setContentsMargins(0,0,0,0);
    ////sqlUserLay->setSpacing(0);


    getUsersListButton_->setStyleSheet("background-color:gray; color:white; font-weight:bold; ");//("background:gainsboro; color:darkslategray;font-weight:bold");

    sqlUserSubLay0->addWidget( getUsersListButton_);


    sqlUserSubLay1->addWidget(grantPermissionsButton_);

    QFrame* sqlUserSubLay1HLine = new QFrame;
    sqlUserSubLay1HLine->setFrameShape(QFrame::HLine);
    sqlUserSubLay1HLine->setFrameShadow(QFrame::Raised);
    sqlUserSubLay1->addWidget(sqlUserSubLay1HLine);

    grantPermissionsButton_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    grantPermissionsButton_->setStyleSheet("background-color:gray; color:white; font-weight:bold; ");

    ui->ButtonsSubLayout->insertLayout(2,sqlUserLay);

    createUserButton_->setStyleSheet("background-color:gray; color:white; font-weight:bold; ");


    ui->rescaleLayout->addWidget(rescaleDefaultCheckBox);

    rescaleDefaultCheckBox->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

//    ui->fontLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    ui->fontLayout->addWidget(fontWidget_);
//    ui->fontLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    fontWidget_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);


    deleteUserButton_->setStyleSheet("background-color:darkred;color:white;");


    QFrame* deleteUserFrame = new QFrame;
    deleteUserFrame->setFrameShape(QFrame::StyledPanel);
    deleteUserFrame->setFrameShadow(QFrame::Raised);
    QHBoxLayout* backLay = new QHBoxLayout{deleteUserFrame};
    backLay->setContentsMargins(1,1,1,1); backLay->setSpacing(0);
    backLay->addWidget(deleteUserButton_);
    ui->deleteLayout->addWidget(deleteUserFrame);


    quitButton_->setStyleSheet("background-color:darkred;color:white;font-size:14pt;padding-left:6px;padding-right:6px;");//padding-top:1px;padding-bottom:1px;");

    QFrame* exitFrame = new QFrame;
    exitFrame->setFrameShape(QFrame::StyledPanel);
    exitFrame->setFrameShadow(QFrame::Raised);
    exitFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QHBoxLayout* exitLay = new QHBoxLayout{exitFrame};
    exitLay->setContentsMargins(1,1,1,1); exitLay->setSpacing(0);
    exitLay->addWidget(quitButton_);
    ui->quitLayout->addWidget(exitFrame);


    changePassButton_->setStyleSheet("background-color:grey; color:white;font-weight:bold; ");
    changePassButton_->setText("Manage passwords");
    sqlUserSubLay1->addWidget(changePassButton_);


    auto fontPushButton = fontWidget_->findChild<QPushButton*>("fontButton");
        fontPushButton->setStyleSheet(QStringLiteral("QPushButton{color: darkslategray; background: #fffffa;} %1").arg(adb_style::getbuttonKhakiHiglightSS()));


    auto rescaleComboBox = rescaleBoxWidget->findChild<notifyComboBox*>();
    if(rescaleComboBox)
        rescaleComboBox->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#fffffa","darkslategray"));//("background:#fffffa;");


    ui->mainLayout->insertWidget(0,menuBar_);

    menuFile_->setTitle("File");

    menuFile_->addAction(serverInfoEntrie_);
    menuFile_->addAction(getSqlCurUsEntrie_);
    menuFile_->addAction(exitEntrie_);


    exitEntrie_->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    serverInfoEntrie_->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));
    getSqlCurUsEntrie_->setIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation));

    menuBar_->addMenu(menuFile_);

    menuBar_->hide();


    ui->create_db_button->setStyleSheet("background:floralwhite;color:darkslategray; font-weight:bold;padding: 6px; ");

    ui->queryButton->setStyleSheet("background: darkslategray; color: white;padding: 6px; ");

    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {

            // if button is inherited class object -> cast pointer to inherit type
            if(obj->objectName()=="reloadButtonObj"){

                auto reloadButtonObj = qobject_cast<reloadButton*>(obj);
                reloadButtonObj->setKhakiHighlight();

            }else if(obj->objectName()!="fontButton"){

                obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }


    ui->showTables_button->setFocus();

    tableView_->setAlternatingRowColors(true);

    tableView_->setPalette(QPalette(adb_style::whiteUndGrayColor));



    QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }

}



void Databases::init_signals()
{

    // reload database list
    connect(showDB_button,&QPushButton::clicked,[=]{
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

        if(!db_connection::set_query(query__,&model_,tableView_/*,QHeaderView::Stretch*/)){

            QString const warning_text = QStringLiteral("Database `%1` is not created. Please check selected name and permissions and try again.").arg(newdb_name__);

            qDebug() << warning_text;

            std::cout << warning_text.toStdString() << std::endl;

            statusBar->get_line()->setText(warning_text);

        } else{

            auto SucCreate = QStringLiteral("Database `%1` successfully created!").arg(newdb_name__);
            qDebug() << SucCreate;
            std::cout << SucCreate.toStdString() << std::endl;

            statusBar->get_line()->setText(SucCreate);
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
    connect(delete_db_window_,&delete_db::delete_form_send,this,[this](QComboBox* comboBox__){

        QString const __queryText = "SHOW DATABASES";

        delete_db_window_->setComboBoxLoadQueryText(__queryText);

        connect(delete_db_window_,&delete_db::reloadSig,[this,comboBox__]{

            db_connection::open(auth_);

            db_connection::set_query(delete_db_window_->comboBoxLoadQueryText(),&model_,comboBox__);

            comboBox__->setCurrentIndex(-1); //for blank cell default
        });

        delete_db_window_->reload();
    });

    // signal-slot chain for delete chosen database(2)
    connect(delete_db_window_,&delete_db::delete_entity,[=](QComboBox*comboBox__){
            db_connection::open(auth_);


            QString const chosen_db = comboBox__->currentText();

            QString const query_text = QString("DROP DATABASE `%1`").arg(QString(adb_utility::escape_sql_backticks(chosen_db)));


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
            QPointer <adbMessageBox> quitMessageBox = new adbMessageBox(QMessageBox::Warning,"Confirm choosing","Are you want to choose database \""+auth_.db_name_+"\" and go to the next form?"
                                                                "<br>Note: <FONT COLOR='#ff0000'>All query windows related to this form (if any) will be closed.</FONT>"
                                                                    " This behaviour changeable via settings.",
                                                          QMessageBox::Yes | QMessageBox::No,this);

            quitMessageBox->setAttribute( Qt::WA_DeleteOnClose, true);
            //connect(quitMessageBox,&adbMessageBox::destroyed,[]{qDebug() << "~adbMessageBox";});

            QList<QPushButton*> ButtonsInFormlist = quitMessageBox->findChildren<QPushButton*>();
                foreach (auto obj, ButtonsInFormlist) {
                    obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                        .arg(adb_style::getbuttonKhakiHiglightSS()));
                }

            quitMessageBox->setModal(true);
            quitMessageBox->show();
            int dialogAnswer = quitMessageBox->exec();

            if(dialogAnswer!=QMessageBox::Yes){

              return;
            }
        }


        tables_window_->show();
        this->hide();

        if(close_chld_wndws_on_next){
            // close all queries form/results from 'database' window
            emit close_all_custom_windows_();
        }
            emit show_tables_signal();
    });


    // mouse double-click handler
    connect(tableView_,&QTableView::activated,[=](const QModelIndex &index){



        tableView_->setCurrentIndex(index);

        auth_.db_name_=tableView_->model()->data(tableView_->currentIndex()).toString();

        statusBar->get_line()->clear();

    });

    // mouse left-click handler
    connect(tableView_,&QTableView::clicked,[=](const QModelIndex &index){

        tableView_->setCurrentIndex(index);

        auth_.db_name_=tableView_->model()->data(tableView_->currentIndex()).toString();

        statusBar->get_line()->clear();


    });

    connect(tableView_,&QTableView::doubleClicked,[=](const QModelIndex &index){

        tableView_->setCurrentIndex(index);

        auth_.db_name_=tableView_->model()->data(tableView_->currentIndex()).toString();

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


    connect(createUserButton_,&QPushButton::clicked, this, &Databases::createUser);



    //QObject::connect(rescaleDefaultCheckBox,&QCheckBox::destroyed,[=]{ qDebug() << "~Databases::ui->rescaleLayout::rescaleDefaultCheckBox"; });


    QPointer</*QComboBox*/notifyComboBox> rescaleComboBox = rescaleBoxWidget->findChild<notifyComboBox*>();


    connect(rescaleComboBox,&notifyComboBox::sameIndexRepeated,[=](){
        rescaleComboBox->currentIndexChanged(rescaleComboBox->currentIndex());
    });

    connect(rescaleComboBox,static_cast<void(/*QComboBox*/notifyComboBox::*)(int)>(&/*QComboBox*/notifyComboBox::currentIndexChanged),[=](int newIndex__){
        if(newIndex__==Databases::defaultScaleIndex_){
            rescaleDefaultCheckBox->setChecked(true);
            rescaleDefaultCheckBox->setEnabled(false);
        } else{
            rescaleDefaultCheckBox->setChecked(false);
            rescaleDefaultCheckBox->setEnabled(true);
        }
    });

    connect(rescaleDefaultCheckBox,&QCheckBox::stateChanged,[=](int state__){
        if(state__){
            int currentBoxIndex = rescaleComboBox->currentIndex();
            if(currentBoxIndex!=Databases::defaultScaleIndex_){
                ////tableScaleChanged=true;
                Databases::defaultScaleIndex_ = currentBoxIndex;
                rescaleDefaultCheckBox->setEnabled(false);

                write2_settings_file();
            }
        }
    });

    connect(grantPermissionsButton_,&QPushButton::clicked,this,&Databases::grantUserPermissionsButtonHandler);

    connect(quitButton_,&QPushButton::clicked,this,&Databases::close);

    connect(deleteUserButton_,&QPushButton::clicked,this,&Databases::deleteUser);

    connect(getUsersListButton_,&QPushButton::clicked,this,&Databases::getUsersList);

    connect(changePassButton_,&QPushButton::clicked, this, &Databases::getPasswordMgmtForm);

    connect(fontWidget_,&fontEmbeddedWidget::defaultFontChanged,[this](QString const& newDefaultFont_){
        Databases::defaultFont_=newDefaultFont_;
        write2_settings_file();
    });

    connect(menuFile_,&hideMenu::menuHiden,menuBar_,&QMenuBar::hide);

    connect(exitEntrie_, &QAction::triggered,[this]{
        close();
    });

    connect(serverInfoEntrie_,&QAction::triggered,[this]{

        adb_utility::get_separate_information_window(QMessageBox::Information,"Server connection information",auth_.getConnectionInfo(), this,false);
    });

    connect(getSqlCurUsEntrie_,&QAction::triggered,[this]{

        adb_utility::get_separate_information_window(QMessageBox::Information,"Current SQL-Server login entry",
                                                     QStringLiteral("Current connected SQL-Server user: %1.")
                                                     .arg(db_connection::getCurrentSQLuserEntry(auth_,auth::con_name_)), this,false);
    });

    connect(this,&Databases::emptySet,[=]{
        if(MSG_SHOW_IF_BLANK_RESULT)
        {
            adb_utility::get_information_window(QMessageBox::Information,"Empty set","(✓) [Note] :: Query result not contain/doesn't imply"
                                   " contain of displayable result.", this);
        }
    });

}


void Databases::fileOps()
{
    // get actual configs from files:

    //->"databases.cfg"
    if(!read4rom_settings_file())
        write2_settings_file();

    //->"adb_utility.cfg"
    if(!adb_utility::config::read4rom_settings_file())
        adb_utility::config::write2_settings_file();
}


void Databases::defaultSettings()
{
    // set default font
    QFont __font;

    __font.fromString(Databases::defaultFont_);
    tableView_->setFont(__font);

    // set default settings
    rescaleBoxWidget->findChild<notifyComboBox*>()->setCurrentIndex(Databases::defaultScaleIndex_);
}


void Databases::message_to_status(const QString & message__) const
{
    statusBar->get_line()->clear();
    statusBar->get_line()->insert(message__);
}


/*void*/bool Databases::show_databases()
{

    if(db_connection::open(auth_)){

        if(db_connection::set_query("SHOW DATABASES;",&model_,tableView_/*,QHeaderView::Stretch*/)){

            // 1st cell in model default selected
            select_cells(0,0, tableView_);

            auth_.db_name_=tableView_->model()->data(tableView_->currentIndex()).toString();

            ////qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

            ////qDebug() << "current auth_.dB_name_ index::" << auth_.db_name_;

            if(!auth_.db_name_.isEmpty()){
                ui->showTables_button->setEnabled(true);
                //ui->showTables_button->setStyleSheet(QStringLiteral("QPushButton {background: darkslategray; color: snow; font-weight:bold;} %1").arg(adb_style::getbuttonKhakiHiglightSS()));
            }

            return true;
        }
    }

    return false;
}



void Databases::deleteUser()
{

    delete_sqldb_user* deleteUserForm = new delete_sqldb_user{this};

    QComboBox* deleteFormComboBox = deleteUserForm->findChild<QComboBox*>();

    QPointer<QSqlQueryModel> __model = new QSqlQueryModel;

    connect(deleteUserForm,&delete_sqldb_user::destroyed,[=]{ delete __model;});


    connect(deleteUserForm,&delete_sqldb_user::reloadSig,[this,deleteFormComboBox,deleteUserForm,__model]{

        db_connection::open(auth_);

        QPointer<adbMessageBox> __warningWindowPtr = nullptr;

        db_connection::set_query(deleteUserForm->comboBoxLoadQueryText(),__model,deleteFormComboBox,__warningWindowPtr,true,Qt::ApplicationModal,auth::con_name_);

        if(__warningWindowPtr){

            auto sig = connect(this,&Databases::close_all_custom_windows_,[__warningWindowPtr]{
                if(__warningWindowPtr){
                    __warningWindowPtr->close();
                }

            });

            connect(__warningWindowPtr,&adbMessageBox::destroyed,[sig]{
                disconnect(sig);
            });

        }

        deleteFormComboBox->setCurrentIndex(-1); //for blank cell default
    });


    QRadioButton* deleteFormUserRButton = deleteUserForm->findChild<QRadioButton*>("deleteUser");

    connect(deleteFormUserRButton,&QRadioButton::toggled,[this,deleteUserForm](bool state__){
        if(state__){

            QString __qryTxt = "";

            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
                __qryTxt = "select concat(quote(user),'@',quote(host)) from mysql.user where is_role=\'n\';";
            } else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){//db_connection::set_query("select concat(quote(user),'@',quote(host)) from mysql.user where password_expired=\'n\' or account_locked=\'n\';",__model,deleteFormComboBox,auth::con_name_);
                __qryTxt = "select concat(quote(user),'@',quote(host)) from mysql.user where password_expired=\'n\' or account_locked=\'n\';";
            }

            deleteUserForm->setComboBoxLoadQueryText(__qryTxt);

            deleteUserForm->reload();
        }
    });

    QRadioButton* deleteFormRoleRButton = deleteUserForm->findChild<QRadioButton*>("deleteRole");

    connect(deleteFormRoleRButton,&QRadioButton::toggled,[this,deleteUserForm](bool state__){
        if(state__){
            QString __qryTxt = "";

            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
                __qryTxt = "select quote(user) from mysql.user where is_role=\'y\';";
            } else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){
                __qryTxt = "select concat(quote(user),'@',quote(host)) from mysql.user where password_expired=\'y\' and account_locked=\'y\';";
            }

            deleteUserForm->setComboBoxLoadQueryText(__qryTxt);

            deleteUserForm->reload();
        }
    });

    connect(deleteUserForm,&delete_sqldb_user::delete_entity,[this,deleteUserForm,deleteFormComboBox,deleteFormUserRButton](/*QComboBox* usersComboBox__*/){

        QString const curUser2drop = deleteFormComboBox->currentText();
         auto qry = QSqlQuery(QSqlDatabase::database(auth_.con_name_,false));

         QString queryText="";
         if(deleteFormUserRButton->isChecked()){
             queryText = QString("DROP USER %1").arg(QString(curUser2drop));
         } else{
             queryText = QString("DROP ROLE %1").arg(QString(curUser2drop));
         }


         if(qry.exec(queryText)){

             statusBar->get_line()->setText(QString("User/role record %1 has been successfully deleted from server.").arg(curUser2drop));
             deleteUserForm->close();

             return;
         }

         QString const error_msg = qry.lastError().text();
         adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
         qDebug() << error_msg;

    });



    deleteUserForm->setAttribute(Qt::WA_DeleteOnClose,true);
    deleteUserForm->setModal(true);
    deleteUserForm->show();
    deleteFormUserRButton->setChecked(true);
}



void Databases::getUsersList()
{
    static QPointer<getCredentialRecordsForm> recordsForm;

    if(!recordsForm){

        recordsForm = new getCredentialRecordsForm{auth_,this};

        recordsForm->setAttribute(Qt::WA_DeleteOnClose,true);
        recordsForm->setModal(false);
        recordsForm->show();

        connect(this,&Databases::close_all_custom_windows_, recordsForm , &getCredentialRecordsForm::closeNowSig, Qt::QueuedConnection);

    } else{
        recordsForm->raise();
    }

}



void Databases::getPasswordMgmtForm()
{
    QPointer<passwordMgmtForm> passwordForm{new passwordMgmtForm{auth_,this}};

    connect(passwordForm,&passwordMgmtForm::message,[this](QString const&message__){
        this->statusBar->get_line()->setText(message__);
    });

    passwordForm->setAttribute(Qt::WA_DeleteOnClose,true);
    passwordForm->setModal(true);
    passwordForm->show();

    //std::cout << auth_.login_.toStdString() << "::" << auth_.host_.toStdString() << std::endl;
}



void Databases::createUser()
{
    QPointer<createUserForm> createUserFormWndw = new createUserForm(auth_,this);

    connect(createUserFormWndw,&createUserForm::message,[this](QString const& message__){
        statusBar->get_line()->setText(message__);
    });

    createUserFormWndw->setAttribute(Qt::WA_DeleteOnClose,true);
    createUserFormWndw->setModal(true);
    createUserFormWndw->show();
}



void Databases::grantUserPermissionsButtonHandler()
{

    QPointer<grantUserPermissionsForm> grantUserPermissionsWndw = new grantUserPermissionsForm(auth_,this);

    connect(grantUserPermissionsWndw,&grantUserPermissionsForm::message,[this](QString const& message__){
        statusBar->get_line()->setText(message__);
    });


    grantUserPermissionsWndw->setAttribute(Qt::WA_DeleteOnClose,true);
//    grantUserPermissionsWndw->setWindowModality(Qt::WindowModal);

    grantUserPermissionsWndw->setModal(true);

    grantUserPermissionsWndw->show();

    grantUserPermissionsWndw->load();

}



void Databases::on_mysqldump_button_clicked()
{

    QScopedPointer<SqlDump_credentials> dump{new SqlDump_credentials{auth_,this}};


    // message with resultation info of successful/unsuccessful dump
    connect(dump.get(),&SqlDump_credentials::message,[&](QString const& message__){
        statusBar->get_line()->setText(message__);
        ////qDebug()<<message__;
        dump->close();
    });


    dump->setModal(true);
    dump->show();
    dump->exec();

    return;
}

void Databases::get_query_wndw()
{

    QPointer<Custom_Query> custom_query_window{new Custom_Query{nullptr,true}};

    if(query2server_note){
        noteFrame* query2server_note_ptr = custom_query_window->add_note("*Note: if you need to operate with certain database or tables in it, "
                         "you must choose database from 'Database' window and click 'Show tables in selected database'.");

        ////connect(custom_query_window,&Custom_Query::dont_show_note,[=]{ query2server_note=false; write2_settings_file();  });
        connect(query2server_note_ptr,&noteFrame::dontShowNoteAgainSig,[this]{
            query2server_note = false;
            write2_settings_file();
        });
    }


    // call overload signal on overload slot
    connect(custom_query_window,static_cast<void(Custom_Query::*)(/*QString,*/Custom_Query *)>(&Custom_Query::send_custom_query),
            this,[custom_query_window,this](Custom_Query* customQueryWndw__){
        custom_query_window->setCheckCloseMessageFlag(false);
        send_custom_query_slot(customQueryWndw__);
        custom_query_window->setCheckCloseMessageFlag(true);
    ;});


//    if(close_chld_wndws_on_next){

        // closing all forms by parent signal
        connect(this,&Databases::close_all_custom_windows_, custom_query_window, &Custom_Query::closeNowSig, Qt::QueuedConnection);
//    }


    custom_query_window->setAttribute( Qt::WA_DeleteOnClose, true );
    custom_query_window->setModal(false);
    custom_query_window->show();
//    custom_query_window->exec();

}


void Databases::send_custom_query_slot(Custom_Query *custom_query_window__)
{
// dummy counter to multiply windows (for removing SQL-Server subconnection when all gone)
static int wndw_cnt=0;

    auto QueryRawText = custom_query_window__->get_text();

    auto QueriesList = adb_utility::unpack_(QueryRawText,";",true);

    // not close form if some errors in queries occures
    auto queryFailFlag = false;

    auto skipAfterFail = false;

    for(auto i=0;i!=QueriesList.size();++i){

        if(skipAfterFail){
            custom_query_window__->save_query(QUERYSTATUS::NOTSENDED,QueriesList.at(i));
            continue;
        }

//        std::cout << QueriesList.at(i).toStdString() << " ";// << std::endl;
        //qDebug() << QueriesList.at(i);
//        std::cout << QueriesList.at(i).toStdString().size() << std::endl;


        // dynamic allocated copy of parent auth for preventing dangling (when local var goes out of scope)
        auth* auth_copy{new (std::nothrow) auth};

        if(auth_copy){


            auth_copy->copy_(auth_);


            // remove db_name_ entry (for design reason)
            auth_copy->db_name_.clear();


            QPointer<CustomQueryResult> new_result_window{new (std::nothrow) CustomQueryResult{{*auth_copy}/*{*__newOwner}*/,nullptr,true}};

            if(!new_result_window){

                delete auth_copy;
                auth_copy=nullptr;

            } else{

                wndw_cnt++;

                auto newTitle = this->metaObject()->className() + QStringLiteral("::") + new_result_window->windowTitle()+QStringLiteral(" № %1").arg(i);

                new_result_window->setWindowTitle(newTitle);

                new_result_window->setAttribute(Qt::WA_DeleteOnClose, true);

                connect(new_result_window,&CustomQueryResult::destroyed,[=]{

                    delete auth_copy;
                    //auth_copy = nullptr;

                    // if all windows was closed -- close and remove sub-connection to SQL server
                    if(!(--wndw_cnt)){
                        db_connection::close(subconnection_name_);
                        db_connection::remove(subconnection_name_);
                    }
    //                qDebug() << "wndw_cnt" << wndw_cnt;
    //                qDebug()<< "~new_result_window (destructor)";
                });



                //closing all forms by parent signal(2)
                connect(this,&Databases::close_all_custom_windows_, new_result_window , &CustomQueryResult::closeNowSig, Qt::QueuedConnection);



                // send query and place result to form of new allocated object
                new_result_window->custom_query_slot(QueriesList.at(i),subconnection_name_);

                // check error of query
                if((new_result_window->tableView->model())!=nullptr) {


                    // add query to encrypt bin. file (with success status)
                    custom_query_window__->save_query(QUERYSTATUS::SUCCESS,QueriesList.at(i));


                    // check if result query is empty
                    if(!new_result_window->tableView->model()->rowCount()&&!BLANK_RESULT){

                        //qDebug() << "(✓) rowCount() in model_==0::display result ignored.";

                        statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                                " contain of displayable result.");

                        emit emptySet();

                        new_result_window->deleteLater();

                    } else{

                        statusBar->get_line()->clear();

                        new_result_window->show();
                        //new_result_window->exec();
                    }

                } else{

                    custom_query_window__->save_query(QUERYSTATUS::FAILED,QueriesList.at(i));

                    // if query exec failed, show edit form in front again (to correct query by user)
                    //emit window_rise_signal();
                    queryFailFlag = true;

                    if(MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL){
                        //break;
                        skipAfterFail = true;
                        continue;
                    }
                }

            }

        }

    }

    if(QueriesList.size()){

        if(!queryFailFlag){
            custom_query_window__->close();

        }else{

            if(queryFailNote){

                auto previousFailNote = custom_query_window__->findChild<noteFrame*>("failNote");

                // delete previous (duplicate) fail note to prevent window's notes-spamming
                if(previousFailNote)
                    previousFailNote->close();

                QPointer<noteFrame> failNotePtr = custom_query_window__->add_note("*Note: Error/s occurred while proceed your SQL transaction/s.");

                failNotePtr->setObjectName("failNote");

                /*auto sig = */connect(failNotePtr,&noteFrame::dontShowNoteAgainSig,[this]{
                    queryFailNote = false;
                    write2_settings_file();
                });

            }

        }

    }
}



void Databases::showSettings()
{
    if(QPointer<QDialog> settings_dial{new(std::nothrow) QDialog{this}}){

        settings_dial->setAttribute(Qt::WA_DeleteOnClose,true);

        settings_dial->setWindowTitle("Databases::Settings");

        QVBoxLayout* main_lay = new QVBoxLayout;

        settings_dial->setLayout(main_lay);

        QLabel* settingsNameSubLabel = new QLabel{"SQL queries:"};

        auto lblFont = settingsNameSubLabel->font();

        lblFont.setBold(true);

        settingsNameSubLabel->setFont(lblFont);

        main_lay->addWidget(settingsNameSubLabel);

        QHBoxLayout* closeNonModal_lay = new QHBoxLayout;

        main_lay->addLayout(closeNonModal_lay);

        QCheckBox* closeNonModalCB = new QCheckBox{};
        closeNonModalCB->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

        ClickableLabel* CloseNonModalCB_lbl = new ClickableLabel("Close all query forms after database chosing");
        CloseNonModalCB_lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        CloseNonModalCB_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        closeNonModal_lay->addWidget(closeNonModalCB);
        closeNonModal_lay->addWidget(CloseNonModalCB_lbl);

        closeNonModalCB->setChecked(close_chld_wndws_on_next);

        connect(CloseNonModalCB_lbl,&ClickableLabel::clicked,[closeNonModalCB]{
            if(closeNonModalCB && closeNonModalCB->isEnabled())
                closeNonModalCB->setChecked(!closeNonModalCB->isChecked());
        });


        QHBoxLayout* errorTimeoutLay = new QHBoxLayout;

        ClickableLabel* errorTimeoutLbl = new ClickableLabel("Timeout for SQL queries errors pop-up messages");
        errorTimeoutLbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        errorTimeoutLbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        errorTimeoutLay->addWidget(errorTimeoutLbl);

        QLineEdit* errorTimeoutLine = new QLineEdit;
        errorTimeoutLine->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

        // only digits validation
        QIntValidator* int_validator = new QIntValidator(1000, 2073600000);

        errorTimeoutLine->setValidator( int_validator );

        connect(errorTimeoutLine, &QLineEdit::destroyed,[=]{ delete int_validator; /*qDebug() << "~int_validator";*/ });

        errorTimeoutLay->addWidget(errorTimeoutLine);

        errorTimeoutLine->setText(QString::number(adb_utility::_CUSTOM_MESSAGE_BOX_TIMER_));

        main_lay->addLayout(errorTimeoutLay);


        QHBoxLayout* showBlankResultLay = new QHBoxLayout;

        main_lay->addLayout(showBlankResultLay);

        QCheckBox* showBlankResultCB = new QCheckBox;
        showBlankResultCB->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

        ClickableLabel* showBlankResultCB_lbl = new ClickableLabel("Display empty query result");
        showBlankResultCB_lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        showBlankResultCB_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        showBlankResultLay->addWidget(showBlankResultCB);
        showBlankResultLay->addWidget(showBlankResultCB_lbl);


        QCheckBox* messageIfBlankResultCB = new QCheckBox;
        ClickableLabel* messageIfBlankResultCB_lbl = new ClickableLabel("Show message if result of SQL query\ndoesn't imply contain of displayable result");


        connect(messageIfBlankResultCB_lbl,&ClickableLabel::clicked,[messageIfBlankResultCB]{
            if(messageIfBlankResultCB && messageIfBlankResultCB->isEnabled())
                messageIfBlankResultCB->setChecked(!messageIfBlankResultCB->isChecked());
        });

        connect(showBlankResultCB,&QCheckBox::stateChanged,[messageIfBlankResultCB,messageIfBlankResultCB_lbl](int state__){
            if(state__){
                messageIfBlankResultCB->setChecked(false);
                messageIfBlankResultCB->setEnabled(false);
                messageIfBlankResultCB_lbl->setEnabled(false);
            } else{
                messageIfBlankResultCB->setEnabled(true);
                messageIfBlankResultCB_lbl->setEnabled(true);
            }
        });

        connect(showBlankResultCB_lbl,&ClickableLabel::clicked,[showBlankResultCB]{
            if(showBlankResultCB && showBlankResultCB->isEnabled())
                showBlankResultCB->setChecked(!showBlankResultCB->isChecked());
        });


        messageIfBlankResultCB->setChecked(MSG_SHOW_IF_BLANK_RESULT);

        showBlankResultCB->setChecked(BLANK_RESULT);


        QHBoxLayout* messageIfBlankResultLay = new QHBoxLayout;

        main_lay->addLayout(messageIfBlankResultLay);


        messageIfBlankResultCB->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

        messageIfBlankResultCB_lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        messageIfBlankResultCB_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        messageIfBlankResultLay->addWidget(messageIfBlankResultCB);
        messageIfBlankResultLay->addWidget(messageIfBlankResultCB_lbl);



        QHBoxLayout* terminate_lay = new QHBoxLayout;

        main_lay->addLayout(terminate_lay);

        QCheckBox* terminateCB = new QCheckBox{};
        terminateCB->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

        ClickableLabel* terminateCB_lbl = new ClickableLabel("Terminate execution of user custom SQL queries list after first failed query");
        terminateCB_lbl->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        terminateCB_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

        terminate_lay->addWidget(terminateCB);
        terminate_lay->addWidget(terminateCB_lbl);

        terminateCB->setChecked(MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL);

        connect(terminateCB_lbl,&ClickableLabel::clicked,[terminateCB]{
            if(terminateCB && terminateCB->isEnabled())
                terminateCB->setChecked(!terminateCB->isChecked());
        });



        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

        main_lay->addWidget(buttonBox);


        QList<QCheckBox*> checkBoxInFormlist = settings_dial->findChildren<QCheckBox*>();
            foreach (auto obj, checkBoxInFormlist) {

                    obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
            }

        QList<QPushButton*> ButtonsInFormlist = settings_dial->findChildren<QPushButton*>();
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


        connect(buttonBox, &QDialogButtonBox::rejected,settings_dial,&QDialog::reject);

        connect(buttonBox, &QDialogButtonBox::accepted,[=]{
            close_chld_wndws_on_next = closeNonModalCB->isChecked();
            bool ok;
            int temp = errorTimeoutLine->text().toInt(&ok);
            if(ok){
                /*new*/ adb_utility::_CUSTOM_MESSAGE_BOX_TIMER_ = temp;
                    adb_utility::config::write2_settings_file();
            }

            BLANK_RESULT = showBlankResultCB->isChecked();

            MSG_SHOW_IF_BLANK_RESULT = messageIfBlankResultCB->isChecked();

            MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL = terminateCB->isChecked();

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

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map)){
        int temp;

        if(adb_utility::intMapSettingExist(temp,__settings_map,"close_chld_wndws_on_next")) // "close_chld_wndws_on_next"
            close_chld_wndws_on_next = temp;

        if(adb_utility::intMapSettingExist(temp,__settings_map,"query2server_note"))//if((temp = __settings_map.value("query2server_note"))!=-1)
            query2server_note = temp;

        if(adb_utility::intMapSettingExist(temp,__settings_map,"defaultScaleMode")){
            Databases::defaultScaleIndex_=temp;
            rescaleBoxWidget->findChild<notifyComboBox*>()->setCurrentIndex(temp);
        }

        if(adb_utility::intMapSettingExist(temp,__settings_map,"BLANK_RESULT"))
            BLANK_RESULT = temp;

        if(adb_utility::intMapSettingExist(temp,__settings_map,"MSG_SHOW_IF_BLANK_RESULT"))
            MSG_SHOW_IF_BLANK_RESULT = temp;

        if(adb_utility::intMapSettingExist(temp,__settings_map,"MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL"))
            MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL = temp;

        if(adb_utility::intMapSettingExist(temp,__settings_map,"queryFailNote"))
            queryFailNote = temp;

    } else{

        qWarning() << "Error while read from"<<settings_f_name_;
        return false;
    }
    //--------------------------------------------------------------------
    QMap<QString,QString> __settings_map_str;

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map_str)){
        QString temp_s;

        if(adb_utility::QStringMapSettingExist(temp_s,__settings_map_str,"defaultTableFont")) //defaultTableFont
            Databases::defaultFont_ = temp_s;

        return true;
    } else{

        qWarning() << "Error while read from"<<settings_f_name_<<"(Strings)";
    }
    return false;
}

void Databases::write2_settings_file()
{
    QFile outFile(settings_f_name_);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << "close_chld_wndws_on_next" << '=' << QString::number(close_chld_wndws_on_next) << Qt::endl;
    textStream << "query2server_note" << '=' << QString::number(query2server_note) << Qt::endl;
    textStream << "defaultScaleMode" << '=' << QString::number(Databases::defaultScaleIndex_) << Qt::endl;
    textStream << "defaultTableFont" << '=' << '\"'+Databases::defaultFont_+'\"' << Qt::endl;
    textStream << "BLANK_RESULT" << '=' << QString::number(BLANK_RESULT) << Qt::endl;
    textStream << "MSG_SHOW_IF_BLANK_RESULT" << '=' << QString::number(MSG_SHOW_IF_BLANK_RESULT) << Qt::endl;
    textStream << "MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL" << '=' << QString::number(MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL) << Qt::endl;
    textStream << "queryFailNote" << '=' << QString::number(queryFailNote) << Qt::endl;
}

void Databases::mousePressEvent(QMouseEvent *event)
{
    auto key = event->button();

    if(key == Qt::LeftButton){

        statusBar->get_line()->clear();

    }

    QDialog::mousePressEvent(event);
}


