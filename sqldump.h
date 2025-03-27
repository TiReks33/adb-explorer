#ifndef SQLDUMP_H
#define SQLDUMP_H

//******************************DUMPING DATABASE IN STEP-BY-STEP***************************************

#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QDebug>
#include <QPointer>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QProcess>
#include <QMessageBox>
#include <QComboBox>
#include <QSqlQueryModel>
#include <QCloseEvent>
#include <QLibrary>

#include "twolistselection.h"
#include "reloadbutton.h"
#include "passwordlineedit.h"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


class SqlDump_name;
class SqlDump_db_choose;
class SqlDump_credentials;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


// select type of dump (all dbs, select dbs, select tables of db)
class SqlDump_settings : public QDialog
{
    Q_OBJECT
public:
    explicit SqlDump_settings(auth&,/*QWidget*/SqlDump_credentials *parent = nullptr);
    virtual ~SqlDump_settings();

    // function for selecting tables from db
    void choose_tables_from_db(QString const&);

    // depending of what settings you choose, get specific dialog
    void choose_settings_interactive();

signals:

    void closed();

private:

    SqlDump_credentials* parent_ = nullptr;

    auth& auth_;
    QLabel* label_ = nullptr;
    QRadioButton* radio_but_0 = nullptr;
    QRadioButton* radio_but_1 = nullptr;
    QRadioButton* radio_but_2 = nullptr;

    QPushButton* close_button_ = nullptr;
    QPushButton* next_button_ = nullptr;

    void init_form();
    void init_connections();


    // get final form with db name choose
    void set_dump_name(auth&,QStringList&,QWidget*);

    inline void closeEvent(QCloseEvent *event){ emit closed(); event->accept(); }

    QScopedPointer<SqlDump_name> dump_name_window_;
    QScopedPointer<SqlDump_db_choose> dump_db_choose_window_;

    QScopedPointer<TwoListSelection> double_list_;

};



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



class SqlDump_db_choose : public QDialog
{
    Q_OBJECT
public:
    explicit SqlDump_db_choose(auth&,QWidget *parent = nullptr);
    virtual ~SqlDump_db_choose();
signals:

    void closed();

    void chosen_db_signal(QString const&);

public slots:

    // choose specific db before selecting tables in it
    bool show_db();

private:
    auth& auth_;

    QSqlQueryModel model_;

    QLabel* label_ = nullptr;
    QComboBox* comboBox_ = nullptr;
    QDialogButtonBox* buttonBox_ = nullptr;
    reloadButton* reload_button_ = nullptr;

    void init_form();
    void init_connections();

    inline void closeEvent(QCloseEvent *event){ emit closed(); event->accept(); }
};



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



// select user
class SqlDump_credentials : public QDialog
{
    Q_OBJECT

public slots:

    // not perfect name to method:); show window (form) with another SQL user's credentials
    void get_another_credentials_window();

public:
    explicit SqlDump_credentials(auth&,QWidget/*Databases*/ *parent = nullptr);
    virtual ~SqlDump_credentials();
signals:

    void closed();

    void message(QString const&);

    void rememberMeSig();

private:

    ICryptoPlugin* cryptoModule_ = nullptr;

    QPointer<QLineEdit> login_form_ = nullptr;
    QPointer<PasswordLineEdit> passw_form_ = nullptr;

    QPointer<QCheckBox> rememberMeCB_ = nullptr;

    auth& auth_;

    static QString const subconnection_name_;

    QLabel* label_ = nullptr;
    QPushButton* current_credential_button_ = nullptr;
    QPushButton * another_credential_button_ = nullptr;
    QPushButton * exit_button_ = nullptr;

    void init_form();
    void init_connections();


    inline void closeEvent(QCloseEvent *event){ emit closed(); event->accept(); }

    QScopedPointer<SqlDump_settings> settings_dialog_;
    friend void SqlDump_settings::choose_tables_from_db(const QString &);
    friend bool SqlDump_db_choose::show_db();
    friend void SqlDump_settings::choose_settings_interactive();

    void cryptoModuleInit();

    bool eventFilter(QObject *object, QEvent *event);
};



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



// final dialog in chain
class SqlDump_name : public QDialog
{
    Q_OBJECT
public:
    explicit SqlDump_name(auth&,QStringList,QWidget *parent = nullptr);
    virtual ~SqlDump_name();

signals:
    void message(QString const&);
private:
    auth& auth_;

    QStringList args_;

    QLabel* label_ = nullptr;
    QLineEdit* line_edit_ = nullptr;
    QDialogButtonBox* button_box_ = nullptr;

    void init_form();
    void init_connections();

    void get_dump();
};



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



#endif // SQLDUMP_H


