#ifndef DELETE_FORM_H
#define DELETE_FORM_H

#include <QDialog>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
#include <QRadioButton>

#include "ui_delete_form.h"
#include "reloadbutton.h"

namespace Ui {
class delete_form;
}

// pure virtual class to be inherited by 'database', 'table' etc. forms
class delete_form : public QDialog
{
    Q_OBJECT

public:
    explicit delete_form(QWidget *parent = nullptr,QString const & = "entity");
    virtual ~delete_form();

    inline void setComboBoxLoadQueryText(QString const& newText__){ comboBoxLoadQueryText_ = newText__; }

    inline QString const& comboBoxLoadQueryText() const { return comboBoxLoadQueryText_;}

    void reload();

signals:
    void delete_form_send(QComboBox*);

    void delete_entity(QComboBox*);

    void reloadSig();

public slots:
    virtual void delete_form_request_slot();

protected slots:
    virtual void on_buttonBox_accepted()=0;

protected:
    Ui::delete_form *ui;
    QString const entity_name_;

    reloadButton* reloadEntitiesListButton_ = nullptr;
    QString comboBoxLoadQueryText_ = "";
};

#endif // DELETE_FORM_H
