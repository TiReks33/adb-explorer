#ifndef SQLUSERMANAGEMENT_H
#define SQLUSERMANAGEMENT_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QLayout>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QSqlQueryModel>
#include <QStatusBar>

#include "twolistselection.h"
#include "auth.h"
#include "adb-exp-utility.h"
#include "db_connection.h"
#include "blinkinbutton.h"
#include "customqueryresult.h"
#include "reloadbutton.h"

#include <iostream>


class createUserForm : public QDialog
{
    Q_OBJECT

public:

    explicit createUserForm(auth&,QWidget * = nullptr);

    virtual ~createUserForm();

signals:

    void message(QString const&);

private:
    auth& auth_;

    void initForm();

    void initSignals();

    QRadioButton* userRButton_ = nullptr;
    QRadioButton* roleRButton_ = nullptr;

    QLineEdit* nameLine_ = nullptr;

    QLabel* hostLbl_ = nullptr;
    QLineEdit* hostLine_ = nullptr;

    QCheckBox* setPasswordCheckBox_ = nullptr;
    QFrame* passwordFrame_ = nullptr;
    QLineEdit* passwordLine_ = nullptr;
    QCheckBox* hidePassCheckBox_ = nullptr;

    QDialogButtonBox* buttonBox_ = nullptr;

    void createAccepted();
};


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


class grantUserPermissionsForm : public QDialog
{
    Q_OBJECT

public:

    explicit grantUserPermissionsForm(auth&,QWidget* = nullptr);

    virtual ~grantUserPermissionsForm();

signals:

    void message(QString const&);


private:
    auth& auth_;

    QString const subConName_ = "sqlUsrMngmnt::grantSpecPermSubConnection";

    QPointer<QSqlQueryModel> model_;

    QLabel* sqlUsersListLbl_ = nullptr;

    QRadioButton* UserRButton_ = nullptr;
    QRadioButton* RoleRButton_ = nullptr;

    QPointer<QComboBox> sqlUsersComboBox_;

    reloadButton* sqlUsersListReloadButton_ = nullptr;


    QRadioButton* specificPermissionsRButton_ = nullptr;
    QRadioButton* allPermissionsRButton_ = nullptr;

    QFrame* specificPermissionsSubFrame_ = nullptr;

    QComboBox* permissionsLvlComboBox_ = nullptr;
    QLabel* GlobalPermissionsWarningLbl_ = nullptr;

    enum permissionsLvl{
        Global,
        Database,
        Table
    };

    bool grantPermissions2user(QString const& recordType__,const QString& userHostRecord__, permissionsLvl permissionsLvl__,
                               const QStringList permissionsList__, bool GRANT_OPT,
                               QString const& database__ ="", QString const& table__ ="");

    void formAccepted();


    QFrame* specificPermissionsDatabaseFrame_ = nullptr;
    QSqlQueryModel databaseBoxModel_;
    QComboBox* permissionsDatabaseSubBox_ = nullptr;

    QFrame* specificPermissionsTableFrame_ = nullptr;
    QSqlQueryModel tableBoxModel_;
    QComboBox* permissionsTableSubBox_ = nullptr;

    QLabel* allPermissionsWarningLbl_ = nullptr;

//        QPushButton* choosePermissionsButton_ = nullptr;
    BlinkinButton* choosePermissionsButton_ = nullptr;

    QPointer<TwoListSelection> doubleList_;
    QCheckBox* GrantOptCheckBox_ = nullptr;
    QLabel* GrantOptWarningLbl_ = nullptr;

    QStringList chosenPrivilegesList_;

    QDialogButtonBox* buttonBox_;

    void initForm();

    void initSignals();


};



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



class getCredentialRecordsForm : public CustomQueryResult
{
    Q_OBJECT

public:
    explicit getCredentialRecordsForm(auth& auth__,QWidget *parent__ = nullptr);

    virtual ~getCredentialRecordsForm();

    void getRecords();



private:

    auth& auth_;

    void initSignals();
    void initForm();

    QString const subConName_ = "getCredentialRecordsForm";

    QString query_ = "select user,host from mysql.user";

    QRadioButton* userRB_ = nullptr;
    QRadioButton* roleRB_ = nullptr;

};



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



class passwordMgmtForm : public QDialog
{
    Q_OBJECT
public:
    explicit passwordMgmtForm(auth& auth__,QWidget* parent__ = nullptr);

    virtual ~passwordMgmtForm();

public slots:

    void formAccepted();

signals:

    void message(QString const&);

private:
    auth& auth_;

    void initSignals();
    void initForm();

    QRadioButton* currentUserRB_ = nullptr;
    QRadioButton* anotherUserRB_ = nullptr;

    QFrame* chooseUserFrame_ = nullptr;

    QComboBox* chooseUserComboBox_ = nullptr;
    QSqlQueryModel model_;
    reloadButton* reloadUsersListButton_ = nullptr;
    QLineEdit* passwLine_ = nullptr;
    QCheckBox* hidePasswCheckBox_ = nullptr;
    QLineEdit* validPasswLine_ = nullptr;

    QDialogButtonBox* buttonBox_ = nullptr;
};

#endif // SQLUSERMANAGEMENT_H
