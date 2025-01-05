#ifndef CREATE_DB_NAME_H
#define CREATE_DB_NAME_H

#include <QDialog>
#include <QString>
#include <QPushButton>

#include "adb-exp-utility.h"

namespace Ui {
class create_db_name;
}

class create_db_name : public QDialog
{
    Q_OBJECT
signals:

    void create_db_signal(QString const&query__);

    void create_db_signal(QString const& query__,QString const& newdb_name__);

public:
    explicit create_db_name(QWidget *parent = nullptr);
    ~create_db_name();



private:
    Ui::create_db_name *ui;
};

#endif // CREATE_DB_NAME_H
