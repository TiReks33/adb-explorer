#ifndef SQLDUMP_H
#define SQLDUMP_H

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
#include <twolistselection.h>

//#include "databases.h"
class Databases;

class SqlDump_settings;

class SqlDump_credentials : public QDialog/*QWidget*/
{
    Q_OBJECT
public:
    explicit SqlDump_credentials(auth&,/*QWidget*/Databases *parent = nullptr);
    virtual ~SqlDump_credentials();
signals:

    void closed();

    void message(QString const&);

private:

    Databases* parent_;

    auth& auth_;

    QLabel* label_;
    QPushButton* current_credential_button_;
    QPushButton * another_credential_button_;
    QPushButton * exit_button_;

    void init_form();
    void init_connections();

    inline void closeEvent(QCloseEvent *event){ emit closed(); event->accept(); }

    QScopedPointer<SqlDump_settings> settings_dialog_;
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class SqlDump_name;
class SqlDump_db_choose;

class SqlDump_settings : public QDialog
{
    Q_OBJECT
public:
    explicit SqlDump_settings(auth&,/*QWidget*/SqlDump_credentials *parent = nullptr);
    virtual ~SqlDump_settings();
signals:

    void closed();

private:

    SqlDump_credentials* parent_;

    auth& auth_;
        QLabel* label_;
    QRadioButton* radio_but_0;
    QRadioButton* radio_but_1;
    QRadioButton* radio_but_2;

    QPushButton* close_button_;
    QPushButton* next_button_;

    void init_form();
    void init_connections();

    void choose_settings_interactive();

    void chose_tables_from_db(QString const&);

    void set_dump_name(auth&,QStringList&,QWidget*);

    inline void closeEvent(QCloseEvent *event){ emit closed(); event->accept(); }

    QScopedPointer<SqlDump_name> dump_name_window_;
    QScopedPointer<SqlDump_db_choose> dump_db_choose_window_;

    QScopedPointer<TwoListSelection> double_list_;

    QString const subconnection_name_="SqlDump_subconnection";
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class SqlDump_name : public QDialog
{
    Q_OBJECT
public:
    explicit SqlDump_name(auth&,/*QString const&,*/QStringList,QWidget *parent = nullptr);
    virtual ~SqlDump_name();
signals:
    void message(QString const&);
private:
    auth& auth_;
//    QString const args_;
    QStringList args_;

    QLabel* label_;
    QLineEdit* line_edit_;
    QDialogButtonBox* button_box_;

    void init_form();
    void init_connections();

    void get_dump();
};


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

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

    bool show_db();

private:
    auth& auth_;

//    QSqlQueryModel* model_;
    QSqlQueryModel model_;

    QLabel* label_;
    QComboBox* comboBox_;
    QDialogButtonBox* buttonBox_;
    QPushButton* reload_button_;

    void init_form();
    void init_connections();

    inline void closeEvent(QCloseEvent *event){ emit closed(); event->accept(); }
};

#endif // SQLDUMP_H
