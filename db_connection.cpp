//#ifndef DB_CONNECTION_CPP
//#define DB_CONNECTION_CPP
#include "db_connection.h"
#include "multi_connection.h"
#include "twolistselection.h"

//QString db_connection::get_unique_con_name(const QMetaObject *class_meta_obj__, multi_connection *multi_con__)
//{
//    return QString(class_meta_obj__->className()+QString::number(multi_con__->unique_number_));
//}

bool db_connection::open_default(auth& auth__)
{
//    qDebug()<<"TEST IS OPEN::"<<QSqlDatabase::database("QPSQL").isOpen();
    if(!QSqlDatabase::database(auth::default_con_name_).isOpen()){
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
        qDebug()<<("Server ::"+auth__.db_server_+":: from connection ::"+auth::default_con_name_+":: already connected.");
        return true;
}


bool db_connection::open(auth &auth__, const QString &con_name__)
{
    //    qDebug()<<"TEST IS OPEN::"<<QSqlDatabase::database("QPSQL").isOpen();
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
            qDebug()<<("Server ::"+auth__.db_server_+":: from connection ::"+con_name__+":: already connected.");
            return true;
}


bool db_connection::reopen_exist(const QString &con_name_)
{
    if(QSqlDatabase::contains(con_name_)){
        qDebug() << "(✓)Connection ::"+con_name_+":: exist in QSqlDatabase list.";

        if(QSqlDatabase::database(con_name_).open()){
            qDebug()<<"(✓)Connection ::"+con_name_+":: successful reopened.";
            return true;
        } else {
            qDebug()<<"(x)Connection ::"+con_name_+":: failed to reopen.";
            return false;
        }


    } else {
        qDebug() << "(x)Connection ::"+con_name_+":: is not exist in QSqlDatabase list.";
    }
    return false;
}



void db_connection::close/*_con*/(const QString &con_)
{
    if(QSqlDatabase::contains(con_)){
//    {
        QSqlDatabase db = QSqlDatabase::database(con_);
        if(db.isOpen()){db.close(); qDebug() << con_+" connection was closed.";}
//    }
//    QSqlDatabase::removeDatabase( con_/*QSqlDatabase::database().connectionName()*/ );
      }  else {
                qDebug() << con_+" connection is not exist in QSql connections list. Nothing to close.";
            }
}

void db_connection::remove(const QString &con_)
{
    if(QSqlDatabase::contains(con_)){
        QSqlDatabase::removeDatabase( con_ );
        qDebug() << con_+" connection was removed from the QSql connections list.";
    } else {
        qDebug() << con_+" connection is not exist in QSql connections list. May be it was removed from it earlier.";
    }
}








bool db_connection::set_query(QString query__, QSqlQueryModel &model__, QListWidget*list, QString const & con_name__/*, QString con_name__,int*/) //QListWidget
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(con_name__));

    qry.prepare(query__); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    QStringList query_2_list;
    for(int i = 0; i < model__.rowCount(); ++i){
        qDebug() << QString("db_connection::QListWidget::model::record(%1)::").arg(i)<< model__.record(i).value(0).toString();
        query_2_list.append(model__.record(i).value(0).toString());
    }
    qDebug() << "query2list::"<<query_2_list;
    list->addItems(query_2_list);
//    comboBox->horizontalHeader()->setSectionResizeMode(scalemode);
qDebug() << "list after query::" << list;
    return true;
    }
    return false;
}

bool db_connection::set_query(QString query__, /*QSqlQueryModel &model__,*/ TwoListSelection*double_list, QString const & con_name__/*,int*/) //double-list class
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(con_name__));

    qry.prepare(query__); //MY_SQL_QUERY

    if(qry.exec()){
    QSqlQueryModel model;
    model.setQuery(qry);

    QStringList query_2_list;
    for(int i = 0; i < model.rowCount(); ++i){
        qDebug() << QString("db_connection::QListWidget::model::record(%1)::").arg(i)<< model.record(i).value(0).toString();
        query_2_list.append(model.record(i).value(0).toString());
    }
    qDebug() << "query2list::"<<query_2_list;
    //list->addItems(query_2_list);
    double_list->addAvailableItems(query_2_list);
//    comboBox->horizontalHeader()->setSectionResizeMode(scalemode);
//qDebug() << "list after query::" << list;
    return true;
    }
    return false;
}


/*static*/bool db_connection::set_query(QString query, QSqlQueryModel*model__, QAbstractItemView* itemView, QString const & con_name__)
{
    //QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(con_name__));

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__->setQuery(qry);

    itemView->setModel(model__);

    return true;
    }
    return false;
}

/*static*/bool db_connection::set_query(QString query, QSqlQueryModel*model__, QTableView *tableView, QHeaderView::ResizeMode scalemode, QString const & con_name__)
{
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(con_name__));

    if(qry.prepare(query)){//; //MY_SQL_QUERY

        if(qry.exec()){
        model__->setQuery(qry);

        tableView->setModel(model__);

        tableView->horizontalHeader()->setSectionResizeMode(scalemode); //QHeaderView::Stretch
    //qDebug()<<"YESS";
        qDebug()<<"VOT ETO POVOROT434";

        return true;
        } else {
            qDebug() << "(x)QSqlQuery execution failed.";
        }
    } /*else {*/
        QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
        qDebug() << qry.lastError().text();
    //}
    return false;
}




bool db_connection::set_query(QString query, QSqlQueryModel *model__, QComboBox *comboBox, QString const & con_name__)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(con_name__));

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__->setQuery(qry);

    comboBox->setModel(model__);
//    comboBox->horizontalHeader()->setSectionResizeMode(scalemode);
qDebug() << "ZVEZDOCHKI ( I SEE A STARS )";
    return true;
    }
    return false;
}



QString const auth::con_name_="ADBEXPLORER";
QString const auth::default_con_name_=QSqlDatabase::database().connectionName();


int multi_connection::con_counter_ = 0;
int multi_connection::unique_number_ = 0;
QStringList multi_connection::multi_con_names_;

//#endif // DB_CONNECTION_CPP
