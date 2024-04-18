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

private slots:
    void on_showDB_button_clicked();

private:
    Ui::Databases *ui;
    //QStatusBar* status_bar_;
    QSqlDatabase db_connection_;
};

#endif // DATABASES_H
