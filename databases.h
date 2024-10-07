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

//#include <QEventLoop>

#include "tables.h"
#include "auth.h"
#include "select_cells.hpp"
#include "db_connection.h"
#include "create_db_name.h"
#include "delete_db.h"

#include "helping_stuff.h"

#include "sqldump.h"

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

//    void message_from_login(QString const&);

    /*void*/bool show_databases();

signals:

    void select_cells_signal(const QModelIndex &,const QModelIndex &);

    void show_tables_signal();

    void delete_form_request();

    void close_all_custom_windows_();

    void window_rise_signal();

private slots:

    void on_mysqldump_button_clicked();

    void get_query_wndw();

    void send_custom_query_slot(Custom_Query*);

private:
    Ui::Databases *ui;

    void init_signals();

    auth& auth_;

    Tables* tables_window_;

    QSqlQueryModel model_;

    create_db_name* new_db_window_;

    delete_db* delete_db_window_;



    inline void keyPressEvent(QKeyEvent *e) {
        if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {/* minimize */
            qDebug()<<"escape pressed (databases)";
            close();
        }
    }


    inline void closeEvent(QCloseEvent *event){
        QMessageBox::StandardButton reply = QMessageBox::warning(this, "Quit", "Do you want to quit?",
                                                                 QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::No) {
            qDebug() << "Database:: cancel closing";
            event->ignore();
//            return;
          } else {
            qDebug() << "Database:: closing accepted";
            event->accept();
          }
    }

    QString const subconnection_name_="Database::custom_query_connection";

};

#endif // DATABASES_H
