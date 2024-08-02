//#ifndef DB_CONNECTION_CPP
//#define DB_CONNECTION_CPP
#include "db_connection.h"
#include "multi_connection.h"
#include "twolistselection.h"

QString db_connection::get_unique_con_name(const QMetaObject *class_meta_obj__, multi_connection *multi_con__)
{
    return QString(class_meta_obj__->className()+QString::number(multi_con__->unique_number_));
}

bool db_connection::open(auth& auth__)
{
//    qDebug()<<"TEST IS OPEN::"<<QSqlDatabase::database("QPSQL").isOpen();
    if(!QSqlDatabase::database(auth__.con_name_).isOpen()){
//    if(!QSqlDatabase::database(auth__.db_server_).isOpen()){
    QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth__.db_server_,auth__.con_name_);

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
        qDebug()<<("Server ::"+auth__.db_server_+":: from connection ::"+auth__.con_name_+":: already connected.");
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

//bool db_connection::open(auth & auth__, default_con_ value__)
//{
//    bool con=false;
//       if(value__==default_con_::default_){
//           con = db_connection::open(auth__);
//       } else if (value__==default_con_::non_default_){
//           con = db_connection::open(auth__,auth__.con_name_);
//       }
//       return con;
//}

//bool db_connection::open(auth &auth__, QString con_name__)
//{
//    //    qDebug()<<"TEST IS OPEN::"<<QSqlDatabase::database("QPSQL").isOpen();
//        //if(!QSqlDatabase::database().isOpen()){
//        if(!QSqlDatabase::database(con_name__).isOpen()){
//    //    if(!QSqlDatabase::database(auth__.db_server_).isOpen()){
//        QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth__.db_server_,con_name__);

//            db_connection_.setUserName(auth__.login_);

//            db_connection_.setPassword(auth__.passw_);

//            //db_connection_.setHostName("localhost");//<-remote IP

//            db_connection_.setDatabaseName(auth__.db_name_);

//            if(!db_connection_.open()){
//                qDebug() << ("(x)Error connection to database.");
//                return false;
//            }
//            else{
//                qDebug()<<("Database succesfull connected.");
//                return true;
//            }
//        }
//            qDebug()<<("11Server from connection ::"+con_name__+":: already connected.");
//            return true;
////            return false;
//}

//int db_connection::open(auth &auth__, QString con_name__,int)
//{
//    //    qDebug()<<"TEST IS OPEN::"<<QSqlDatabase::database("QPSQL").isOpen();
//        //if(!QSqlDatabase::database().isOpen()){
//        if(!QSqlDatabase::database(con_name__).isOpen()){
//    //    if(!QSqlDatabase::database(auth__.db_server_).isOpen()){
//        QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth__.db_server_,con_name__);

//            db_connection_.setUserName(auth__.login_);

//            db_connection_.setPassword(auth__.passw_);

//            //db_connection_.setHostName("localhost");//<-remote IP

//            db_connection_.setDatabaseName(auth__.db_name_);

//            if(!db_connection_.open()){
//                qDebug() << ("(x)Error connection to database.");
//                return 0;
//            }
//            else{
//                qDebug()<<("Database succesfull connected.");
//                return 1;
//            }
//        }
//            qDebug()<<("11Server from connection ::"+con_name__+":: already connected.");
//            return 2;
////            return false;

//}

//bool db_connection::open(auth &auth__, QMetaObject const * class_meta_obj__/*QString con_name__*/,multi_connection*multi_con__)
//{
////    if(!QSqlDatabase::database(multi_con__->con_name_).isOpen()){
//    QString con_name = /*class_meta_obj__->className()+QString::number(multi_con__->unique_number_);*/get_unique_con_name(class_meta_obj__,multi_con__);
//    int db_con = db_connection::open(auth__,con_name,1);
//    if(!db_con){
//        qDebug() << QString("(x)There is error while establishing connection %1.").arg(con_name);
//        return false;
//    } else if (db_con==1){
//        qDebug() << QString("Connection %1 successful established.").arg(con_name);

////        multi_con__->con_name_=con_name;
//        multi_con__->con_list_.append(con_name);
//        multi_con__->multi_con_names_.append(con_name);
//        ++multi_con__->con_counter_;
//        ++multi_con__->unique_number_;
//    } else { //if already exist
//        qDebug() << QString("Connection %1 already exist.").arg(con_name);
//    }

//    qDebug() << "Con counter after::"<< multi_con__->con_counter_;
////    QSqlQueryModel* model = new QSqlQueryModel;
////    int models_cap = multi_con_models_.length();
////    qDebug() << "length of multi_con_models_"<< models_cap;
////    multi_con_models_.append(model);

////    }
//    return true;
//}

//bool db_connection::open(auth &auth__, QString con_name__, multi_connection *multi_con__)
//{
//        int db_con = db_connection::open(auth__,con_name__,1);
//        if(!db_con){
//            qDebug() << QString("(x)There is error while establishing connection %1.").arg(con_name__);
//            return false;
//        } else if (db_con==1){
//            qDebug() << QString("Connection %1 successful established.").arg(con_name__);

//    //        multi_con__->con_name_=con_name;
//            multi_con__->con_list_.append(con_name__);
//            multi_con__->multi_con_names_.append(con_name__);
//            ++multi_con__->con_counter_;
//            ++multi_con__->unique_number_;
//        } else { //if already exist
//            qDebug() << QString("Connection %1 already exist.").arg(con_name__);
//        }

//        qDebug() << "Con counter after::"<< multi_con__->con_counter_;
//    //    QSqlQueryModel* model = new QSqlQueryModel;
//    //    int models_cap = multi_con_models_.length();
//    //    qDebug() << "length of multi_con_models_"<< models_cap;
//    //    multi_con_models_.append(model);

//    //    }
//        return true;
//}










//void db_connection::close()
//{
//    {
//        QSqlDatabase db = QSqlDatabase::database();
//        db.close();
//    }
//    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection/*QSqlDatabase::database().connectionName()*/ );
//}

void db_connection::close/*_con*/(const QString &con_)
{
//    {
        QSqlDatabase db = QSqlDatabase::database(con_);
        if(db.isOpen()){db.close(); qDebug() << con_+" connection was closed.";}
//    }
//    QSqlDatabase::removeDatabase( con_/*QSqlDatabase::database().connectionName()*/ );

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



//void db_connection::close(QStringList *con_list_)
//{
//    size_t con_sum=con_list_->size();
//    for(size_t i=0;i!=con_sum;++i)
//    {
//        db_connection::close/*_con*/(con_list_->at(i));
//    }
//    //con_list_->clear();
//}














// USES NON DEFAULT CONNECTION -->

bool db_connection::set_query(QString query__, QSqlQueryModel &model__, QTableView *tableView__, QHeaderView::ResizeMode scalemode__, QString con_name__)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(con_name__));

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

bool db_connection::set_query(QString query__, QSqlQueryModel &model__, QComboBox*comboBox, QString con_name__,int)
{
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(con_name__));

    qry.prepare(query__); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    comboBox->setModel(&model__);

//    comboBox->horizontalHeader()->setSectionResizeMode(scalemode);
qDebug() << "TUT VSE MENYAT NADO";
    return true;
    }
    return false;
}

bool db_connection::set_query(QString query__, QSqlQueryModel &model__, QListWidget*list, QString con_name__,int) //QListWidget
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

bool db_connection::set_query(QString query__, /*QSqlQueryModel &model__,*/ TwoListSelection*double_list, QString con_name__/*,int*/) //double-list class
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


//<<-

























// USES DEFAULT CONNECTION -->

/*static*/bool db_connection::set_query(QString query, QSqlQueryModel&model__, QAbstractItemView* itemView)
{
    //QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(auth::con_name_));

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
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(auth::con_name_));

    if(qry.prepare(query)){//; //MY_SQL_QUERY

        if(qry.exec()){
        model__.setQuery(qry);

        tableView->setModel(&model__);

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



/*static*/bool db_connection::set_query(QString query, QSqlQueryModel &model__, QComboBox *comboBox, int)
{
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(auth::con_name_));

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    comboBox->setModel(&model__);

//    comboBox->horizontalHeader()->setSectionResizeMode(scalemode);
qDebug() << "ZVEZDOCHKI ( I SEE A STARS )";
    return true;
    }
    return false;
}

bool db_connection::set_query(QString query, QSqlQueryModel *model__, QComboBox *comboBox, int)
{
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(auth::con_name_));

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
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(auth::con_name_));

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
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(auth::con_name_));

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
//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(auth::con_name_));

    qry.prepare(query); //MY_SQL_QUERY

    if(qry.exec()){
    model__.setQuery(qry);

    comboBox->setModel(&model__);

    //comboBox->horizontalHeader()->setSectionResizeMode(scalemode);

    return true;
    }
    return false;
}

//<<-

QString const auth::con_name_="ADBEXPLORER";

int multi_connection::con_counter_ = 0;
int multi_connection::unique_number_ = 0;
QStringList multi_connection::multi_con_names_;

//#endif // DB_CONNECTION_CPP
