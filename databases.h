#ifndef DATABASES_H
#define DATABASES_H

#include <QDialog>
#include <QStatusBar>
#include <QSqlDatabase>

namespace Ui {
class Databases;
}

class Databases : public QDialog
{
    Q_OBJECT

public:
    explicit Databases(QWidget *parent = nullptr);
//             Databases(QSqlDatabase, QWidget *parent = nullptr);
    ~Databases();


             void close_DB();

public slots:
    void message_from_login(QString);

    void receive_auth(QString,QString,QString);

signals:
    void test_signal();

private slots:
    void on_showDB_button_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Databases *ui;
    //QStatusBar* status_bar_;
    //QSqlDatabase db_connection_;
    QString db_server_;
    QString db_login_;
    QString db_passw_;
    QString db_host_;
};

#endif // DATABASES_H
