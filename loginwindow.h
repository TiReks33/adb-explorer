#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QScreen>
#include <QCloseEvent>
#include <QToolTip>

#include "databases.h"
#include "tables.h"
#include "auth.h"
#include "db_connection.h"

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
    void closeEvent(QCloseEvent *event){event->accept();};



signals:
    void message_to_database_window(QString const&);


private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::loginWindow *ui;

    Databases* db_window_;

    auth auth_;


};
#endif // LOGINWINDOW_H
