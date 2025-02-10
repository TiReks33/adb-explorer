#include "sqlusermanagement.h"



createUserForm::createUserForm(auth& auth__,QWidget *parent__)
    :QDialog(parent__)
    , auth_{auth__}

    , userRButton_{new QRadioButton{}}

    , roleRButton_{new QRadioButton{}}

    , nameLine_{new QLineEdit{}}

    , hostLbl_{new QLabel{}}
    , hostLine_{new QLineEdit{}}

    , setPasswordCheckBox_{new QCheckBox{}}
    , passwordFrame_{new QFrame{}}
    , passwordLine_{new QLineEdit{}}
    , hidePassCheckBox_{new QCheckBox{}}

    , buttonBox_{new QDialogButtonBox{}}
{
    initForm();
    initSignals();
}

createUserForm::~createUserForm()
{

}

void createUserForm::initForm()
{
    this->setWindowTitle("Create new SQL user/role dialog");

    QVBoxLayout* mainLay = new QVBoxLayout;

    this->setLayout(mainLay);


    QLabel* entityChooseLbl = new QLabel{"Select what do you want to create:"};

    entityChooseLbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    mainLay->addWidget(entityChooseLbl);


    nameLine_->setPlaceholderText(adb_utility::en_lit+adb_utility::digits_lit+adb_utility::spec_chars_lit);

    QRegExp regex ("["+adb_utility::en_lit+adb_utility::digits_lit+adb_utility::spec_chars_lit+"]*");
    QRegExpValidator * v = new QRegExpValidator (regex, this);
    nameLine_->setValidator (v);
//    connect(nameLine_, &QLineEdit::destroyed,[=]{ delete v; /*qDebug() << "~createUser::validator";*/ });

    userRButton_->setText("create User");



    roleRButton_->setText("create Role");



    QHBoxLayout* entityChooseSubLay = new QHBoxLayout;

    entityChooseSubLay->addWidget(userRButton_);
    entityChooseSubLay->addWidget(roleRButton_);

    mainLay->addLayout(entityChooseSubLay);



    QLabel* setNameLbl = new QLabel{"set Name"};

    QGridLayout* credentialSubLay = new QGridLayout;

    mainLay->addLayout(credentialSubLay);

    credentialSubLay->addWidget(setNameLbl,0,0);

    credentialSubLay->addWidget(nameLine_,0,1);



    hostLbl_->setText("set Host");



    hostLine_->setPlaceholderText("'%' by default");

    credentialSubLay->addWidget(hostLbl_,1,0);

    credentialSubLay->addWidget(hostLine_,1,1);



    credentialSubLay->addWidget(setPasswordCheckBox_,2,0);

    setPasswordCheckBox_->setText( "set Password");

    credentialSubLay->addWidget(passwordFrame_,2,1);

    passwordFrame_->setFrameShape(QFrame::NoFrame);

    passwordFrame_->setFrameShadow(QFrame::Plain);

    QHBoxLayout* passwordLineLay = new QHBoxLayout{passwordFrame_};


    passwordLineLay->addWidget(passwordLine_);

    hidePassCheckBox_-> setText("Hide");

    passwordLineLay->addWidget(hidePassCheckBox_);

    passwordLineLay->setContentsMargins(0,0,0,0);



    buttonBox_ -> addButton(QDialogButtonBox::Ok);
    buttonBox_->addButton(QDialogButtonBox::Cancel);


    mainLay->addWidget(buttonBox_);



    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj==buttonBox_->button(QDialogButtonBox::Ok)||obj==buttonBox_->button(QDialogButtonBox::Cancel)){
                obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            } else {

                obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }



    QList<QRadioButton*> RButtonsInFormlist = this->findChildren<QRadioButton*>();
        foreach (auto obj, RButtonsInFormlist) {

                obj->setStyleSheet(adb_style::adbRadioStyleSheet);
        }

    QList<QComboBox*> comboBoxInFormlist = this->findChildren<QComboBox*>();
        foreach (auto obj, comboBoxInFormlist) {

                obj->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#fffffa","darkslategray"));
        }


    QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }


}



void createUserForm::initSignals()
{
    connect(hidePassCheckBox_,&QCheckBox::stateChanged,[=](int state_arg__){
        if (state_arg__)
            passwordLine_->setEchoMode(QLineEdit::Password);
        else
            passwordLine_->setEchoMode(QLineEdit::Normal);
    });


    connect(setPasswordCheckBox_,&QCheckBox::stateChanged,[=](int state__){
        if(state__){
            passwordFrame_->setEnabled(true);
        } else{
            passwordFrame_->setEnabled(false);
            hidePassCheckBox_->setChecked(true);
        }
    });


    hidePassCheckBox_->setChecked(true);

    setPasswordCheckBox_->setChecked(true);

    connect(userRButton_,&QRadioButton::toggled,[this](bool state__){
        if(state__){
            passwordFrame_->show();
            setPasswordCheckBox_->show();
            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
//                hostLine_->setEnabled(true);
                hostLine_->show();
                hostLbl_->show();
            }
            nameLine_->clear();
        }
    });

    userRButton_->setChecked(true);

    connect(roleRButton_,&QRadioButton::toggled,[this](bool state__){
        if(state__){
            passwordFrame_->hide();
            setPasswordCheckBox_->hide();
            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
//                hostLine_->setEnabled(false);
                hostLine_->hide();
                hostLbl_->hide();
                adjustSize();
            }
            nameLine_->clear();
        }
    });


    connect(buttonBox_, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(buttonBox_, &QDialogButtonBox::accepted,this,&createUserForm::createAccepted);
}



void createUserForm::createAccepted()
{

    QRegularExpression symb(".*["+adb_utility::en_lit+adb_utility::spec_chars_lit+"].*");

    QRegularExpressionValidator v_en(symb, 0);

    QString str = nameLine_->text();
    int pos=0;
    if(v_en.validate(str, pos)!=QValidator::Acceptable)
    {
        adb_utility::get_information_window(QMessageBox::Information,"Wrong user name",
                                            QString("Name must consist at least 1 letter (lower- or uppercase) or one of special symbols:`%1`.")
                                            .arg(adb_utility::spec_chars_lit),this);
        return;

        // regexp. validation success
    } else if(userRButton_->isChecked()){

            QString const currentName = nameLine_->text();

            QString currentHost="";
            (hostLine_->text().isEmpty())? currentHost = "%" : currentHost = hostLine_->text();

            QString const currentEntrie =  '\''+currentName+'\''+'@'+'\''+currentHost+'\'';

            QScopedPointer<QSqlQueryModel> __model{new QSqlQueryModel};

            QStringList existingUserHostPairs;

            if(db_connection::try_to_reopen(auth_,auth::con_name_))
                db_connection::set_query("Select concat(quote(user),'@',quote(host)) from mysql.user;",*(__model.get()),&existingUserHostPairs,auth::con_name_);

            // check existing user name-host pair
            for(auto i = 0;i!=existingUserHostPairs.size();++i){
             qDebug() << existingUserHostPairs.at(i);
                if(existingUserHostPairs.at(i)==currentEntrie){
                    adb_utility::get_information_window(QMessageBox::Warning,"Username-host pair exist",
                                                        "This pair of username-host already exist in DB. Set another name or/and host and try again.",
                                                        this);
                    return;
                }
            }

            QString createQueryText = QString("CREATE USER %1").arg(currentEntrie);

            if(setPasswordCheckBox_->isChecked())
                createQueryText += QString(" identified by '%1'").arg(passwordLine_->text());

            auto qry = QSqlQuery(QSqlDatabase::database(auth_.con_name_,false));



            if(qry.prepare(createQueryText)){
                if(qry.exec()){
                    QString const createSuccessMsg = QString("User record %1 has been successfully created at server.").arg(currentEntrie);
    //                    statusBar->get_line()->setText(createSuccessMsg);
                    emit message(createSuccessMsg);
                    qDebug() << createSuccessMsg;


                        this->close();
                        return;

                }
            }

            QString const error_msg = qry.lastError().text();
            adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
            qDebug() << error_msg;
            return;

    } else if(roleRButton_->isChecked()){


        QString newRoleName =  '\''+nameLine_->text()+'\'';

        if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){
            QString currentHost="";
            (hostLine_->text().isEmpty())? currentHost = "%" : currentHost = hostLine_->text();

            newRoleName+="@\'"+currentHost+'\'';
        }

        QScopedPointer<QSqlQueryModel> __model{new QSqlQueryModel};

        QStringList existingRoles;

        if(db_connection::try_to_reopen(auth_,auth::con_name_)){
            QString __query = "";
            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
                __query="select user from mysql.user where is_role=\'y\';";
            } else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){
                __query="select concat(quote(user),\'@\',quote(host)) from mysql.user where password_expired=\'y\' and account_locked=\'y\';";
            }
            db_connection::set_query(__query,*(__model.get()),&existingRoles,auth::con_name_);
        }

        // check existing user name-host pair
        for(auto i = 0;i!=existingRoles.size();++i){
             qDebug() << existingRoles.at(i);
            if(existingRoles.at(i)==newRoleName){
                adb_utility::get_information_window(QMessageBox::Warning,"Role exist",
                                                    "This role name already exist in DB. Set another name or/and host and try again.",
                                                    this);
                return;
            }
        }

        QString createQueryText = QString("CREATE ROLE %1").arg(newRoleName);

        qDebug() << createQueryText;

        auto qry = QSqlQuery(QSqlDatabase::database(auth_.con_name_,false));


        if(qry.exec(createQueryText)){
            QString const createSuccessMsg = QString("Role record %1 has been successfully created at server.").arg(newRoleName);
//                    statusBar->get_line()->setText(createSuccessMsg);
            emit message(createSuccessMsg);
            qDebug() << createSuccessMsg;


                this->close();
                return;

        }

        QString const error_msg = qry.lastError().text();
        adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
        qDebug() << error_msg;
        return;

    }
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



grantUserPermissionsForm::grantUserPermissionsForm(auth & auth__, QWidget *parent__)
    : QDialog(parent__)
    , auth_(auth__)
    , model_{new QSqlQueryModel{}}
    , sqlUsersListLbl_{new QLabel{}}
    , UserRButton_{new QRadioButton{}}
    , RoleRButton_{new QRadioButton{}}
    , sqlUsersComboBox_{new QComboBox{}}
    , sqlUsersListReloadButton_{new reloadButton{}}

    , specificPermissionsRButton_{new QRadioButton{}}
    , allPermissionsRButton_{new QRadioButton{}}

    , specificPermissionsSubFrame_{new QFrame{}}

    , permissionsLvlComboBox_{new QComboBox{}}
    , GlobalPermissionsWarningLbl_{new QLabel{}}

    , specificPermissionsDatabaseFrame_{new QFrame{}}
    , permissionsDatabaseSubBox_{ new QComboBox{}}

    , specificPermissionsTableFrame_{new QFrame{}}
    , permissionsTableSubBox_{new QComboBox{}}

    , allPermissionsWarningLbl_{new QLabel{}}
    , choosePermissionsButton_{new BlinkinButton{}}
    , doubleList_{new TwoListSelection{auth_,this}}

    , GrantOptCheckBox_{new QCheckBox{}}
    , GrantOptWarningLbl_{new QLabel{}}

    , buttonBox_{new QDialogButtonBox{}}
{
    initForm();
    initSignals();
}

grantUserPermissionsForm::~grantUserPermissionsForm()
{
    delete model_;

    db_connection::close(subConName_);
    db_connection::remove(subConName_);
}



void grantUserPermissionsForm::initForm()
{

    this->setWindowTitle("Grant permissions to SQL user dialog");

    QVBoxLayout* mainLay = new QVBoxLayout;

    mainLay->setSizeConstraint(QLayout::SetFixedSize);

    this->setLayout(mainLay);



    QLabel* topLbl = new QLabel{"Select what you want to give permissions to:"};

    topLbl->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);


    mainLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Preferred, QSizePolicy::Expanding));

    mainLay->addWidget(topLbl);

    QHBoxLayout* sqlUsersListLay = new QHBoxLayout;


    sqlUsersListLbl_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);


    UserRButton_->setText("User");
    RoleRButton_->setText("Role");

    QHBoxLayout* userOrRoleSwitchSubLay = new QHBoxLayout;

    mainLay->addLayout(userOrRoleSwitchSubLay);

    userOrRoleSwitchSubLay->addWidget(UserRButton_);
    userOrRoleSwitchSubLay->addWidget(RoleRButton_);


    mainLay->addWidget(sqlUsersListLbl_);

    sqlUsersListLbl_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    mainLay->addLayout(sqlUsersListLay);


    sqlUsersComboBox_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    sqlUsersComboBox_->setCurrentIndex(-1);

    sqlUsersListLay->addWidget(sqlUsersComboBox_);


//    sqlUsersListReloadButton_->QPushButton::setText("reload");//setText("reload");
    sqlUsersListReloadButton_->setBackgroundColour("darkslategray");
    sqlUsersListReloadButton_->setFontColour("snow");
    sqlUsersListReloadButton_->setBold(true);

    sqlUsersListLay->addWidget(sqlUsersListReloadButton_);


    QLabel* descrLbl = new QLabel{"Choose parameters:"};

    mainLay->addWidget(descrLbl);


    QFrame* permissionsLvlMainFrame = new QFrame;

    permissionsLvlMainFrame->setFrameShape(QFrame::StyledPanel);

    permissionsLvlMainFrame->setFrameShadow(QFrame::Raised);

    QVBoxLayout* permissionsLvlMainLay = new QVBoxLayout{permissionsLvlMainFrame};
    permissionsLvlMainLay->setSizeConstraint(QLayout::SetDefaultConstraint);

    permissionsLvlMainFrame->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);


    mainLay->addWidget(permissionsLvlMainFrame);


    QFrame* permissionsLvlSubFrame = new QFrame;

    permissionsLvlSubFrame->setFrameShape(QFrame::NoFrame);

    permissionsLvlSubFrame->setFrameShadow(QFrame::Plain);

    permissionsLvlSubFrame->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);


    QHBoxLayout* permissionsLvlSubLay = new QHBoxLayout{permissionsLvlSubFrame};
    permissionsLvlSubLay->setContentsMargins(0,0,0,0);
    permissionsLvlSubLay->setSpacing(0);

    QLabel* permissionsLvlLbl = new QLabel{"Permissions level: "};

    permissionsLvlSubLay->addWidget(permissionsLvlLbl);

    permissionsLvlLbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    permissionsLvlSubLay->addWidget(permissionsLvlComboBox_);

    permissionsLvlComboBox_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    permissionsLvlComboBox_->addItems(adb_utility::unpack_("Global, Database, Table"));



    GlobalPermissionsWarningLbl_->setText("[<b><font color=\"Red\">Warning</font></b>]");

    permissionsLvlSubLay->addWidget(GlobalPermissionsWarningLbl_);

    GlobalPermissionsWarningLbl_->setToolTip("Set GLOBAL (\"*.*\") access for SQL Server User may have related to potential risk");

    QString style = QString("QToolTip {padding-top:0px;padding-bottom:0px;color:yellow;font-weight:bold;background:brown;border:0px;}");

    GlobalPermissionsWarningLbl_->setStyleSheet(style);

    GlobalPermissionsWarningLbl_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    GlobalPermissionsWarningLbl_->setCursor(Qt::WhatsThisCursor);


    permissionsLvlMainLay->addWidget(permissionsLvlSubFrame);


    specificPermissionsDatabaseFrame_->setFrameShape(QFrame::NoFrame);

    specificPermissionsSubFrame_->setFrameShadow(QFrame::Plain);

    QHBoxLayout* permissionsDatabaseSubLay = new QHBoxLayout{specificPermissionsDatabaseFrame_};


    permissionsDatabaseSubLay->setContentsMargins(0,0,0,0);
    permissionsDatabaseSubLay->setSpacing(0);


    permissionsLvlMainLay->addWidget(specificPermissionsDatabaseFrame_);

    QLabel* permissionsDatabaseLbl = new QLabel{"Choose Database: "};
    permissionsDatabaseLbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    permissionsDatabaseSubLay->addWidget(permissionsDatabaseLbl);

    permissionsDatabaseSubLay->addWidget(permissionsDatabaseSubBox_);

    permissionsDatabaseSubBox_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);



    specificPermissionsTableFrame_->setFrameShape(QFrame::NoFrame);

    specificPermissionsTableFrame_->setFrameShadow(QFrame::Plain);


    QHBoxLayout* permissionsTableSubLay = new QHBoxLayout{specificPermissionsTableFrame_};


    permissionsTableSubLay->setContentsMargins(0,0,0,0);
    permissionsTableSubLay->setSpacing(0);


    permissionsLvlMainLay->addWidget(specificPermissionsTableFrame_);



    QLabel* permissionsTableLbl = new QLabel{"Choose Table: "};
    permissionsTableLbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    permissionsTableSubLay->addWidget(permissionsTableLbl);



    permissionsTableSubLay->addWidget(permissionsTableSubBox_);



    permissionsTableSubBox_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    permissionsTableSubBox_->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    permissionsDatabaseSubBox_->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    permissionsLvlComboBox_->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    specificPermissionsTableFrame_->hide();
    specificPermissionsDatabaseFrame_->hide();



    QFrame* grantPermissionsFrame = new QFrame;
    grantPermissionsFrame->setFrameShape(QFrame::StyledPanel);
    grantPermissionsFrame->setFrameShadow(QFrame::Raised);

    QVBoxLayout* grantPermissionsLay = new QVBoxLayout{grantPermissionsFrame};

    mainLay->addWidget(grantPermissionsFrame);

    specificPermissionsRButton_->setText("Select certain permissions");
    specificPermissionsRButton_->setObjectName("specificPermissions");
    std::cout << specificPermissionsRButton_->objectName().toStdString() << std::endl;


    allPermissionsRButton_->setText("Grant ALL permissions");
    allPermissionsRButton_->setObjectName("allPermissions");
    std::cout << allPermissionsRButton_->objectName().toStdString() << std::endl;

    grantPermissionsLay->addWidget(specificPermissionsRButton_);



    mainLay->addWidget(grantPermissionsFrame);


    specificPermissionsSubFrame_->setFrameShape(QFrame::NoFrame);
    specificPermissionsSubFrame_->setFrameShadow(QFrame::Plain);
    specificPermissionsSubFrame_->setContentsMargins(0,0,0,0);
    specificPermissionsSubFrame_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    choosePermissionsButton_->setMinimumHeight(12);

    QHBoxLayout* specificPermissionsSubLay = new QHBoxLayout{specificPermissionsSubFrame_};
    specificPermissionsSubLay->setSpacing(0);

    specificPermissionsSubLay->setSizeConstraint(QLayout::SetMinimumSize);

    specificPermissionsSubLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Preferred, QSizePolicy::Preferred));
    specificPermissionsSubLay->addWidget(choosePermissionsButton_);
    //specificPermissionsSubLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));

    grantPermissionsLay->addWidget(specificPermissionsSubFrame_);




    QFrame* grantPermissionsSeparateline = new QFrame;
    grantPermissionsSeparateline->setFrameShape(QFrame::HLine);
    grantPermissionsSeparateline->setFrameShadow(QFrame::Sunken);

    grantPermissionsLay->addWidget(grantPermissionsSeparateline);



    style = QString("QToolTip {padding-top:0px;padding-bottom:0px;color:yellow;font-weight:bold;background:brown;border:0px;}");

    allPermissionsWarningLbl_->setText("[<b><font color=\"Red\">Warning</font></b>]");

    allPermissionsWarningLbl_->setToolTip("Set full (root) access for SQL Server User may have potential risk");

    allPermissionsWarningLbl_->setStyleSheet(style);

    allPermissionsWarningLbl_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    allPermissionsWarningLbl_->setCursor(Qt::WhatsThisCursor);

    allPermissionsWarningLbl_->hide();


    choosePermissionsButton_->setObjectName("permissionsButton");

    choosePermissionsButton_->setText("Select..");

    choosePermissionsButton_->setStyleSheet("min-height: 38px;max-height: 38px;background: floralwhite; color: darkslategray; font-weight:bold;");

    choosePermissionsButton_->changeBlinkSS("min-height: 38px;max-height: 38px;background: darkslategray; color: snow; font-weight:bold;");

    choosePermissionsButton_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    QHBoxLayout* allPermissionsRButtonSubLay = new QHBoxLayout;

    allPermissionsRButtonSubLay->addWidget(/*allPermissionsRButton_Lbl*/allPermissionsRButton_);

    allPermissionsRButtonSubLay->addWidget(allPermissionsWarningLbl_);


    grantPermissionsLay->addLayout(allPermissionsRButtonSubLay);



    QHBoxLayout* grantOptionSubLay = new QHBoxLayout;

    GrantOptCheckBox_->setText( "with GRANT option");

    GrantOptCheckBox_->setObjectName("grantOption");
    std::cout << GrantOptCheckBox_->objectName().toStdString() << std::endl;

    mainLay->addLayout(grantOptionSubLay);

    grantOptionSubLay->addWidget(GrantOptCheckBox_);


    GrantOptWarningLbl_->setText("[<b><font color=\"Red\">Warning</font></b>]");

    GrantOptWarningLbl_->setToolTip("This permission allow the SQL Server User to give another accounts additional privileges not intended by the administrator.");

    grantOptionSubLay->addWidget(GrantOptWarningLbl_);

    GrantOptWarningLbl_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    GrantOptWarningLbl_->setCursor(Qt::WhatsThisCursor);

    GrantOptWarningLbl_->hide();



    buttonBox_->addButton(QDialogButtonBox::Ok);
    buttonBox_->addButton(QDialogButtonBox::Cancel);

    mainLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Preferred, QSizePolicy::Expanding));

    mainLay->addWidget(buttonBox_);



    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj==buttonBox_->button(QDialogButtonBox::Ok)||obj==buttonBox_->button(QDialogButtonBox::Cancel)){
                obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            } else
            // if button is inherited class object -> cast pointer to inherit type
            if(obj->objectName()=="reloadButtonObj"){

                auto reloadButtonObj = qobject_cast<reloadButton*>(obj);
                reloadButtonObj->setKhakiHighlight();

            } else if(obj->objectName()=="permissionsButton"){

                // if button is inherited class object -> cast pointer to inherit type
                auto reloadButtonObj = qobject_cast<BlinkinButton*>(obj);

                reloadButtonObj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                                   .arg(reloadButtonObj->styleSheet())
                                   .arg(adb_style::getbuttonKhakiHiglightSS()));

                reloadButtonObj->changeBlinkSS(QStringLiteral("QPushButton {%1} %2")
                                               .arg(reloadButtonObj->blinkSS())
                                               .arg(adb_style::getbuttonKhakiHiglightSS()));

            }else {

                obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }



    QList<QRadioButton*> RButtonsInFormlist = this->findChildren<QRadioButton*>();
        foreach (auto obj, RButtonsInFormlist) {

                obj->setStyleSheet(adb_style::adbRadioStyleSheet);
        }

    QList<QComboBox*> comboBoxInFormlist = this->findChildren<QComboBox*>();
        foreach (auto obj, comboBoxInFormlist) {

                obj->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#fffffa","darkslategray"));
        }



    QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }




    QList<QPushButton*> ButtonsInDoublelist = doubleList_->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInDoublelist) {

            obj->setStyleSheet(QStringLiteral("QPushButton:!disabled { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                .arg(adb_style::getbuttonKhakiHiglightSS()));

        }

    QList<QListWidget*> listsInDoublelist = doubleList_->findChildren<QListWidget*>();
        foreach (auto obj, listsInDoublelist) {

            obj->setAlternatingRowColors(true);
            obj->setPalette(QPalette(adb_style::whiteUndGrayColor));

        }


}



void grantUserPermissionsForm::initSignals()
{
    connect(UserRButton_,&QRadioButton::toggled,[this](bool state__){
        if(state__){
           sqlUsersListLbl_->setText("Select user:");
        }
        sqlUsersListReloadButton_->clicked();
    });

    connect(RoleRButton_,&QRadioButton::toggled,[this](bool state__){
        if(state__){
           sqlUsersListLbl_->setText("Select role:");
        }
        sqlUsersListReloadButton_->clicked();
    });

    UserRButton_->setChecked(true);

    connect(sqlUsersComboBox_,&QComboBox::currentTextChanged,[this](QString const& newRecord__){
        sqlUsersComboBox_->setToolTip(newRecord__);
    });

    connect(sqlUsersListReloadButton_,&QPushButton::clicked,[=]{
        if(db_connection::try_to_reopen(auth_,auth::con_name_)){
            if(UserRButton_->isChecked()){
                if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB))
                    db_connection::set_query("select concat(quote(user),\'@\',quote(host)) from mysql.user where is_role=\'n\';",model_,sqlUsersComboBox_,auth::con_name_);
                else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL))
                    db_connection::set_query("select concat(quote(user),\'@\',quote(host)) from mysql.user "
                                            "where password_expired=\'n\' or account_locked=\'n\';",model_,sqlUsersComboBox_,auth::con_name_);
            } else if(RoleRButton_->isChecked()){
                if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB))
                    db_connection::set_query("select user from mysql.user where is_role=\'y\';",model_,sqlUsersComboBox_,auth::con_name_);
                else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL))
                    db_connection::set_query("select concat(quote(user),\'@\',quote(host)) from mysql.user "
                                            "where password_expired=\'y\' and account_locked=\'y\';",model_,sqlUsersComboBox_,auth::con_name_);
            }
        }
    });

        sqlUsersListReloadButton_->click();

    connect(allPermissionsRButton_,&QRadioButton::toggled,[this](bool state__){
       if(!state__){
            allPermissionsWarningLbl_->hide();
        } else{
            chosenPrivilegesList_.clear();
            chosenPrivilegesList_.append("ALL PRIVILEGES");
            allPermissionsWarningLbl_->show();
            adjustSize();
            choosePermissionsButton_->stop_blinkin();
        }
    });

    connect(specificPermissionsRButton_,&QRadioButton::toggled,[this](bool state__){
        if(!state__){
//            choosePermissionsButton_->setEnabled(false);
            specificPermissionsSubFrame_->hide();
        } else{
//            choosePermissionsButton_->setEnabled(true);
            specificPermissionsSubFrame_->show();
            chosenPrivilegesList_.clear();
        }
    });

    specificPermissionsRButton_->setChecked(true);

    connect(permissionsLvlComboBox_,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[this]
    (int curInd__)
    {
        if(curInd__==permissionsLvl::Global){
//            db_connection::try_to_reopen()
            specificPermissionsTableFrame_->hide();
            specificPermissionsDatabaseFrame_->hide();

            choosePermissionsButton_->setEnabled(true);

            GlobalPermissionsWarningLbl_->show();

        } else{

            GlobalPermissionsWarningLbl_->hide();

            if(curInd__==permissionsLvl::Database){
            specificPermissionsDatabaseFrame_->show();
            specificPermissionsTableFrame_->hide();

            if(db_connection::try_to_reopen(auth_,auth::con_name_))
                db_connection::set_query("show databases;",&databaseBoxModel_,permissionsDatabaseSubBox_,auth::con_name_);
            if(!permissionsDatabaseSubBox_->count()){
                choosePermissionsButton_->setEnabled(false);
                //return;
            }

    //            return;

            } else if(curInd__==permissionsLvl::Table){
                specificPermissionsDatabaseFrame_->show();
                specificPermissionsTableFrame_->show();

                if(db_connection::try_to_reopen(auth_,auth::con_name_))
                    db_connection::set_query("show databases;",&databaseBoxModel_,permissionsDatabaseSubBox_,auth::con_name_);

    //            return;
            }

        }

        permissionsLvlComboBox_->setToolTip(permissionsLvlComboBox_->itemText(curInd__));
        choosePermissionsButton_->stop_blinkin();
    });



    connect(permissionsDatabaseSubBox_,&QComboBox::currentTextChanged,this,[this]
            (QString const& currentDBname__){

        db_connection::close(subConName_);

        auth auth_copy=auth_;

        auth_copy.db_name_=currentDBname__;

        QSqlDatabase::database(subConName_,false).setDatabaseName(auth_copy.db_name_);


        db_connection::try_to_reopen(auth_copy,subConName_);



        if(db_connection::set_query("SHOW TABLES;", &tableBoxModel_,permissionsTableSubBox_,subConName_)){
            // if no tables in DB -->> clear previous keys comboBox model (preventing model's nullptr calling)
            if(permissionsLvlComboBox_->currentIndex()==permissionsLvl::Table){
                if(!permissionsTableSubBox_->count()){
                    //submodel_2_.clear();

                    permissionsTableSubBox_->setToolTip("*no tables*");
                    //ui->ref_key_comboBox_2->setToolTip("*no corteges (empty table)*");
                    choosePermissionsButton_->setEnabled(false);
                    return;
                }
            }
            choosePermissionsButton_->setEnabled(true);

        }

        permissionsDatabaseSubBox_->setToolTip(currentDBname__);

    },Qt::QueuedConnection);



    connect(permissionsTableSubBox_,&QComboBox::currentTextChanged,this,[this]
            (QString const& currentTableName__){

        permissionsTableSubBox_->setToolTip(currentTableName__);

    });



    permissionsLvlComboBox_->setCurrentIndex(permissionsLvl::Database);



    connect(doubleList_,&TwoListSelection::export_list,[this](QStringList chosenPrivilegesList__){
        chosenPrivilegesList_=chosenPrivilegesList__;
    });


    connect(choosePermissionsButton_,&QPushButton::clicked,[this]{

        doubleList_->clear();
        QString SQLPermissions;

        int __currentPrivilegesInd = permissionsLvlComboBox_->currentIndex();

        if(__currentPrivilegesInd==permissionsLvl::Global){
            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB))
                SQLPermissions = "CREATE TABLESPACE,CREATE USER,FILE,PROCESS,RELOAD,REPLICATION CLIENT,REPLICATION SLAVE,SHOW DATABASES,SHUTDOWN,SUPER";
            else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL))
                SQLPermissions = "CREATE TABLESPACE,CREATE USER,FILE,PROCESS,RELOAD,REPLICATION CLIENT,REPLICATION SLAVE,SHOW DATABASES,SHUTDOWN,SUPER,CREATE ROLE,DROP ROLE";
        } else if(__currentPrivilegesInd==permissionsLvl::Database){
            SQLPermissions = "ALTER,ALTER ROUTINE,CREATE,CREATE ROUTINE,CREATE TEMPORARY TABLES,CREATE VIEW,DELETE,DROP,EVENT,EXECUTE,INDEX,INSERT,"
                                "LOCK TABLES,REFERENCES,SELECT,SHOW VIEW,TRIGGER,UPDATE";
        } else if(__currentPrivilegesInd==permissionsLvl::Table){
            SQLPermissions = "ALTER,CREATE,CREATE VIEW,DELETE,DROP,INDEX,INSERT,REFERENCES,SELECT,SHOW VIEW,TRIGGER,UPDATE";
        }

//        = "ALTER,ALTER ROUTINE,CREATE,CREATE ROLE,CREATE ROUTINE,CREATE TABLESPACE,"
//                                     "CREATE TEMPORARY TABLES,CREATE USER,CREATE VIEW,DELETE,DROP,DROP ROLE,EVENT,"
//                                     "EXECUTE,FILE,INDEX,INSERT,LOCK TABLES,PROCESS,PROXY,REFERENCES,RELOAD,REPLICATION CLIENT,"
//                                     "REPLICATION SLAVE,SELECT,SHOW DATABASES,SHOW VIEW,SHUTDOWN,SUPER,TRIGGER,UPDATE";

        doubleList_->addAvailableItems(adb_utility::unpack_(SQLPermissions,","));

//        doubleList_->setAttribute(Qt::WA_DeleteOnClose,true);
        doubleList_->setModal(true);
        doubleList_->show();

        choosePermissionsButton_->stop_blinkin();
    });



    connect(GrantOptCheckBox_,&QCheckBox::stateChanged,[this](int state__){
        if(!state__){
            GrantOptWarningLbl_->hide();
        } else{
            GrantOptWarningLbl_->show();
        }
    });



    connect(buttonBox_, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(buttonBox_, &QDialogButtonBox::accepted,this, &grantUserPermissionsForm::formAccepted);
}



void grantUserPermissionsForm::formAccepted()
{


    QString __recordType="";
    (UserRButton_->isChecked()) ? __recordType = "User" : __recordType = "Role";

    QString __curRecord = sqlUsersComboBox_->currentText();

    if(__curRecord.isEmpty()){
        adb_utility::get_information_window(QMessageBox::Information,"No User/Role select","You must provide "+__recordType+" SQL Server record before continue.",this);
        return;

    } else if(  // Choosed Database lvl && database list is empty
                    (permissionsLvlComboBox_->currentIndex()==permissionsLvl::Database
                     && permissionsDatabaseSubBox_->currentText().isEmpty())

          )
        {
            adb_utility::get_information_window(QMessageBox::Information,"No database provided","You must choose database for granting permissions from list"
                                                                                                " or choose another level of permissions before continue.",this);
            return;

        } else if(  // Choosed Table lvl && (database list is empty OR tables list is empty)
                         permissionsLvlComboBox_->currentIndex()==permissionsLvl::Table
                          && (permissionsDatabaseSubBox_->currentText().isEmpty()||permissionsTableSubBox_->currentText().isEmpty())

            )
        {

            adb_utility::get_information_window(QMessageBox::Information,"No tables provided","You must choose table for granting permissions from list"
                                                                                              " or choose another level of permissions before continue.",this);
            return;

        }else if(specificPermissionsRButton_->isChecked()
                && chosenPrivilegesList_.isEmpty())
        {
            if(!GrantOptCheckBox_->isChecked()){
                adb_utility::get_information_window(QMessageBox::Information,"No permissions select","Please, select privileges for grant to "+__recordType+" before continue.",this);
                choosePermissionsButton_->start_blinkin(890/*2000*/,25000);
                return;

            } else{
                  if(permissionsLvlComboBox_->currentIndex()!=permissionsLvl::Global){
                      QPointer <adbMessageBox> messageBox = new adbMessageBox(QMessageBox::Information,"No additional permissions select",
                                                    "You doesn't provide any additional privileges for grant to "+__recordType+" besides <b>GRANT</b> privilege."
                                                        " This permission allow the user to give another accounts additional privileges not intended by the administrator."
                                                            " Do you want to continue anyway?",
                                                                      QMessageBox::Ok | QMessageBox::Cancel,this);

                      messageBox->setAttribute( Qt::WA_DeleteOnClose, true);

    //                      QFlag flags = QFlag(Qt::Window & ~Qt::WindowStaysOnTopHint);

    //                      messageBox->setWindowFlags(flags);
                      messageBox->setModal(true);
                      messageBox->show();
                      int dialogAnswer = messageBox->exec();

                      if(dialogAnswer!=QMessageBox::Ok){

                          choosePermissionsButton_->start_blinkin(890);
                          return;
                      }

                  } else {

                      QPointer <adbMessageBox> messageBox = new adbMessageBox(QMessageBox::Warning,"GRANT permission on ALL DATABASES",
                                                    "<b>Are you sure that you want provide <FONT COLOR='#ff0000'>GRANT option</FONT> to current "+__recordType+" for <FONT COLOR='#ff0000'>ALL DATABASES(!!!)</FONT>"
                                                        " on server? <FONT COLOR='#ff0000'>This will cause a serious potential risk.</FONT></b>",
                                                                      QMessageBox::Ok | QMessageBox::Cancel,this);

                      messageBox->setAttribute( Qt::WA_DeleteOnClose, true);

    //                      QFlag flags = QFlag(Qt::Window & ~Qt::WindowStaysOnTopHint);

    //                      messageBox->setWindowFlags(flags);
                      messageBox->setModal(true);
                      messageBox->show();
                      int dialogAnswer = messageBox->exec();

                      if(dialogAnswer!=QMessageBox::Ok){

                          choosePermissionsButton_->start_blinkin(890);
                          return;
                      }

                  }
            }

        } else if(allPermissionsRButton_->isChecked()
                  &&permissionsLvlComboBox_->currentIndex()==permissionsLvl::Global){

                QString warningHeader="";
                QString warningText="";

                if(!GrantOptCheckBox_->isChecked()){
                    warningHeader = "ROOT privilleges will provided";
                    warningText = "<b>You choose <FONT COLOR='#ff0000'>ROOT (\"Super-User\")</FONT> privileges for"
                                    " grant to selected "+__recordType+"(!!!)."
                                    "<br><FONT COLOR='#ff0000'>!!! Set full (root) access for SQL DB user may have serious potential risk !!!</FONT><br>"
                                                      " Do you want to continue anyway?</b>";
                } else{
                    warningHeader = "ROOT privilleges with GRANT OPTION will provided";
                    warningText = "<b>You choose <FONT COLOR='#ff0000'>ROOT (\"Super-User\")</FONT> privileges with <FONT COLOR='#ff0000'>GRANT OPTION</FONT> for"
                                    " grant to selected "+__recordType+"(!!!)."
                                    "<br><FONT COLOR='#ff0000'>!!! Set full (root) access for SQL DB user may have serious potential risk !!!</FONT><br>"
                                                      " Do you want to continue anyway?</b>";

                }
                    QPointer <adbMessageBox> messageBox = new adbMessageBox(QMessageBox::Warning,warningHeader,
                                                                            warningText,
                                                                    QMessageBox::Ok | QMessageBox::Cancel,this);

                    messageBox->setAttribute( Qt::WA_DeleteOnClose, true);

            //                      QFlag flags = QFlag(Qt::Window & ~Qt::WindowStaysOnTopHint);

            //                      messageBox->setWindowFlags(flags);
                    messageBox->setModal(true);
                    messageBox->show();
                    int dialogAnswer = messageBox->exec();

                    if(dialogAnswer!=QMessageBox::Ok){

                        choosePermissionsButton_->start_blinkin(890);
                        return;
                    }

        }

            if(grantPermissions2user(__recordType,__curRecord,static_cast<permissionsLvl>(permissionsLvlComboBox_->currentIndex()),chosenPrivilegesList_,GrantOptCheckBox_->isChecked(),
                                     permissionsDatabaseSubBox_->currentText(),permissionsTableSubBox_->currentText())){
                // success/error debug && messages signals handled inside function
                this->close();
            }

}



bool grantUserPermissionsForm::grantPermissions2user(QString const& recordType__,const QString &userHostRecord__, permissionsLvl permissionsLvl__,
                                                     const QStringList permissionsList__, bool GRANT_OPT,
                                                     QString const& database__, QString const& table__)
{
    if(db_connection::try_to_reopen(auth_,auth::con_name_)){

        auto qry = QSqlQuery(QSqlDatabase::database(auth_.con_name_,false));



        QString permissionsStringArg = adb_utility::pack_(permissionsList__,",");
        if(GRANT_OPT){
            if(permissionsStringArg!="ALL PRIVILEGES"){
                if(!permissionsStringArg.isEmpty())
                    permissionsStringArg.append(',');
                permissionsStringArg.append("GRANT OPTION");
            }
        }


        QString __privLvlSubStr;

        switch (permissionsLvl__) {
            case permissionsLvl::Global : __privLvlSubStr = "*.*"; break;
            case permissionsLvl::Database : __privLvlSubStr = database__+".*"; break;
            case permissionsLvl::Table : __privLvlSubStr = database__+'.'+table__;
        }

        QString __qryStr = QString("GRANT %0 ON %1 TO %2").arg(permissionsStringArg).arg(__privLvlSubStr).arg(userHostRecord__);

        QString __withGrantOpt="";

        if(GRANT_OPT){
            if(permissionsStringArg=="ALL PRIVILEGES"){
                __withGrantOpt = " WITH GRANT OPTION";
                __qryStr+=__withGrantOpt;

            }
        }

        qDebug() << "FINAL GRANT QUERY::" << __qryStr;

        if(qry.prepare(__qryStr)){
            if(qry.exec()){
                QString const text_success = QString("Permissions [%1] on `%2` for %3 `%4`%5 successfully granted.").arg(permissionsStringArg).arg(__privLvlSubStr).arg(recordType__).arg(userHostRecord__).arg(__withGrantOpt);
                emit message( text_success);
                qDebug() << text_success;
                return true;
            }
        }

        QString const error_msg = qry.lastError().text();
        adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
        qDebug() << error_msg;

    }

    return false;
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



getCredentialRecordsForm::getCredentialRecordsForm(auth &auth__, QWidget *parent__) :
    CustomQueryResult(auth__,parent__)
  , auth_{auth__}
  , userRB_{new QRadioButton{}}
  , roleRB_{new QRadioButton{}}
{
    initSignals();
    initForm();
}

getCredentialRecordsForm::~getCredentialRecordsForm()
{
    db_connection::close(subConName_);
    db_connection::remove(subConName_);
}

void getCredentialRecordsForm::initSignals()
{

    connect(userRB_,&QRadioButton::toggled,[this](int state__){
        if(state__){
            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
                query_ = "select user as \"SQL User\",host as Host from mysql.user where is_role=\'n\';";
            } else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){
                query_ = "select user as \"SQL User\",host as Host from mysql.user "
                                            "where password_expired=\'n\' or account_locked=\'n\';";
            }
            getRecords();
        }
    });

    connect(roleRB_,&QRadioButton::toggled,[this](int state__){
        if(state__){
            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
                query_ = "select user as \"SQL Role\" from mysql.user where is_role=\'y\';";
            } else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){
                query_ = "select user as \"SQL Role\",host as Host from mysql.user "
                                            "where password_expired=\'y\' and account_locked=\'y\';";
            }
            getRecords();
        }
    });

}

void getCredentialRecordsForm::initForm()
{
    userRB_->setText("Users");
    roleRB_->setText("Roles");

    userRB_->setChecked(true);

    QHBoxLayout* recordTypeMainLay = new QHBoxLayout;


    QFrame* recordChooseFrame = new QFrame;

    recordChooseFrame->setFrameShape(QFrame::Panel);

    recordChooseFrame->setFrameShadow(QFrame::Plain);


    QHBoxLayout* recordChooseLay = new QHBoxLayout{recordChooseFrame};


    recordTypeMainLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));
    recordChooseLay->addWidget(userRB_,0,Qt::AlignCenter);



    ////recordChooseLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

//    QFrame* recordTypeLineFrame = new QFrame{};
//    recordTypeLineFrame->setFrameShape(QFrame::VLine);
//    recordTypeLineFrame->setFrameShadow(QFrame::Raised);
//    recordChooseLay->addWidget(recordTypeLineFrame);

    ////recordChooseLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));






    recordChooseLay->addWidget(roleRB_,0,Qt::AlignCenter);

    recordTypeMainLay->addWidget(recordChooseFrame);
    recordTypeMainLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

    parent()->findChild<QVBoxLayout*>("mainLay")->insertLayout(0,recordTypeMainLay);

    setWindowTitle("SQL Users/Roles list");

    QList<QRadioButton*> RButtonsInFormlist = this->findChildren<QRadioButton*>();
        foreach (auto obj, RButtonsInFormlist) {

                obj->setStyleSheet(adb_style::adbRadioStyleSheet);
        }

}



void getCredentialRecordsForm::getRecords()
{
    CustomQueryResult::custom_query_slot(query_,subConName_);
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5



passwordMgmtForm::passwordMgmtForm(auth &auth__, QWidget *parent__)
    : QDialog(parent__)
    , auth_{auth__}

    , currentUserRB_{new QRadioButton{}}
    , anotherUserRB_{new QRadioButton{}}

    , chooseUserFrame_{new QFrame{}}

    , chooseUserComboBox_{new QComboBox{}}
    , reloadUsersListButton_{new reloadButton{}}
    , passwLine_{new QLineEdit{}}
    , hidePasswCheckBox_{new QCheckBox{}}
    , validPasswLine_{new QLineEdit{}}
    , buttonBox_{new QDialogButtonBox{}}
{
    initSignals();

    initForm();
}

passwordMgmtForm::~passwordMgmtForm()
{

}

void passwordMgmtForm::initSignals()
{
    connect(anotherUserRB_, &QRadioButton::toggled,[this](int state__){
        if(state__){
            chooseUserFrame_->setEnabled(true);
            emit reloadUsersListButton_ ->clicked();
        }
    });

    connect(currentUserRB_, &QRadioButton::toggled,[this](int state__){
        if(state__){
            if(db_connection::try_to_reopen(auth_,auth::con_name_))
                db_connection::set_query("SELECT concat(quote(SUBSTRING_INDEX(current_user(),"
                            "'@',1)),'@',quote(SUBSTRING_INDEX(current_user(),'@',-1)))",&model_,chooseUserComboBox_,auth::con_name_);
            chooseUserFrame_->setEnabled(false);
        }
    });

    connect(chooseUserComboBox_,&QComboBox::currentTextChanged,[this]
    (QString const& newEntry__)
    {
        chooseUserComboBox_->setToolTip(newEntry__);
    });

    connect(reloadUsersListButton_,&QPushButton::clicked,[this]{
        if(db_connection::try_to_reopen(auth_,auth::con_name_)){
            QString __query = "";
            if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
                __query = "select concat(quote(user),\'@\',quote(host)) from mysql.user where is_role=\'n\';";
            }else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){
                __query = "select concat(quote(user),\'@\',quote(host)) from mysql.user";
            }
            db_connection::set_query(__query,&model_,chooseUserComboBox_,auth::con_name_);
        }
    });

    connect(hidePasswCheckBox_,&QCheckBox::stateChanged,[=](int state_arg__){
        if (state_arg__ == Qt::Checked){
            passwLine_->setEchoMode(QLineEdit::Password);
            validPasswLine_->setEchoMode(QLineEdit::Password);
        }else {
            passwLine_->setEchoMode(QLineEdit::Normal);
            validPasswLine_->setEchoMode(QLineEdit::Normal);
        }
    });

    connect(buttonBox_,&QDialogButtonBox::rejected,this,&passwordMgmtForm::reject);

    connect(buttonBox_,&QDialogButtonBox::accepted,this,&passwordMgmtForm::formAccepted);
}

void passwordMgmtForm::initForm()
{
    this->setWindowTitle("Manage SQL Users passwords");

    QVBoxLayout* widgetLay = new QVBoxLayout;

    this->setLayout(widgetLay);



    QFrame* userFrame = new QFrame;

    userFrame->setFrameShape(QFrame::StyledPanel);

    userFrame->setFrameShadow(QFrame::Raised);

    QVBoxLayout* userLay = new QVBoxLayout{userFrame};

    //
    QLabel* chooseUserLbl = new QLabel{};

    chooseUserLbl->setText("Choose SQL Server User:");

    chooseUserLbl->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //

    QHBoxLayout* userRBSubLay = new QHBoxLayout;

    currentUserRB_->setText( "Current SQL Server User");

    currentUserRB_->setChecked(true);

    anotherUserRB_->setText( "Another SQL Server User");

    userRBSubLay->addWidget(currentUserRB_);
    userRBSubLay->addWidget(anotherUserRB_);

    userLay->addWidget(chooseUserLbl);
    userLay->addLayout(userRBSubLay);

    widgetLay->addWidget(userFrame);


    chooseUserFrame_->setFrameShape(QFrame::NoFrame);

    QHBoxLayout* chooseUserLay = new QHBoxLayout{chooseUserFrame_};



    //chooseUserLay->addWidget(chooseUserLbl);

    chooseUserLay->addWidget(chooseUserComboBox_);

    chooseUserComboBox_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

//    reloadUsersListButton_->setText("reload");
    reloadUsersListButton_->setBackgroundColour("darkslategray");
    reloadUsersListButton_->setFontColour("snow");
    reloadUsersListButton_->setBold(true);


    chooseUserLay->addWidget(reloadUsersListButton_);

    userLay->addWidget(chooseUserFrame_);




    QFrame* passwordFrame = new QFrame;

    passwordFrame->setFrameShape(QFrame::StyledPanel);

    passwordFrame->setFrameShadow(QFrame::Raised);

    QGridLayout* passwordLay = new QGridLayout{passwordFrame};
    widgetLay->addWidget(passwordFrame);


//    emit reloadUsersListButton_ ->clicked();


    QLabel* passwLbl = new QLabel{"Set new password:"};

    passwordLay->addWidget(passwLbl,1,0);

    passwordLay->addWidget(passwLine_,1,1);


    hidePasswCheckBox_->setText("hide");
    passwordLay ->addWidget(hidePasswCheckBox_,1,2);
    hidePasswCheckBox_->setChecked(true);


    QLabel* validPassLbl = new QLabel{};
    validPassLbl->setText("Re-enter password:");


    passwordLay->addWidget(validPassLbl,2,0);
    passwordLay->addWidget(validPasswLine_,2,1);


    buttonBox_->addButton(QDialogButtonBox::Ok);
    buttonBox_->addButton(QDialogButtonBox::Cancel);

    widgetLay->addWidget(buttonBox_);


    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj==buttonBox_->button(QDialogButtonBox::Ok)||obj==buttonBox_->button(QDialogButtonBox::Cancel)){
                obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            } else if(obj->objectName()=="reloadButtonObj"){
                // if button is inherited class object -> cast pointer to inherit type
                auto reloadButtonObj = qobject_cast<reloadButton*>(obj);
                reloadButtonObj->setKhakiHighlight();
            }

        }


    QList<QRadioButton*> RButtonsInFormlist = this->findChildren<QRadioButton*>();
        foreach (auto obj, RButtonsInFormlist) {

                obj->setStyleSheet(adb_style::adbRadioStyleSheet);
        }

    QList<QComboBox*> comboBoxInFormlist = this->findChildren<QComboBox*>();
        foreach (auto obj, comboBoxInFormlist) {

                obj->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#fffffa","darkslategray"));
        }



    QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }
}



void passwordMgmtForm::formAccepted()
{
    if(passwLine_->text()!=validPasswLine_->text()){

//        adb_utility::get_information_window(QMessageBox::Warning,"Passwords confirmation wrong","Passwords must be identical. Please, try again.",this);
        QPointer <adbMessageBox> messageBox = new adbMessageBox(QMessageBox::Warning,"Passwords confirmation wrong",
                                                                "Passwords must be identical. Please, try again.",
                                                                QMessageBox::Ok,this);

        messageBox->setAttribute( Qt::WA_DeleteOnClose, true);
        messageBox->setModal(true);
        messageBox->show();

        return;

    } else if(passwLine_->text().isEmpty()&&validPasswLine_->text().isEmpty()){
        QPointer <adbMessageBox> messageBox = new adbMessageBox(QMessageBox::Warning,"Password empty",
                                                                "Entered password is empty. Do you really want assign an empty password?",
                                                        QMessageBox::Yes | QMessageBox::No,this);

        messageBox->setAttribute( Qt::WA_DeleteOnClose, true);
        messageBox->setModal(true);
        messageBox->show();

        auto reply = messageBox->exec();

        if(reply != QMessageBox::Yes)
            return;
    }

    if(db_connection::try_to_reopen(auth_,auth::con_name_)){

        auto qry = QSqlQuery(QSqlDatabase::database(auth::con_name_,false));

        QString __qryStr="",__curUsr=chooseUserComboBox_->currentText(),__curPassw = passwLine_->text();


        if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MARIADB)){
            __qryStr = QString("set password for %1 = Password('%2')").arg(__curUsr).arg(__curPassw);

        } else if(auth::SQLdriverMatch(auth_.db_driver_,SQLDBtype::MYSQL)){
            __qryStr = QString("set password for %1 = '%2'").arg(__curUsr).arg(__curPassw);
        }

        if(qry.prepare(__qryStr)){
            if(qry.exec()){
                QString const text_success = QString("Password for user %1 successfully changed.").arg(__curUsr);
                emit message( text_success);
                qDebug() << text_success;

                qry.prepare("SELECT concat(quote(SUBSTRING_INDEX(current_user(),"
                            "'@',1)),'@',quote(SUBSTRING_INDEX(current_user(),'@',-1)))");

                if(qry.exec()){

                    qry.first();
                    auto curentUser = qry.value(0).toString();
                    qDebug() << "curentUser:->" <<curentUser;

                    // update password info for QSqlDatabase.open()
                    if(curentUser==__curUsr){
                        auth_.passw_=__curPassw;
                    }

                }



                std::cout << "CURUSR::"<< __curUsr.toStdString() << std::endl;
                std::cout << "CURPASSW::"<< __curPassw.toStdString() << std::endl;


                std::cout << "auth_login_==" << auth_.login_.toStdString() << std::endl;
                std::cout << "auth_host_==" << auth_.host_.toStdString() << std::endl;
                std::cout << "auth_passw_==" << auth_.passw_.toStdString() << std::endl;

                this->close();
                return;
            }
        }

        QString const error_msg = qry.lastError().text();
        adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,this,true);
        qDebug() << error_msg;
    }

}


