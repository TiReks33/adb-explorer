#include "customqueryresult.h"
#include "ui_customqueryresult.h"

CustomQueryResult::CustomQueryResult(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomQueryResult)
  , auth_(auth__)
  //, non_static_connection_(new db_connection)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
}


//void CustomQueryResult::custom_query_slot(QString query)
//{
//    db_connection::open(auth_);


//    db_connection::set_query(query,&model_,ui->tableView,QHeaderView::Stretch);
//}



void CustomQueryResult::custom_query_slot(/*auth & auth__,*/ QString const & query__, QString const & con_name__, QHeaderView::ResizeMode scalemode__)
{
    //db_connection::open(auth__,con_name__);
    if(db_connection::try_to_reopen(/*auth__,*/auth_,con_name__))


    db_connection::set_query(query__,&model_,ui->tableView,scalemode__,con_name__);
}



//void CustomQueryResult::custom_query_slot(QString query, QTableView * tableview_)
//{
//    db_connection::open(auth_);

//QSqlQueryModel* model__ = new QSqlQueryModel;
//    db_connection::set_query(query,model__,tableview_,QHeaderView::Stretch);
//}

//void CustomQueryResult::custom_query_slot(QString query,QTableView*tableView)
//{
//    //db_connection non_static_con;
//    db_connection::open(auth_);


//    db_connection::set_query(query,&model_,tableView,QHeaderView::Stretch);
//}

//void CustomQueryResult::custom_query_slot(QString query,QTableView*tableView,QString db_name__)
//{
//    //db_connection non_static_con;
////    db_connection::open(auth_,db_name__);
//    db_connection::open(auth_);

//qDebug() << "ZAYCHIK";
//    db_connection::set_query(query,&this->/*sub_*/model_,tableView,QHeaderView::Stretch/*,db_name__*/);
//}

//void CustomQueryResult::custom_query_slot(QString query,QComboBox*comboBox)
//{
//    //db_connection non_static_con;
//    db_connection::open(auth_);


//    db_connection::set_query(query,&this->model_,comboBox/*,QHeaderView::Stretch*/);
//}

//void CustomQueryResult::custom_query_slot(QString query, QSqlQueryModel model_, QTableView *tableView)
//{
//    db_connection* non_static_con = new db_connection;
//    db_connection::open(auth_);


//    non_static_con->set_query(query,tableView,QHeaderView::Stretch,1);
//}


CustomQueryResult::~CustomQueryResult()
{
    delete ui;
  //  delete non_static_connection_;
}

void CustomQueryResult::on_Cancel_button_clicked()
{
    this->close();
}

void CustomQueryResult::close_window()
{
    qDebug()<<"Close custom query form signal handled";
    this->close();
}
