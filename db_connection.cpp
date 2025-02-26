#include "db_connection.h"
#include "twolistselection.h"

#define __CH " Check log for more info."

bool db_connection::open(auth &auth__, const QString &con_name__/*, QString const & options__*/)
{
    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

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
            if(auth::SQLdriverMatch(auth__.db_driver_,SQLDBtype::MARIADB) || auth::SQLdriverMatch(auth__.db_driver_,SQLDBtype::MYSQL)){
                db_connection.setConnectOptions("MYSQL_OPT_RECONNECT=1");
                std::cout << "MySQL/MariaDB flag to auto-retrieve connection when database is gone was set." << std::endl;
            }

            if(!db_connection.open()){

                ////QMessageBox::warning(0,"SQL connection open failed","(x)Error occured while connect to database by connection ::"+con_name__+":: Error message: "+database.lastError().text(),QMessageBox::Close);
                auto errorText = "(x)Error occured while connect to database by connection ::"+con_name__+"::"+__CH;
                qDebug() << errorText;
                std::cout << errorText.toStdString() << std::endl;
                qDebug() << db_connection.lastError().text();

                return false;

            } else {

                auto successCon = "(✓)SQL Server connection ::"+con_name__ +":: successfully established.";
                qDebug()<< successCon;
                std::cout << successCon.toStdString() << std::endl;
                return true;
            }
        }
            auto alreadyCon = "(✓)SQL Server connection ::"+con_name__+":: already established by driver ::"+auth__.db_driver_+":: .";
            //qDebug()<<alreadyCon;
            std::cout << alreadyCon.toStdString() << std::endl;
            return true;
}



bool db_connection::try_to_reopen(auth &auth__,const QString & con_name__)
{
    auto stdCon = con_name__.toStdString();

        if(db_connection::reopen_exist(con_name__)){

            std::cout << "(✓)Connection ::"+stdCon+":: successfully reopened." << std::endl;
            return true;

        } else {

            std::cout << "(x)Connection ::"+stdCon+":: failed to reopen. Trying to remove and re-establish connection to SQL DB." << std::endl;
            db_connection::remove(con_name__);

        }

        if(db_connection::open(auth__,con_name__)){

            std::cout << "(✓)Connection ::"+stdCon+":: successfully re-established." << std::endl;
            return true;

        } else {
            auto failMsg = "(x)Connection ::"+con_name__+":: failed to open by scratch.";
            qDebug() << failMsg;
            std::cout << failMsg.toStdString() << std::endl;

        }

    return false;
}



/*private*/bool db_connection::reopen_exist(const QString &con_name_)
{
    auto stdCon = con_name_.toStdString();

    if(QSqlDatabase::contains(con_name_)){

        std::cout << "(✓)Connection ::"+stdCon+":: exist in QSqlDatabase list." << std::endl;
        QSqlDatabase database = QSqlDatabase::database(con_name_,false);
        //qDebug() << "DATABASE DEBUG::"<<database;
        if(!database.isOpen()){

            if(database.open()){

                std::cout << "(✓)Connection ::"+stdCon+":: successful reopened." << std::endl;
                return true;

            } else {

                auto errConMsg = "(x)Connection ::"+con_name_+":: failed to reopen."+__CH;
                qDebug()<<errConMsg;
                std::cout << errConMsg.toStdString() << std::endl;

                return false;

            }

        } else {

            std::cout <<"(✓)Connection ::"+stdCon+":: already opened." << std::endl;
            return true;
        }

    } else {

        std::cout << "(x)Connection ::"+stdCon+":: is not exist in QSqlDatabase list." << std::endl;
    }

    return false;
}



void db_connection::close/*_con*/(const QString &con_name_)
{
    auto stdCon = con_name_.toStdString();
    if(QSqlDatabase::contains(con_name_))
    {
        //{

            QSqlDatabase db = QSqlDatabase::database(con_name_,false);

            if(db.isOpen()){db.close();
                std::cout << stdCon+" connection was closed." << std::endl;
            }
        //}

        //QSqlDatabase::removeDatabase( con_name_/*QSqlDatabase::database().connectionName()*/ );

    }   else {

            std::cout << stdCon+" connection is not exist in QSql connections list. Nothing to close." << std::endl;
        }
}



void db_connection::remove(const QString &con_name_)
{
    auto stdCon = con_name_.toStdString();
    if(QSqlDatabase::contains(con_name_)){

        QSqlDatabase::removeDatabase( con_name_ );
        std::cout << stdCon+" connection was removed from the QSql connections list." << std::endl;
    } else {
        std::cout << stdCon+" connection is not exist in QSql connections list. May be it was removed from it earlier." << std::endl;
    }
}







bool db_connection::set_query(QString const& query__, QSqlQueryModel &model__, QStringList*stringlist__, QString const & con_name__/*QSqlDatabase const &db__*/) //QListWidget
{
    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query__)){ //MY_SQL_QUERY

        if(qry.exec()){

            model__.setQuery(qry);

            for(int i = 0; i < model__.rowCount(); ++i){

                stringlist__->append(model__.record(i).value(0).toString());
            }

            return true;

        } else {

            std::cout << "(x) QSqlQuery execution failed." << __CH <<std::endl;
        }
    }

    QString const error_msg = qry.lastError().text();
//    QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
    adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    qDebug() << error_msg;

    return false;
}

bool db_connection::set_query(QString const& query__, QSqlQueryModel &model__, QListWidget*listWidget__, QString const & con_name__/*QSqlDatabase const &db__*/) //QListWidget
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

        listWidget__->addItems(query_2_list);

        return true;

        } else {

            std::cout << "(x) QSqlQuery execution failed." << __CH <<std::endl;
        }
    }

    QString const error_msg = qry.lastError().text();
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

            std::cout << "(x) QSqlQuery execution failed." << __CH <<std::endl;
        }

    }

    QString const error_msg = qry.lastError().text();
    adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    qDebug() << error_msg;

    return false;
}


/*static*/bool db_connection::set_query(QString const& query, QSqlQueryModel*model__, QAbstractItemView* itemView, QString const & con_name__/*QSqlDatabase const &db__*/)
{
    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query)){ //MY_SQL_QUERY

        //qDebug() << "(✓) QSqlQuery prepare successful.";
        if(qry.exec()){
            //qDebug() << "(✓) QSqlQuery execution successful.";
        model__->setQuery(qry);

        itemView->setModel(model__);

        return true;

        } else {

            std::cout << "(x) QSqlQuery execution failed." << __CH <<std::endl;
        }

    }

    QString const error_msg = qry.lastError().text();
//    QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
    adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    qDebug() << error_msg;

    return false;
}



/*static*/bool db_connection::set_query(QString const& query, QSqlQueryModel*model__, QTableView *tableView, /*QHeaderView::ResizeMode scalemode, */QString const & con_name__/*QSqlDatabase const &db__*/)
{

    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query)){//; //MY_SQL_QUERY

        //qDebug() << "(✓) QSqlQuery prepare successful.";
        if(qry.exec()){
            //qDebug() << "(✓) QSqlQuery execution successful.";
        model__->setQuery(qry);

        tableView->setModel(model__);

        //tableView->horizontalHeader()->setSectionResizeMode(scalemode); //QHeaderView::Stretch
        //tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        return true;

        } else {

            std::cout << "(x) QSqlQuery execution failed." << __CH <<std::endl;
        }
    }

    QString const error_msg = qry.lastError().text();
//    QMessageBox::warning(0,"Query to DB failed",qry.lastError().text(),QMessageBox::Close);
    ////adb_utility::get_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,true);
    adb_utility::get_separate_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,false,true);
    qDebug() << error_msg;

    return false;
}



/*static*/bool db_connection::set_query(QString const& query, QSqlQueryModel*model__, QTableView *tableView, QPointer<adbMessageBox>& warningWindow__ ,QString const & con_name__/*QSqlDatabase const &db__*/)
{

    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query)){

        if(qry.exec()){

        model__->setQuery(qry);

        tableView->setModel(model__);

        return true;

        } else {

            std::cout << "(x) QSqlQuery execution failed." << __CH <<std::endl;
        }
    }

    QString const error_msg = qry.lastError().text();

    warningWindow__ = adb_utility::get_separate_information_window(QMessageBox::Warning,"Query to DB failed",error_msg,0,false,true);

    qDebug() << error_msg;

    return false;
}



bool db_connection::set_query(QString const& query__, QSqlQueryModel *model__, QComboBox *comboBox, QString const & con_name__/*QSqlDatabase const &db__*/)
{

    QSqlDatabase database = QSqlDatabase::database(con_name__,false);

    QSqlQuery qry = QSqlQuery(database);

    if(qry.prepare(query__)){ //MY_SQL_QUERY

        //qDebug() << "(✓) QSqlQuery prepare successful.";
        if(qry.exec()){
            //qDebug() << "(✓) QSqlQuery execution successful.";

        model__->setQuery(qry);

        comboBox->setModel(model__);

        return true;

        } else {

            std::cout << "(x) QSqlQuery execution failed." << __CH <<std::endl;
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


