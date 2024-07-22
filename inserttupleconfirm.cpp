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

void insertTupleConfirm::final_query_slot(QString __fin_query)
{
    ui->plainTextEdit->setPlainText(__fin_query);
}
