#include "inserttupleconfirm.h"
#include "ui_inserttupleconfirm.h"

insertTupleConfirm::insertTupleConfirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::insertTupleConfirm)
{
    ui->setupUi(this);
}

insertTupleConfirm::~insertTupleConfirm()
{
    delete ui;
}
