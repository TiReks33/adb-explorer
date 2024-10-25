#include "db_connection.h"
#include "twolistselection.h"

QString const auth::con_name_="ADBEXPLORER";



//QString db_connection::get_unique_con_name(const QMetaObject *class_meta_obj__, multi_connection *multi_con__)
//{
//    return QString(class_meta_obj__->className()+QString::number(multi_con__->unique_number_));
//}



bool db_connection::open(auth &auth__, const QString &con_name__/*, QString const & options__*/)
{
    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

        //if(!QSqlDatabase::database(QSqlDatabase::database(con_name__).connectionName()).isOpen()){
        if(!database.isOpen()){

            QSqlDatabase db_connection = QSqlDatabase::addDatabase(auth__.db_driver_,con_name__);

            db_connection.setUserName(auth__.login_);

            db_connection.setPassword(auth__.passw_);

            //db_connection_.setHostName("localhost");//<-remote IP
            db_connection.setHostName(auth__.host_);

            db_connection.setPort(auth__.port_);

            db_connection.setDatabaseName(auth__.db_name_);

            // set SQL flag to reconnect MySQl/MariaDB connection that already exists in list and opened by QSqlDatabase::open
            // WHEN connection is lost
            if(auth__.db_driver_=="QMARIADB" || auth__.db_driver_=="QMYSQL" || auth__.db_driver_=="QMYSQL3"){
                db_connection.setConnectOptions("MYSQL_OPT_RECONNECT=1");
                qDebug() << "MySQL/MariaDB flag to auto-retrieve the connection when database is gone was set.";
            }

            if(!db_connection.open()){

                ////QMessageBox::warning(0,"SQL connection open failed","(x)Error occured while connect to database by connection ::"+con_name__+":: Error message: "+database.lastError().text(),QMessageBox::Close);
                qDebug() << ("(x)Error occured while connect to database by connection ::"+con_name__+":: .");
                return false;

            } else {
                qDebug()<<("(✓)Database by connection ::"+con_name__ +"::successfully connected.");
                return true;
            }
        }
            qDebug()<<("(✓)Server ::"+auth__.db_driver_+":: from connection ::"+con_name__+":: already connected.");
            return true;
}



bool db_connection::try_to_reopen(auth &auth__,const QString & con_name__)
{
        if(db_connection::reopen_exist(con_name__)){
            qDebug() << QString("(✓)Connection ::"+con_name__+":: successfully reopened.");
            return true;
        } else {
            qDebug() << QString("(x)Connection ::"+con_name__+":: failed to reopen. Trying to remove and re-establish connection to SQL DB.");
            db_connection::remove(con_name__);
        }
        if(db_connection::open(auth__,con_name__)){
            qDebug() << QString("(✓)Connection ::"+con_name__+":: successfully re-established.");
            return true;
        } else {
            qDebug() << "(x)Connection ::"+con_name__+":: failed to open.";
            //return false;
        }
    return false;
}



/*private*/bool db_connection::reopen_exist(const QString &con_name_)
{
    if(QSqlDatabase::contains(con_name_)){
        qDebug() << "(✓)Connection ::"+con_name_+":: exist in QSqlDatabase list.";
        QSqlDatabase database = QSqlDatabase::database(con_name_,false);
        //qDebug() << "DATABASE DEBUG::"<<database;
        if(!database.isOpen()){

            if(database.open()){
                qDebug()<<"(✓)Connection ::"+con_name_+":: successful reopened.";
                return true;
            } else {
                qDebug()<<"(x)Connection ::"+con_name_+":: failed to reopen.";
                return false;
            }

        } else {
            qDebug()<<"(✓)Connection ::"+con_name_+":: already opened.";
            return true;
        }

    } else {
        qDebug() << "(x)Connection ::"+con_name_+":: is not exist in QSqlDatabase list.";
    }
    return false;
}



void db_connection::close/*_con*/(const QString &con_name_)
{
    if(QSqlDatabase::contains(con_name_))
    {
        //{

            QSqlDatabase db = QSqlDatabase::database(con_name_,false);

            if(db.isOpen()){db.close(); qDebug() << con_name_+" connection was closed.";}
        //}

        //QSqlDatabase::removeDatabase( con_name_/*QSqlDatabase::database().connectionName()*/ );

    }   else {
            qDebug() << con_name_+" connection is not exist in QSql connections list. Nothing to close.";
        }
}



void db_connection::remove(const QString &con_name_)
{
    if(QSqlDatabase::contains(con_name_)){
        QSqlDatabase::removeDatabase( con_name_ );
        qDebug() << con_name_+" connection was removed from the QSql connections list.";
    } else {
        qDebug() << con_name_+" connection is not exist in QSql connections list. May be it was removed from it earlier.";
    }
}









bool db_connection::set_query(QString const& query__, QSqlQueryModel &model__, QListWidget*list, QString const & con_name__/*QSqlDatabase const &db__*/) //QListWidget
{
    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query__)){ //MY_SQL_QUERY

    if(qry.exec()){
        model__.setQuery(qry);

        QStringList query_2_list;
        for(int i = 0; i < model__.rowCount(); ++i){

            query_2_list.append(model__.record(i).value(0).toString());
        }

        list->addItems(query_2_list);

        return true;
        } else {
        qDebug() << "(x) QSqlQuery execution failed.";
        }
    }
    QString const error_msg = qry.lastError().text();
//    QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
    adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    qDebug() << error_msg;
    return false;
}

bool db_connection::set_query(QString const& query__, /*QSqlQueryModel &model__,*/ TwoListSelection*double_list, QString const & con_name__/*QSqlDatabase const &db__*/) //double-list class
{
    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query__)){ //MY_SQL_QUERY

        if(qry.exec()){
            QSqlQueryModel model;
            model.setQuery(qry);

            QStringList query_2_list;
            for(int i = 0; i < model.rowCount(); ++i){
                query_2_list.append(model.record(i).value(0).toString());
        }

        double_list->addAvailableItems(query_2_list);

        return true;

        } else {
            qDebug() << "(x) QSqlQuery execution failed.";
        }

    }

    QString const error_msg = qry.lastError().text();
//    QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
    adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    qDebug() << error_msg;
    return false;
}


/*static*/bool db_connection::set_query(QString const& query, QSqlQueryModel*model__, QAbstractItemView* itemView, QString const & con_name__/*QSqlDatabase const &db__*/)
{
    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query)){ //MY_SQL_QUERY
        qDebug() << "(✓) QSqlQuery prepare successful.";
        if(qry.exec()){
            qDebug() << "(✓) QSqlQuery execution successful.";
        model__->setQuery(qry);

        itemView->setModel(model__);

        return true;
        } else {
            qDebug() << "(x) QSqlQuery execution failed.";
        }

    }

    QString const error_msg = qry.lastError().text();
//    QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
    adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    qDebug() << error_msg;
    return false;
}



/*static*/bool db_connection::set_query(QString const& query, QSqlQueryModel*model__, QTableView *tableView, QHeaderView::ResizeMode scalemode, QString const & con_name__/*QSqlDatabase const &db__*/)
{

    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query)){//; //MY_SQL_QUERY
        qDebug() << "(✓) QSqlQuery prepare successful.";
        if(qry.exec()){
            qDebug() << "(✓) QSqlQuery execution successful.";
        model__->setQuery(qry);

        tableView->setModel(model__);

        tableView->horizontalHeader()->setSectionResizeMode(scalemode); //QHeaderView::Stretch

        return true;
        } else {
            qDebug() << "(x) QSqlQuery execution failed.";
        }
    }

    QString const error_msg = qry.lastError().text();
//    QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
    adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    qDebug() << error_msg;
    return false;
}




bool db_connection::set_query(QString const& query__, QSqlQueryModel *model__, QComboBox *comboBox, QString const & con_name__/*QSqlDatabase const &db__*/)
{

    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query__)){ //MY_SQL_QUERY
        qDebug() << "(✓) QSqlQuery prepare successful.";
        if(qry.exec()){
            qDebug() << "(✓) QSqlQuery execution successful.";

        model__->setQuery(qry);

        comboBox->setModel(model__);

        return true;
        } else {
            qDebug() << "(x) QSqlQuery execution failed.";
        }
    }

    QString const error_msg = qry.lastError().text();
//    QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
    adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    qDebug() << error_msg;
    return false;
}



//int multi_connection::con_counter_ = 0;
//int multi_connection::unique_number_ = 0;
//QStringList multi_connection::multi_con_names_;


