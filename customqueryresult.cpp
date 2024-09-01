#include "customqueryresult.h"
#include "ui_customqueryresult.h"

CustomQueryResult::CustomQueryResult(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomQueryResult)
  , auth_(auth__)

{
    ui->setupUi(this);

    Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                        | Qt::WindowMinimizeButtonHint
                                        | Qt::WindowMaximizeButtonHint
                                        | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    connect(ui->Cancel_button,&QPushButton::clicked,[=]{
        this->close();
    });
}



void CustomQueryResult::custom_query_slot(QString const & query__, QString const & con_name__, QHeaderView::ResizeMode scalemode__)
{

    if(db_connection::try_to_reopen(auth_,con_name__))


    db_connection::set_query(query__,&model_,ui->tableView,scalemode__,con_name__);
}



CustomQueryResult::~CustomQueryResult()
{
    qDebug()<<"~CustomQueryResult activated";
    delete ui;
}


void CustomQueryResult::close_window()
{
    qDebug()<<"Close custom query form signal handled";
    this->close();
}
