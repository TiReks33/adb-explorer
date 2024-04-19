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
             Databases(QSqlDatabase, QWidget *parent = nullptr);
    ~Databases();

public slots:
    void message_from_login(QString);

signals:
    void test_signal();

private slots:
    void on_showDB_button_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Databases *ui;
    //QStatusBar* status_bar_;
    QSqlDatabase db_connection_;
};

#endif // DATABASES_H
