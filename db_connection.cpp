#include "db_connection.h"


bool db_connection::open(auth& auth__)
{
    if(!QSqlDatabase::database().isOpen()){
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

void db_connection::close()
{
    {
        QSqlDatabase db = QSqlDatabase::database();
        db.close();
    }
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection/*QSqlDatabase::database().connectionName()*/ );
}

bool db_connection::set_query(QString query, QSqlQueryModel&model__, QAbstractItemView* itemView)
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

bool db_connection::set_query(QString query, QSqlQueryModel&model__, QTableView *tableView, QHeaderView::ResizeMode scalemode)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    tableView->setModel(&model__);

    tableView->horizontalHeader()->setSectionResizeMode(scalemode);

    return true;
    }
    return false;
}

bool db_connection::set_query(QString query, QSqlQueryModel &model__, QComboBox *comboBox, int)
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

    return true;
    }
    return false;
}

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



bool db_connection::set_query(QString query, QSqlQueryModel &model__, QComboBox *comboBox/*, QHeaderView::ResizeMode scalemode*/)
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
