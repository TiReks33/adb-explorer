#ifndef CREATE_TABLE_CONSTRUCTOR_H
#define CREATE_TABLE_CONSTRUCTOR_H

#include <QStackedWidget>

#include "QDebug"
#include <QString>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QList>

#include "customqueryresult.h"

namespace Ui {
class CreateTableConstructor;
}

class CreateTableConstructor : public QStackedWidget
{
    Q_OBJECT

public:
    explicit CreateTableConstructor(auth& auth__,QWidget *parent = nullptr);
    ~CreateTableConstructor();

    bool decimal_type_more_or_eq();

    bool add_attributes(QPlainTextEdit*);

    bool add_keys(QPlainTextEdit*);

public slots:

    void AttrTypeChanged(QString const &);

    void foreignkeychecked(int);

    void onDeleteChecked(int);

    void onUpdateChecked(int);

private slots:
    void on_next_0_clicked();

    void on_next_1_clicked();

    void on_send_button_clicked();

    void on_plus_button_1_clicked();

    void on_erase_button_1_clicked();

    void on_help_button_2_clicked();

    void on_reset_button_2_clicked();

    void on_plus_button_2_clicked();

private:
    Ui::CreateTableConstructor *ui;
    bool first_attribute_;
    QString sql_query_;
    bool first_key_;
    QList<QString> attributes_;

    auth& auth_;
    db_connection* non_static_connection_;
};

#endif // CREATE_TABLE_CONSTRUCTOR_H
