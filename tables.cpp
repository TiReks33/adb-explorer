#include "tables.h"
#include "ui_tables.h"

#include "createtupleconstructor.h"

Tables::Tables(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tables)
  , statusBar(new scrolledStatusBar)
  , showTable_button{new BlinkinButton{"Reload tables list"}}

  , auth_(auth__)

  , custom_query_result_window_(new CustomQueryResult{auth_})
  , delete_table_window_(new delete_table)
  , constructor_(new CreateTableConstructor{auth_,this})

{
    ui->setupUi(this);

    ui->verticalLayout->addWidget(statusBar);

    setWindowIcon(QIcon(":/pic/anthead2.png"));

    ui->reloadLayout->addWidget(showTable_button);

    showTable_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);

    statusBar->get_line()->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                        | Qt::WindowMinimizeButtonHint
                                        | Qt::WindowMaximizeButtonHint
                                        | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    ////delete_table_window_->setWindowTitle("Remove table dialog");


    constructor_->setWindowFlag(Qt::Dialog);


    constructor_->setWindowModality(Qt::WindowModal);


    //SIGNALS

    init_connections();

    ui->tableView->setSelectionMode(QTableView::SingleSelection);

    if(!read4rom_query_file())
        write2_query_file();

}

Tables::~Tables()
{
    delete ui;

    delete custom_query_result_window_;
//    delete settings_;
    delete delete_table_window_;
    delete constructor_;

}

void Tables::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to close all windows related to current database and return to DB list?",
                                                             QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No) {
//        qDebug() << "Tables window close:: cancel closing";
        event->ignore();
        return;
      }/* else {
//        qDebug() << "Tables window close:: closing accepted";
      }*/


    emit db_show();
    event->accept();

    emit custom_query_windows_close();

    custom_query_result_window_->close();

 //   db_connection::close(this->metaObject()->className());
  //  db_connection::remove(this->metaObject()->className());
}


void Tables::init_connections()
{
    connect(this,SIGNAL(delete_form_request()),delete_table_window_,SLOT(delete_form_request_slot()));

    connect(delete_table_window_,&delete_table::delete_form_send,[=](QComboBox* comboBox__){
        db_connection::open(auth_);

        db_connection::set_query("SHOW TABLES;",&model_,comboBox__);

        comboBox__->setCurrentIndex(-1); //for blank cell default
    });

    connect(delete_table_window_,&delete_table::delete_entity,[=](QComboBox*comboBox__){
        db_connection::open(auth_);

        QString const chosen_table = comboBox__->currentText();
        QString const query_text = QString("DROP TABLE `%1`").arg(QString(escape_sql_backticks(chosen_table)));

        if(!db_connection::set_query(query_text,&model_,comboBox__))
        {
//            QMessageBox::warning(this,"Warning","Table is not dropped. May be it was been droped earlier?");
            statusBar->get_line()->setText(QString("(x)Table `%1` is not dropped.").arg(chosen_table));
            qDebug() << QString("(x)Table `%1` is not dropped.").arg(chosen_table);
        } else {
                qDebug() << QString("Table `%1` successfully deleted.").arg(chosen_table);
                statusBar->get_line()->setText(QString("(✓)Table `%1` successfully deleted.").arg(chosen_table));
        }
        show_tables();      // view updated tables list after table deletion
        //select_cells(0,0, ui->tableView);
    });

    // transfer tables list to 'create table' constructor
    connect(this,SIGNAL(current_tables_list_signal(QList<QString>)),constructor_,SLOT(current_exist_tables_slot(QList<QString>)));

    //CUSTOM CREATE TABLE CONSTRUCTOR
    connect(constructor_,
            static_cast<void (CreateTableConstructor::*)(QString const&,QString const&)>(&CreateTableConstructor::send_custom_query),
            [=](QString const&query__,QString const&newtable_name__){
        db_connection::open(auth_);

        if(db_connection::set_query(query__,&model_,ui->tableView,QHeaderView::Stretch)) //TABLE MAIN WINDOW MODEL
            emit constructor_query_success(newtable_name__);
        else
            emit constructor_query_fails(newtable_name__);
        show_tables();
    });

    // quit from constructor without warning
    connect(this,&Tables::constructor_query_success, [=] (QString const& newtable_name__) {

        QString const text = QString("(✓)New table `%1` successfully created by `Create Table Constructor`.").arg(newtable_name__);
        qDebug() << text;
        statusBar->get_line()->setText(text);

        constructor_->set_warning_flag_(false);
        constructor_->close();
        constructor_->set_warning_flag_(true);

    });

    // if final query from table constructor fails while exec.
    connect(this,&Tables::constructor_query_fails, this,[=](QString const& newtable_name__){

        constructor_->constructor_query_fails_handle();
        QString const text = QString("(x)`Create table constructor` query fails. Table `%1` is not created.").arg(newtable_name__);
        qDebug() << text;
        statusBar->get_line()->setText(text);

    },Qt::QueuedConnection);


    connect(showTable_button,&QPushButton::clicked,this,&Tables::show_tables);

    // disable some buttons if tables list empty && stop timer for custom blinkin button
    connect(this,&Tables::tables_reloaded,[=]{

        if(!ui->tableView->currentIndex().data().toString().isEmpty()){

            ui->select_from_table_button->setStyleSheet("background: green; color: white;");
            ui->select_from_table_button->setEnabled(true);

            ui->DescribeButton->setStyleSheet("background-color: yellow;");
            ui->DescribeButton->setEnabled(true);

        } else {

            ui->select_from_table_button->setStyleSheet("background: palette(window)");
            ui->select_from_table_button->setEnabled(false);

            ui->DescribeButton->setStyleSheet("background: palette(window)");
            ui->DescribeButton->setEnabled(false);

        }

        showTable_button->stop_blinkin();
    });

    // if separate select, describe and custom query settings disabled, disable some buttons after query
    connect(this,&Tables::disable_select_until_reload,[=]{
        ui->select_from_table_button->setEnabled(false);
        ui->DescribeButton->setEnabled(false);
        showTable_button->start_blinkin(890);
    });

    connect(ui->tableView,&QTableView::clicked,[=](const QModelIndex & index){

        ui->tableView->setCurrentIndex(index);

        statusBar->get_line()->clear();
    });

    connect(ui->tableView,&QTableView::activated,[=](const QModelIndex & index){

        ui->tableView->setCurrentIndex(index);

        statusBar->get_line()->clear();
    });

    connect(ui->tableView,&QTableView::doubleClicked,[=](const QModelIndex & index){

        ui->tableView->setCurrentIndex(index);

        statusBar->get_line()->clear();

        ui->select_from_table_button->click();
    });

    connect(ui->select_from_table_button,&QPushButton::clicked,this,&Tables::show_table_content);


    connect(ui->Custom_Query_Button,&QPushButton::clicked, this, static_cast<void (Tables::*)()>(&Tables::get_custom_query_window_));

    // load settings from file and set new by gui form
    connect(ui->query_settings_button,&QPushButton::clicked,[=]{
        if(!settings_){
            if((settings_ = new CustomQuerySettings)){
                settings_->setAttribute(Qt::WA_DeleteOnClose,true);

                connect(settings_,static_cast<void (CustomQuerySettings::*)(QMap<QString,int>)>(&CustomQuerySettings::settings_changed),[=](QMap<QString,int>settings_map__){
                    int temp;

                    if((temp = settings_map__.value("t_content_wnd"))!=-1)
                        t_content_wnd = temp;//settings_map__.value("t_content_wnd");

                    if((temp = settings_map__.value("t_describe_wnd"))!=-1)
                       t_describe_wnd = temp;//settings_map__.value("t_query_wnd");

                    if((temp = settings_map__.value("t_query_wnd"))!=-1)
                       t_query_wnd = temp;//settings_map__.value("t_query_wnd");

                    if((temp = settings_map__.value("BLANK_RESULT"))!=-1)
                        BLANK_RESULT = temp;

                    if((temp = settings_map__.value("MSG_SHOW_IF_BLANK_RESULT"))!=-1)
                        MSG_SHOW_IF_BLANK_RESULT = temp;//settings_map__.value("MSG_SHOW_IF_BLANK_RESULT");
                });
            }
        }
        if(settings_){
            settings_->show();
            settings_->raise();
        }
    });

    connect(ui->delete_table_button,&QPushButton::clicked,[=]{
        delete_table_window_->setModal(true);
        delete_table_window_->show();
        emit delete_form_request();
    });

    connect(ui->create_table_button,&QPushButton::clicked, this, &Tables::get_table_constructor);

    connect(ui->insert_inTable_button,&QPushButton::clicked,this, &Tables::get_tuple_constructor_instance);


    connect(ui->DescribeButton,&QPushButton::clicked,this,&Tables::/*get_describe_table_instance*/show_table_description);


    connect(this,&Tables::empty_set,[=]{
        if(MSG_SHOW_IF_BLANK_RESULT)
        {
            adb_utility::get_information_window(QMessageBox::Information,"Empty set","(✓) [Note] :: Query result not contain/doesn't imply"
                                   " contain of displayable result.", this);
        }
    });


}

bool Tables::read4rom_query_file()
{
    QMap<QString,int> __settings_map;

    if(get_settings_4rom_file(query_settings_f_name,__settings_map)){
        int temp;

        if((temp = __settings_map.value("t_content_wnd"))!=-1)
            t_content_wnd = temp;

        if((temp = __settings_map.value("t_describe_wnd"))!=-1)
            t_describe_wnd = temp;

        if((temp = __settings_map.value("t_query_wnd"))!=-1)
            t_query_wnd = temp;

        if((temp = __settings_map.value("BLANK_RESULT"))!=-1)
            BLANK_RESULT = temp;

        if((temp = __settings_map.value("MSG_SHOW_IF_BLANK_RESULT"))!=-1)
            MSG_SHOW_IF_BLANK_RESULT = temp;

        return true;
    }

    qWarning() << "Error while read from"<<query_settings_f_name;

    return false;
}

void Tables::write2_query_file()
{
    QFile outFile(query_settings_f_name);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << "t_content_wnd" << '=' << QString::number(t_content_wnd) << Qt::endl;
    textStream << "t_describe_wnd" << '=' << QString::number(t_describe_wnd) << Qt::endl;
    textStream << "t_query_wnd" << '=' << QString::number(t_query_wnd) << Qt::endl;
    textStream << "BLANK_RESULT" << '=' << QString::number(BLANK_RESULT) << Qt::endl;
    textStream << "MSG_SHOW_IF_BLANK_RESULT" << '=' << QString::number(MSG_SHOW_IF_BLANK_RESULT) << Qt::endl;
}


void Tables::show_tables()
{
//QElapsedTimer timer;
//timer.start();

    // ==> close current and open new connect with chosen db
////db_connection::close();

////        QSqlDatabase::database(auth::con_name_,false).setDatabaseName(auth_.db_name_);


    db_connection::try_to_reopen(auth_);


    if(db_connection::set_query("SHOW TABLES;",&model_,ui->tableView,QHeaderView::Stretch)){

        if((ui->tableView->model())!=nullptr){

            select_cells(0,0, ui->tableView);

            //qDebug() << "Number of existing tables::" <<(model_.rowCount());

            //qDebug()<< "ui->tableView->currentIndex().data().toString()::" <<ui->tableView->currentIndex().data().toString();

        }

        emit tables_reloaded();
    }

//qDebug() << "The Set query model operation took" << timer.elapsed() << "milliseconds";
//qDebug() << "The Set query model operation took" << timer.nsecsElapsed() << "nanoseconds";
}



void Tables::send_custom_query_slot(/*QString query__,*/Custom_Query *custom_query_window__)
{

    if(!t_query_wnd){

        if(db_connection::open(auth_)){

            if(db_connection::set_query(/*query__*/custom_query_window__->get_text(),&model_,ui->tableView,QHeaderView::Stretch)){
                custom_query_window__->close_window();

                //qDebug() << "tableViewModelColumnCOUNT::" << ui->tableView->model()->columnCount();
                //qDebug() << "tableViewModelRowsCOUNT::" << ui->tableView->model()->rowCount();

                if(!ui->tableView->model()->rowCount()&&!BLANK_RESULT) {
                    show_tables();
                    //qDebug() << "rowCount() in model_==0::display result ignored.";

                    statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                            " contain of displayable result. Current DB tables list reloaded.");

                    emit empty_set();

                } else{
                    emit disable_select_until_reload();
                    statusBar->get_line()->clear();
                }
            }
        }

    } else{

        CustomQueryResult new_result_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_result_window, &CustomQueryResult::close);

        new_result_window.custom_query_slot(/*auth_,*//*query__*/custom_query_window__->get_text()/*,*/ /*new_result_window->model_,*/ /*new_result_window.ui->tableView*/);

        if((new_result_window.ui->tableView->model())!=nullptr) {

            //qDebug() << "Number of columns in tableView->model()::"<<new_result_window.ui->tableView->model()->columnCount();
           // qDebug() << "Number of rows in tableView->model()::"<<new_result_window.ui->tableView->model()->rowCount();

            custom_query_window__->close_window();

            if(!new_result_window.ui->tableView->model()->rowCount()&&!BLANK_RESULT){

                //qDebug() << "(✓) rowCount() in model_==0::display result ignored.";

                statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                        " contain of displayable result.");

                emit empty_set();

            } else{

                statusBar->get_line()->clear();

                new_result_window.show();
                new_result_window.exec();
            }

        }

    }

}



//void Tables::get_information_window(const QString & header_text__, const QString & main_text__, QWidget *parent__)
//{
//    QPointer <QMessageBox> messageBox{new QMessageBox(QMessageBox::Information,header_text__,
//                                                      main_text__,
//                                                      QMessageBox::Ok,parent__)};

////    connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
//    messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
//    messageBox->setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
//    messageBox->setModal(false);
//    messageBox->show();
//}



void Tables::get_custom_query_window_()
{

        Custom_Query custom_query_window;


            connect(&custom_query_window,static_cast<void(Custom_Query::*)(/*QString,*/Custom_Query *)>(&Custom_Query::send_custom_query),
                    this,static_cast<void(Tables::*)(/*QString,*/Custom_Query *)>(&Tables::send_custom_query_slot));


        connect(this,&Tables::custom_query_windows_close, &custom_query_window , &Custom_Query::close);

        custom_query_window.setModal(false);
        custom_query_window.show();
        custom_query_window.exec();

}

void Tables::get_custom_query_window_(QString const& __pre_query)
{
        Custom_Query custom_query_window;

            connect(&custom_query_window,static_cast<void(Custom_Query::*)(/*QString,*/Custom_Query*)>(&Custom_Query::send_custom_query),
                    this,static_cast<void(Tables::*)(/*QString,*/Custom_Query*)>(&Tables::send_custom_query_slot));

        connect(this,&Tables::custom_query_windows_close, &custom_query_window , &Custom_Query::close);

        custom_query_window.set_text(__pre_query);

        custom_query_window.setModal(false);
        custom_query_window.show();
        custom_query_window.exec();
}

void Tables::show_table_content()
{

    QString const curIndStr = escape_sql_backticks(ui->tableView->currentIndex().data().toString());

    if(!t_content_wnd){

        db_connection::open(auth_);

        //qDebug() << "CurrentIndexString(Table)::" << curIndStr;

        db_connection::set_query(QString("SELECT * FROM ")+/*auth_.table_name_*/curIndStr+(";"),&model_,ui->tableView,QHeaderView::Stretch);

        //qDebug() << "tableViewModelColumnCOUNT::" << ui->tableView->model()->columnCount();
        //qDebug() << "tableViewModelRowsCOUNT::" << ui->tableView->model()->rowCount();

        if(!ui->tableView->model()->rowCount()&&!BLANK_RESULT) {
            show_tables();
            //qDebug() << "rowCount() in model_==0::display result ignored.";

            statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                    " contain of displayable result. Current DB tables list reloaded.");

            emit empty_set();

        } else{

            emit disable_select_until_reload();
            statusBar->get_line()->clear();
        }



    }else{

        CustomQueryResult new_select_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_select_window, &Custom_Query::close);

        new_select_window.setWindowTitle(/*auth_.table_name_*/curIndStr+": table data");

        new_select_window.custom_query_slot(QString("SELECT * FROM ")+/*auth_.table_name_*/curIndStr+(";"));

        if((new_select_window.ui->tableView->model())!=nullptr) {

            //qDebug() << "Number of columns in tableView->model()::"<<new_select_window.ui->tableView->model()->columnCount();
            //qDebug() << "Number of rows in tableView->model()::"<<new_select_window.ui->tableView->model()->rowCount();


            if(!new_select_window.ui->tableView->model()->rowCount()&&!BLANK_RESULT){

               // qDebug() << "(✓) rowCount() in model_==0::display result ignored.";

                statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                        " contain of displayable result.");

                emit empty_set();

            } else{

            statusBar->get_line()->clear();

            new_select_window.show();
            new_select_window.exec();

            }

        }

    }

}



void Tables::get_table_constructor()
{
    if(!constructor_->isVisible()){

        constructor_->setCurrentIndex(0);

        // UNIQUE NAME CHECK for constructor [
        QList<QString> list_to_send;

        size_t size_of_list_=ui->tableView->model()->rowCount();

        //qDebug() << "NUMBER OF TABLES::" << size_of_list_;

        for(size_t i=0;i!=size_of_list_;++i)
            list_to_send.append(ui->tableView->model()->index(i,0).data().toString());

        //qDebug()<< "CURRENT LIST OF STRINGs::"<<list_to_send;

        emit current_tables_list_signal(list_to_send);

        // ]

        //CONSTRUCTOR WINDOW AT THE CENTER OF TABLE WINDOW
        window_center_from_another_(this,constructor_);

        constructor_->setWindowFlag(Qt::Dialog);

        constructor_->show();

    } else {

        constructor_->raise();

    }
}



void Tables::get_tuple_constructor_instance()
{
    if(!ui->tableView->model()->rowCount())
    {

        adb_utility::get_information_window(QMessageBox::Information,"Current database is empty","Current database is empty. You must create at least 1 table to insert data,"
                                                            " or choose another DB.",this);
        return;
    }
        QPointer<createTupleConstructor> constr_window_{new createTupleConstructor{auth_/*,this*/}};
        ++tuples_windows_counter_;
        //qDebug() << "tuples constructor counter incremented; counter =="+QString::number(tuples_windows_counter_);

        connect(constr_window_, static_cast<void (createTupleConstructor::*) (QString const &)>(&createTupleConstructor::final_query_sig),
                this, static_cast<void (Tables::*) (QString const &)>(&Tables::get_custom_query_window_));
        connect(this,&Tables::custom_query_windows_close, constr_window_, &createTupleConstructor::close);

        connect(constr_window_,static_cast<void (createTupleConstructor::*) (QString const &)>(&createTupleConstructor::closed),[=](QString const & con_name_/*that_mustBclosed*/){

                            if(this->tuples_windows_counter_>0){
                                --this->tuples_windows_counter_;
                                //qDebug() << "tuples counter decremented; counter =="+QString::number(tuples_windows_counter_);
                            }
                            if(this->tuples_windows_counter_==0){
                                db_connection::close(con_name_);
                                        //qDebug() << "tuples counter ==0 -> connection ::"+con_name_+":: closed";
                            }
                    ;});

        constr_window_->setAttribute( Qt::WA_DeleteOnClose, true );

        constr_window_->show();

}



void Tables::/*get_describe_table_instance*/show_table_description()
{
    QString const curIndStr = escape_sql_backticks(ui->tableView->currentIndex().data().toString());
    QString query_text = "DESC "+curIndStr+';';

    if(!t_describe_wnd){

        db_connection::open(auth_);

        db_connection::set_query(query_text,&model_,ui->tableView,QHeaderView::Stretch);

        //qDebug() << "tableViewModelColumnCOUNT::" << ui->tableView->model()->columnCount();
        //qDebug() << "tableViewModelRowsCOUNT::" << ui->tableView->model()->rowCount();

        if(!ui->tableView->model()->rowCount() && !BLANK_RESULT) {
            show_tables();
            //qDebug() << "rowCount() in model_==0::display result ignored.";

            statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                    " contain of displayable result. Current DB tables list reloaded.");

            emit empty_set();

        } else{

            emit disable_select_until_reload();
            statusBar->get_line()->clear();

        }

    }else{

        CustomQueryResult new_select_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_select_window, &Custom_Query::close);

        new_select_window.setWindowTitle(/*auth_.table_name_*/curIndStr+": detail table info");

        new_select_window.custom_query_slot(query_text);


        if((new_select_window.ui->tableView->model())!=nullptr) {

            //qDebug() << "Number of columns in tableView->model()::"<<new_select_window.ui->tableView->model()->columnCount();
            //qDebug() << "Number of rows in tableView->model()::"<<new_select_window.ui->tableView->model()->rowCount();


            if(!new_select_window.ui->tableView->model()->rowCount()&&!BLANK_RESULT){

                //qDebug() << "(✓) rowCount() in model_==0::display result ignored.";

                statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                        " contain of displayable result.");

                emit empty_set();

            } else{

                statusBar->get_line()->clear();

                new_select_window.show();
                new_select_window.exec();
            }

        }
    }

}


