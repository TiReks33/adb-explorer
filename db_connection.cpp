#include "db_connection.h"


bool db_connection::open(auth& auth__)
{
//    qDebug()<<"TEST IS OPEN::"<<QSqlDatabase::database("QPSQL").isOpen();
    if(!QSqlDatabase::database().isOpen()){
//    if(!QSqlDatabase::database(auth__.db_server_).isOpen()){
    QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth__.db_server_);

        db_connection_.setUserName(auth__.login_);

        db_connection_.setPassword(auth__.passw_);

        //db_connection_.setHostName("localhost");//<-remote IP

        db_connection_.setDatabaseName(auth__.db_name_);

        if(!db_connection_.open()){
            qDebug() << ("(x)Error connection to database.");
            return false;
        }
        else{
            qDebug()<<("Database succesfull connected.");
            return true;
        }
    }
        qDebug()<<("Server from connection ::"+auth__.db_server_+":: already connected.");
        return true;

}

bool db_connection::open(auth &auth__, QString con_name__)
{
    //    qDebug()<<"TEST IS OPEN::"<<QSqlDatabase::database("QPSQL").isOpen();
        //if(!QSqlDatabase::database().isOpen()){
        if(!QSqlDatabase::database(con_name__).isOpen()){
    //    if(!QSqlDatabase::database(auth__.db_server_).isOpen()){
        QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth__.db_server_,con_name__);

            db_connection_.setUserName(auth__.login_);

            db_connection_.setPassword(auth__.passw_);

            //db_connection_.setHostName("localhost");//<-remote IP

            db_connection_.setDatabaseName(auth__.db_name_);

            if(!db_connection_.open()){
                qDebug() << ("(x)Error connection to database.");
                return false;
            }
            else{
                qDebug()<<("Database succesfull connected.");
                return true;
            }
        }
            qDebug()<<("11Server from connection ::"+con_name__+":: already connected.");
            return true;
}

void db_connection::close()
{
    {
        QSqlDatabase db = QSqlDatabase::database();
        db.close();
    }
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection/*QSqlDatabase::database().connectionName()*/ );
}

/*static*/bool db_connection::set_query(QString query, QSqlQueryModel&model__, QAbstractItemView* itemView)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    itemView->setModel(&model__);

    return true;
    }
    return false;
}

/*static*/bool db_connection::set_query(QString query, QSqlQueryModel&model__, QTableView *tableView, QHeaderView::ResizeMode scalemode)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    tableView->setModel(&model__);

    tableView->horizontalHeader()->setSectionResizeMode(scalemode);
//qDebug()<<"YESS";
    qDebug()<<"VOT ETO POVOROT4";

    return true;
    } else {
        QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
        qDebug() << qry.lastError().text();
    }
    return false;
}

bool db_connection::set_query(QString query__, QSqlQueryModel &model__, QTableView *tableView__, QHeaderView::ResizeMode scalemode__, QString db_name__)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(db_name__));

    qry.prepare(query__); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    tableView__->setModel(&model__);

    tableView__->horizontalHeader()->setSectionResizeMode(scalemode__);
//qDebug()<<"YESS";
    qDebug()<<"VOT ETO POVOROT4";
    return true;
    } else {
        QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
        qDebug() << qry.lastError().text();
    }
    return false;
}

/*static*/bool db_connection::set_query(QString query, QSqlQueryModel &model__, QComboBox *comboBox, int)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    comboBox->setModel(&model__);

//    comboBox->horizontalHeader()->setSectionResizeMode(scalemode);

    return true;
    }
    return false;
}

//bool db_connection::set_query(QString query, QSqlQueryModel &model__, QComboBox *comboBox, QHeaderView::ResizeMode scalemode)
//{
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

//    qry.prepare(query); //MY_SQL_QUERY

//    if(qry.exec()){
//    model__.setQuery(qry);

//    comboBox->setModel(&model__);

////    comboBox->horizontalHeader()->setSectionResizeMode(scalemode);

//    return true;
//    }
//    return false;
//}

bool db_connection::set_query(QString query, QSqlQueryModel&model__, QComboBox *comboBox, QHeaderView::ResizeMode scalemode)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    comboBox->setModel(&model__);

    //tableView->horizontalHeader()->setSectionResizeMode(scalemode);
qDebug()<<"VOT ETO POVOROT1";
    return true;
    }
    return false;
}

bool db_connection::set_query(QString query,/* QSqlQueryModel&model__,*/ QTableView *tableView, QHeaderView::ResizeMode scalemode/*, int*/)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    this->model_.setQuery(qry);

    tableView->setModel(&this->model_);

    tableView->horizontalHeader()->setSectionResizeMode(scalemode);
qDebug()<<"VOT ETO POVOROT2";
    return true;
    }
    return false;
}

//bool db_connection::set_query(QString query__, QTableView *tableView__, QHeaderView::ResizeMode scalemode, QString db_name__)
//{
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(db_name__));

//    qry.prepare(query__); //MY_SQL_QUERY

//    if(qry.exec()){
//    this->model_.setQuery(qry);

//    tableView__->setModel(&this->model_);

//    tableView__->horizontalHeader()->setSectionResizeMode(scalemode);
//qDebug()<<"VOT ETO POVOROT2";
//    return true;
//    }
//    return false;
//}

//                        bool db_connection::set_query(QString query, QSqlQueryModel&model__, QTableView *tableView, QHeaderView::ResizeMode scalemode, int)
//                        {
//                            QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

//                            qry.prepare(query); //MY_SQL_QUERY

//                            if(qry.exec()){
//                            model__.setQuery(qry);

//                            tableView->setModel(&model__);

//                            tableView->horizontalHeader()->setSectionResizeMode(scalemode);

//                            return true;
//                            }
//                            return false;
//                        }

//bool db_connection::set_query(QString query, QSqlQueryModel*model__, QTableView *tableView, QHeaderView::ResizeMode scalemode)
//{
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

//    qry.prepare(query); //MY_SQL_QUERY

//    if(qry.exec()){
//    model__->setQuery(qry);

//    tableView->setModel(model__);

//    tableView->horizontalHeader()->setSectionResizeMode(scalemode);

//    return true;
//    }
//    return false;
//}



/*static*/bool db_connection::set_query(QString query, QSqlQueryModel &model__, QComboBox *comboBox/*, QHeaderView::ResizeMode scalemode*/)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    comboBox->setModel(&model__);

    //comboBox->horizontalHeader()->setSectionResizeMode(scalemode);

    return true;
    }
    return false;
}
