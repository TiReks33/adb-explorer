#include "newtable.h"
#include "ui_newtable.h"

NewTable::NewTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTable)
{
    ui->setupUi(this);
}

NewTable::~NewTable()
{
    delete ui;
}
