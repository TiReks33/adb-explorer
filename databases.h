#ifndef DATABASES_H
#define DATABASES_H

#include <QDialog>
#include <QStatusBar>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QMessageBox>
#include <QComboBox>
#include <QProcess>
#include <QVariant>
#include <QElapsedTimer>


#include "tables.h"
#include "auth.h"
#include "select_cells.hpp"
#include "db_connection.h"
#include "create_db_name.h"
#include "delete_db.h"

#include "helping_stuff.h"

namespace Ui {
class Databases;
}

class Databases : public QDialog
{
    Q_OBJECT

public:
    explicit /*Databases(QWidget *parent = nullptr);*/
             Databases(auth&, QWidget *parent = nullptr);
    ~Databases();



public slots:

    void message_to_status(QString const &) const;

    void message_from_login(QString const&);

    void show_databases();

signals:

    void select_cells_signal(const QModelIndex &,const QModelIndex &);

    void show_tables_signal();

    void delete_form_request();

private slots:

    void on_mysqldump_button_clicked();

private:
    Ui::Databases *ui;

    void init_signals();

    auth& auth_;

    Tables* tables_window_;

    QSqlQueryModel model_;

    create_db_name* new_db_window_;

    delete_db* delete_db_window_;


};

#endif // DATABASES_H
