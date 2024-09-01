#include "customquerysettings.h"
#include "ui_customquerysettings.h"

CustomQuerySettings::CustomQuerySettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomQuerySettings)
{
    ui->setupUi(this);

    connect(ui->Close_button,&QPushButton::clicked,[=]{
        this->close();
    });
}

CustomQuerySettings::~CustomQuerySettings()
{
    delete ui;
}

