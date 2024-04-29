#include "customqueryresult.h"
#include "ui_customqueryresult.h"

CustomQueryResult::CustomQueryResult(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomQueryResult)
  , auth_(auth__)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
}


void CustomQueryResult::custom_query_slot(QString query)
{
    db_connection::open(auth_);


    db_connection::set_query(query,model_,ui->tableView,QHeaderView::Stretch);
}


CustomQueryResult::~CustomQueryResult()
{
    delete ui;
}

void CustomQueryResult::on_Cancel_button_clicked()
{
    this->close();
}
