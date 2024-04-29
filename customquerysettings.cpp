#include "customquerysettings.h"
#include "ui_customquerysettings.h"

CustomQuerySettings::CustomQuerySettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomQuerySettings)
{
    ui->setupUi(this);
}

CustomQuerySettings::~CustomQuerySettings()
{
    delete ui;
}

void CustomQuerySettings::on_Close_button_clicked()
{
    this->close();
}
