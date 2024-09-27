#include "tables.h"
#include "ui_tables.h"

#include "createtupleconstructor.h"

Tables::Tables(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tables)

  , showTable_button{new BlinkinButton{"Reload tables list"}}

  , auth_(auth__)

  , custom_query_result_window_(new CustomQueryResult{auth_})
  , settings_(new CustomQuerySettings)
  , delete_table_window_(new delete_table)
  , constructor_(new CreateTableConstructor{auth_,this})

{
    ui->setupUi(this);

    ui->reloadLayout->addWidget(showTable_button);

    showTable_button->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Preferred);

    ui->statusLine->setReadOnly(true);

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

}

Tables::~Tables()
{
    delete ui;

    delete custom_query_result_window_;
    delete settings_;
    delete delete_table_window_;
    delete constructor_;

}

void Tables::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to close all windows related to current database and return to DB list?",
                                                             QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No) {
        qDebug() << "Tables window close:: cancel closing";
        event->ignore();
        return;
      } else {
        qDebug() << "Tables window close:: closing accepted";
      }


    emit db_show();
    event->accept();

    emit custom_query_windows_close();

    custom_query_result_window_->close();

 //   db_connection::close(this->metaObject()->className());
  //  db_connection::remove(this->metaObject()->className());
}

//bool Tables::event(QEvent* event)
//{
//  QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

//  // if ESC key pressed-->
//  if(keyEvent && keyEvent->key() == Qt::Key_Escape)
//  {
//      this->hide();
//      emit db_show();
//    keyEvent->accept();
//    return true;
//  }
//  // otherwise call non-override parent-->
//  return QDialog::event(event);
//}


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
            ui->statusLine->setText(QString("(x)Table `%1` is not dropped.").arg(chosen_table));
            qDebug() << QString("(x)Table `%1` is not dropped.").arg(chosen_table);
        } else {
                qDebug() << QString("Table `%1` successfully deleted.").arg(chosen_table);
                ui->statusLine->setText(QString("(✓)Table `%1` successfully deleted.").arg(chosen_table));
        }
        show_tables();      // view updated tables list after table deletion
        //select_cells(0,0, ui->tableView);
    });

    //QStringList?-->
    connect(this,SIGNAL(current_tables_list_signal(QList<QString>)),constructor_,SLOT(current_exist_tables_slot(QList<QString>)));

    //CUSTOM CREATE TABLE CONSTRUCTOR
    connect(constructor_,&CreateTableConstructor::send_custom_query,[=](QString const&query__){
        db_connection::open(auth_);

        if(db_connection::set_query(query__,&model_,ui->tableView,QHeaderView::Stretch)) //TABLE MAIN WINDOW MODEL
            emit constructor_query_success();
        else
            emit constructor_query_fails();
        show_tables();
    });


    connect(this,&Tables::constructor_query_success, [=] () {
        constructor_->set_warning_flag_(false);
        constructor_->close();
        constructor_->set_warning_flag_(true);
        qDebug() << "(✓)Table constructor query successful completed.";
    });

//    connect(this,&Tables::constructor_query_fails, constructor_, &CreateTableConstructor::constructor_query_fails_handle,Qt::QueuedConnection);

//    connect(this,&Tables::constructor_query_fails, [=] () {
//        qDebug() << "(x)Table constructor query fails.";
//    });

    connect(this,&Tables::constructor_query_fails, this,[=]{
        constructor_->constructor_query_fails_handle();
        qDebug() << "(x)Table constructor query fails.";
    },Qt::QueuedConnection);

    connect(/*ui->*/showTable_button,&QPushButton::clicked,[=]{
        show_tables();
//        ui->selectButtonsLayout->setEnabled(true);
    });

    connect(this,&Tables::tables_reloaded,[=]{
//        ui->selectButtonsLayout->setEnabled(true);
        ui->select_from_table_button->setEnabled(true);
        ui->DescribeButton->setEnabled(true);
        showTable_button->stop_blinkin();
    });

    connect(this,&Tables::disable_select_until_reload,[=]{
        ui->select_from_table_button->setEnabled(false);
        ui->DescribeButton->setEnabled(false);
        showTable_button->start_blinkin(890);
    });

    connect(ui->tableView,&QTableView::clicked,[=](const QModelIndex & index){
        ////SETUP CURRENT INDEX POSITION (first 'a' index)
        ui->tableView->setCurrentIndex(index);
//??
    ////    auth_.table_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();
//??
        qDebug() << "CLICKED::::::::::::" << ui->tableView->currentIndex() << "::" /*<< auth_.table_name_*/;
    });

    connect(ui->select_from_table_button,&QPushButton::clicked,this,&Tables::show_table_content);
//    connect(ui->select_from_table_button,&QPushButton::clicked,[=]{
//        show_table_content();
//        if(!settings_->ui->custom_checkbox->isChecked())
//            ui->selectButtonsLayout->setEnabled(false);
//    });


    connect(ui->Custom_Query_Button,&QPushButton::clicked, this, static_cast<void (Tables::*)()>(&Tables::get_custom_query_window_));


    connect(ui->Query_settings,&QPushButton::clicked,[=]{
        settings_->show();
    });

    connect(ui->delete_table_button,&QPushButton::clicked,[=]{
        delete_table_window_->setModal(true);
        delete_table_window_->show();
        emit delete_form_request();
    });

    connect(ui->create_table_button,&QPushButton::clicked, this, &Tables::get_table_constructor);

    connect(ui->insert_inTable_button,&QPushButton::clicked,this, &Tables::get_tuple_constructor_instance);


    connect(ui->DescribeButton,&QPushButton::clicked,this,&Tables::/*get_describe_table_instance*/show_table_description);



}

//void Tables::disable_select_until_reload()
//{
//    ui->select_from_table_button->setEnabled(false);
//    ui->DescribeButton->setEnabled(false);
//}


void Tables::show_tables()
{
//QElapsedTimer timer;
//timer.start();

    // ==> close current and open new connect with chosen db
////db_connection::close();

////        QSqlDatabase::database(auth::con_name_,false).setDatabaseName(auth_.db_name_);



qDebug() << "AFTER CLOSE()::";

        db_connection::try_to_reopen(auth_);




    if(db_connection::set_query("SHOW TABLES;",&model_,ui->tableView,QHeaderView::Stretch)){

        if((ui->tableView->model())!=nullptr){
            select_cells(0,0, ui->tableView);


//            auth_.table_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();
        }

        qDebug() << "Number of existinf tables::" <<(model_.rowCount());

qDebug()<< ui->tableView->currentIndex().data().toString();

        if(ui->tableView->currentIndex().data().toString().isEmpty()){
//        if(!auth_.table_name_.isNull()){
            ui->select_from_table_button->setEnabled(true);
            ui->select_from_table_button->setStyleSheet("background: green; color: white;");
        }

        emit tables_reloaded();

    }

//qDebug() << "The Set query model operation took" << timer.elapsed() << "milliseconds";
//qDebug() << "The Set query model operation took" << timer.nsecsElapsed() << "nanoseconds";
}



//void Tables::send_custom_query_slot(QString const& query__)
//{

//    if(!settings_->ui->custom_checkbox->isChecked()){

//        db_connection::open(auth_);

//        if(db_connection::set_query(query__,&model_,ui->tableView,QHeaderView::Stretch))
//            emit close_custom_query_form(); // don't need anymore?
//qDebug() << "tableViewModelColumnCOUNT::" << ui->tableView->model()->columnCount();
//        if(!ui->tableView->model()->columnCount()) {
//            show_tables();
//            qDebug() << "rowCount() in model_==0::display result ignored.";

//            ui->statusLine->setText("(✓) [Note] :: Query result not contain/doesn't imply"
//                    " contain of displayable result. Current DB tables list reloaded.");

//        } else{
////            ui->selectButtonsLayout->setEnabled(false);
////            qDebug() << "send_custom_query_slot::disabled_layout";
//            emit disable_select_until_reload();
//        }

//    } else{

//        CustomQueryResult new_result_window{auth_};

//        new_result_window.custom_query_slot(query__);

//        if((new_result_window.ui->tableView->model())!=nullptr) {

//            qDebug() << "Number of columns in tableView->model()::"<<new_result_window.ui->tableView->model()->columnCount();

//            emit close_custom_query_form(); // don't need anymore?

//            if(new_result_window.ui->tableView->model()->columnCount()>0){
//                new_result_window.show();
//                new_result_window.exec();
//            }

//        }

//        qDebug() << "NUMBER OF CORTEGES::"<< ((new_result_window.ui->tableView->model())==nullptr);

//    }

//}


void Tables::send_custom_query_slot(/*QString query__,*/Custom_Query *custom_query_window__)
{


    if(!settings_->ui->custom_checkbox->isChecked()){
        db_connection::open(auth_);
        qDebug()<< "SHASHLIK";

        if(db_connection::set_query(/*query__*/custom_query_window__->get_text(),&model_,ui->tableView,QHeaderView::Stretch))
            custom_query_window__->close_window();

        qDebug() << "tableViewModelColumnCOUNT::" << ui->tableView->model()->columnCount();
        qDebug() << "tableViewModelRowsCOUNT::" << ui->tableView->model()->rowCount();

        if(!ui->tableView->model()->rowCount()) {
            show_tables();
            qDebug() << "rowCount() in model_==0::display result ignored.";

            ui->statusLine->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                    " contain of displayable result. Current DB tables list reloaded.");

        } else{
//            ui->selectButtonsLayout->setEnabled(false);
//            qDebug() << "send_custom_query_slot::disabled_layout";
            emit disable_select_until_reload();
            ui->statusLine->clear();
        }

    } else{

        CustomQueryResult new_result_window{auth_};

        new_result_window.custom_query_slot(/*auth_,*//*query__*/custom_query_window__->get_text()/*,*/ /*new_result_window->model_,*/ /*new_result_window.ui->tableView*/);

        if((new_result_window.ui->tableView->model())!=nullptr) {

            qDebug() << "Number of columns in tableView->model()::"<<new_result_window.ui->tableView->model()->columnCount();
            qDebug() << "Number of rows in tableView->model()::"<<new_result_window.ui->tableView->model()->rowCount();

            custom_query_window__->close_window();

            if(!new_result_window.ui->tableView->model()->rowCount()){

                qDebug() << "(✓) rowCount() in model_==0::display result ignored.";

                ui->statusLine->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                        " contain of displayable result.");

            } else{

                ui->statusLine->clear();

                new_result_window.show();
                new_result_window.exec();
            }

        }

    }

}



void Tables::show_table_describe_form(QString const & db_name__,QString const&table_name__,QString const & con_name__,QWidget * parent__, Qt::WindowType window_type_flag__,Qt::WindowModality window_modality_flag__) const
{
    db_connection::close(con_name__);
    //db_connection::remove(con_name__);
    auth __auth = auth_;
    __auth.db_name_=db_name__;//ui->ref_DB_comboBox_2->currentText();
//qDebug() << "COUNTER BEFORE::" << window_counter_;
//    ++window_counter_;

    QSqlDatabase::database(con_name__,false).setDatabaseName(__auth.db_name_);

    CustomQueryResult new_select_window{__auth};

    //__auth.db_name_=ui->ref_DB_comboBox_2->currentText()+QString::number(window_counter_);
    ////QString _con_name=ui->ref_DB_comboBox_2->currentText()+QString::number(window_counter_);

    new_select_window.setWindowTitle(/*ui->ref_table_comboBox_2->currentText()*/table_name__);

//custom_query_result_window_->show();
new_select_window.custom_query_slot(/*__auth,*/QString(/*"USE "+ui->ref_DB_comboBox_2->currentText()+';'+' '+*/"DESCRIBE ")+
                                    /*ui->ref_table_comboBox_2->currentText()*/table_name__+(";")/*,new_select_window->ui->tableView*/,
                                    /*new_select_window->model_,*//*new_select_window.ui->tableView,*/ /*_con_name*/con_name__/*__auth.db_name_*/);
//if(model_.rowCount()!=0)


new_select_window.setParent(parent__);
new_select_window.setWindowFlag(window_type_flag__);
new_select_window.setWindowModality(window_modality_flag__);

new_select_window.show();

new_select_window.exec();
qDebug()<<"AFTER EXECUTION()";
                                                                    ////db_connection::close(con_name__);
////db_connection::close/*_con*/(/*_con_name*/con_name__/*__auth.db_name_*/);
//--window_counter_;
//qDebug() << "COUNTER AFTER::" << window_counter_;
}






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

    qDebug()<<"SHOW TABLE [START]";

    QString const curIndStr = escape_sql_backticks(ui->tableView->currentIndex().data().toString());

    if(!settings_->ui->select_checkbox->isChecked()){

        db_connection::open(auth_);

        qDebug() << "CurrentIndexString(Table)::" << curIndStr;

        db_connection::set_query(QString("SELECT * FROM ")+/*auth_.table_name_*/curIndStr+(";"),&model_,ui->tableView,QHeaderView::Stretch);



        qDebug() << "tableViewModelColumnCOUNT::" << ui->tableView->model()->columnCount();
        qDebug() << "tableViewModelRowsCOUNT::" << ui->tableView->model()->rowCount();

        if(!ui->tableView->model()->rowCount()) {
            show_tables();
            qDebug() << "rowCount() in model_==0::display result ignored.";

            ui->statusLine->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                    " contain of displayable result. Current DB tables list reloaded.");

        } else{

            emit disable_select_until_reload();
            ui->statusLine->clear();
        }



    }else{

        CustomQueryResult new_select_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_select_window, &Custom_Query::close);

        new_select_window.setWindowTitle(/*auth_.table_name_*/curIndStr+": table data");

        new_select_window.custom_query_slot(QString("SELECT * FROM ")+/*auth_.table_name_*/curIndStr+(";"));

        if((new_select_window.ui->tableView->model())!=nullptr) {

            qDebug() << "Number of columns in tableView->model()::"<<new_select_window.ui->tableView->model()->columnCount();
            qDebug() << "Number of rows in tableView->model()::"<<new_select_window.ui->tableView->model()->rowCount();


            if(!new_select_window.ui->tableView->model()->rowCount()){

                qDebug() << "(✓) rowCount() in model_==0::display result ignored.";

                ui->statusLine->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                        " contain of displayable result.");
            } else{

            ui->statusLine->clear();

            new_select_window.show();
            new_select_window.exec();

            }

        }

    }
    qDebug()<<"SHOW TABLE [END]";
}



void Tables::get_table_constructor()
{
    if(!constructor_->isVisible()){

    constructor_->setCurrentIndex(0);

    QList<QString> list_to_send; // UNIQUE NAME CHECK

    size_t size_of_list_=ui->tableView->model()->rowCount();

    qDebug() << "NUMBER OF TABLES::" << size_of_list_;

    //qDebug() << "CURRENT TEXT::"<<ui->tableView->model()->index(1,0).data().toString();
    for(size_t i=0;i!=size_of_list_;++i)
        list_to_send.append(ui->tableView->model()->index(i,0).data().toString());

    qDebug()<< "CURRENT LIST OF STRINGs::"<<list_to_send;

    emit current_tables_list_signal(list_to_send);



    //CONSTRUCTOR WINDOW AT THE CENTER OF TABLE WINDOW
    window_center_from_another_(this,constructor_);

    ////constructor_->setParent(this);

    //    constructor_->setWindowFlag(Qt::Window);
        constructor_->setWindowFlag(Qt::Dialog);


    ////    constructor_->setWindowModality(Qt::WindowModal);
    //    constructor_->setWindowModality(Qt::NonModal);

    constructor_->show();

    //this->hide();

    } else {

        constructor_->raise();

    }
}

void Tables::get_tuple_constructor_instance()
{
    if(!ui->tableView->model()->rowCount())
    {
        QPointer <QMessageBox> messageBox{new QMessageBox(QMessageBox::Information,"Current database is empty",
                                                          "Current database is empty. You must create at least 1 table to insert data,"
                                                            " or choose another DB.",
                                                          QMessageBox::Ok,this/*0*/)};

        connect(messageBox,&QMessageBox::destroyed,[&](){ qDebug() << "~messageBox activated (destroyed).";});
        messageBox->setAttribute( Qt::WA_DeleteOnClose, true );
        //messageBox->setModal(false);
        messageBox->show();
        return;
    }
        QPointer<createTupleConstructor> constr_window_{new createTupleConstructor{auth_/*,this*/}};
        ++tuples_windows_counter_;
        qDebug() << "tuples constructor counter incremented; counter =="+QString::number(tuples_windows_counter_);

        connect(constr_window_, static_cast<void (createTupleConstructor::*) (QString const &)>(&createTupleConstructor::final_query_sig),
                this, static_cast<void (Tables::*) (QString const &)>(&Tables::get_custom_query_window_));
        connect(this,&Tables::custom_query_windows_close, constr_window_, &createTupleConstructor::close);

        connect(constr_window_,static_cast<void (createTupleConstructor::*) (QString const &)>(&createTupleConstructor::closed),[=](QString const & con_name_/*that_mustBclosed*/){

                            if(this->tuples_windows_counter_>0){
                                --this->tuples_windows_counter_;
                                qDebug() << "tuples counter decremented; counter =="+QString::number(tuples_windows_counter_);
                            }
                            if(this->tuples_windows_counter_==0){
                                db_connection::close(con_name_);
                                        qDebug() << "tuples counter ==0 -> connection ::"+con_name_+":: closed";
                            }
                    ;});

        constr_window_->setAttribute( Qt::WA_DeleteOnClose, true );

        //constr_window_->setWindowFlag(Qt::Window);
        //constr_window_->setModal(false);

        //constr_window_->setWindowModality(Qt::NonModal);
        constr_window_->show();
        //constr_window_->setVisible(true);
//        constr_window_.exec();
        qDebug()<<"TupleConstructor window after exec() before out of scope";
}



void Tables::/*get_describe_table_instance*/show_table_description()
{
    qDebug() << "Table describe[BEGIN]";

    QString const curIndStr = escape_sql_backticks(ui->tableView->currentIndex().data().toString());
    QString query_text = "DESC "+/*auth_.table_name_*/curIndStr+';';

    if(!settings_->ui->select_checkbox->isChecked()){

        db_connection::open(auth_);

        emit disable_select_until_reload();

        db_connection::set_query(query_text,&model_,ui->tableView,QHeaderView::Stretch);

        qDebug() << "tableViewModelColumnCOUNT::" << ui->tableView->model()->columnCount();
        qDebug() << "tableViewModelRowsCOUNT::" << ui->tableView->model()->rowCount();

        if(!ui->tableView->model()->rowCount()) {
            show_tables();
            qDebug() << "rowCount() in model_==0::display result ignored.";

            ui->statusLine->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                    " contain of displayable result. Current DB tables list reloaded.");

        } else{

            emit disable_select_until_reload();
            ui->statusLine->clear();

        }

    }else{

        CustomQueryResult new_select_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_select_window, &Custom_Query::close);

        new_select_window.setWindowTitle(/*auth_.table_name_*/curIndStr+": detail table info");

        new_select_window.custom_query_slot(query_text);


        if((new_select_window.ui->tableView->model())!=nullptr) {

            qDebug() << "Number of columns in tableView->model()::"<<new_select_window.ui->tableView->model()->columnCount();
            qDebug() << "Number of rows in tableView->model()::"<<new_select_window.ui->tableView->model()->rowCount();


            if(!new_select_window.ui->tableView->model()->rowCount()){

                qDebug() << "(✓) rowCount() in model_==0::display result ignored.";

                ui->statusLine->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                        " contain of displayable result.");

            } else{

                ui->statusLine->clear();

                new_select_window.show();
                new_select_window.exec();
            }

        }
    }

    qDebug() << "Table describe[END]";

}


