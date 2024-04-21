#include <tables.h>
#include "ui_tables.h"

Tables::Tables(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tables)
  , db_server_("QMYSQL")
{
    ui->setupUi(this);

    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    //SIGNALS

}

Tables::~Tables()
{
    delete ui;
}

