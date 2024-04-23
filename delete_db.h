#ifndef DELETE_DB_H
#define DELETE_DB_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class delete_db;
}

class delete_db : public QDialog
{
    Q_OBJECT

public:
    explicit delete_db(QWidget *parent = nullptr);
    ~delete_db();

signals:
    void delete_form_send(QComboBox*);

    void delete_database(QComboBox*);

public slots:
    void delete_form_request_slot();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::delete_db *ui;
};

#endif // DELETE_DB_H
