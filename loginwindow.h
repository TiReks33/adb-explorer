#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include "databases.h"

QT_BEGIN_NAMESPACE
namespace Ui { class loginWindow; }
QT_END_NAMESPACE

class loginWindow : public QMainWindow
{
    Q_OBJECT

public:
    loginWindow(QWidget *parent = nullptr);
    ~loginWindow();

    // close-event override for save/db closing connection-purpose after app closing
    void closeEvent(QCloseEvent *event);

signals:
    void message_to_database(QString);

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_login_testButton_clicked();

private:
    Ui::loginWindow *ui;
    QSqlDatabase db_connection_;
    Databases* db_window_;
};
#endif // LOGINWINDOW_H
