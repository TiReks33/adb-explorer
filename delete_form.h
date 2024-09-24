#ifndef DELETE_FORM_H
#define DELETE_FORM_H

#include <QDialog>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
#include "ui_delete_form.h"

namespace Ui {
class delete_form;
}

class delete_form : public QDialog
{
    Q_OBJECT

public:
    explicit delete_form(QWidget *parent = nullptr,QString const & = "entity");
    virtual ~delete_form();

signals:
    void delete_form_send(QComboBox*);

    void delete_entity(QComboBox*);

public slots:
    virtual void delete_form_request_slot();

//private slots:
protected slots:
    virtual void on_buttonBox_accepted()=0;

//private:
protected:
    Ui::delete_form *ui;
    QString const entity_name_;
};

#endif // DELETE_FORM_H
