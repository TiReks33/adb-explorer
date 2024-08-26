#include <tables.h>
#include "ui_tables.h"

#include <QCheckBox>
#include <QLabel>
//#include <QStackedLayout>
#include <QGraphicsEffect>
#include <QElapsedTimer>

#include <customqueryresult.h>
#include <ui_customqueryresult.h>

#include "createtupleconstructor.h"

Tables::Tables(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tables)

  , auth_(auth__)


  , custom_query_result_window_(new CustomQueryResult{auth_})
  , settings_(new CustomQuerySettings)
  , delete_table_window_(new delete_table)
  , constructor_(new CreateTableConstructor{auth_,this})

{
    ui->setupUi(this);

    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                        | Qt::WindowMinimizeButtonHint
                                        | Qt::WindowMaximizeButtonHint
                                        | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    delete_table_window_->setWindowTitle("Remove table dialog");


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
////    table_query_window_->close();
    emit custom_query_windows_close();

    custom_query_result_window_->close();

    db_connection::close(this->metaObject()->className());
    db_connection::remove(this->metaObject()->className());
}

bool Tables::event(QEvent* event)
{
  QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

  // if ESC key pressed-->
  if(keyEvent && keyEvent->key() == Qt::Key_Escape)
  {
      this->hide();
      emit db_show();
    keyEvent->accept();
    return true;
  }
  // otherwise call non-override parent-->
  return QDialog::event(event);
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

        if(!db_connection::set_query("DROP TABLE "+comboBox__->currentText()+";",&model_,comboBox__)){

            QMessageBox::warning(this,"Warning","Table is not droped. May be it was been droped earlier?");

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

    connect(this,&Tables::constructor_query_fails, constructor_, &CreateTableConstructor::constructor_query_fails_handle,Qt::QueuedConnection);

    connect(this,&Tables::constructor_query_fails, [=] () {
        qDebug() << "(x)Table constructor query fails.";
    });

    connect(ui->showTable_button,&QPushButton::clicked,[=]{
        show_tables();
    });

}


void Tables::show_tables()
{
//QElapsedTimer timer;
//timer.start();

    // ==> close current and open new connect with chosen db
db_connection::close();

        QSqlDatabase::database(auth::con_name_,false).setDatabaseName(auth_.db_name_);



qDebug() << "AFTER CLOSE()::";

        db_connection::try_to_reopen(auth_);




    if(db_connection::set_query("SHOW TABLES;",&model_,ui->tableView,QHeaderView::Stretch)){

        if((ui->tableView->model())!=nullptr){
            select_cells(0,0, ui->tableView);


            auth_.table_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();
        }

        qDebug() << "Number of existinf DBs::" <<(model_.rowCount());


        if(!auth_.table_name_.isNull()){
            ui->select_from_table_button->setEnabled(true);
            ui->select_from_table_button->setStyleSheet("background: green; color: white;");
        }
    }

//qDebug() << "The Set query model operation took" << timer.elapsed() << "milliseconds";
//qDebug() << "The Set query model operation took" << timer.nsecsElapsed() << "nanoseconds";
}

void Tables::send_custom_query_slot(QString query__)
{
//    //qDebug()<<"test";
//    db_connection::open(auth_);


//    db_connection::set_query(query__,model_,ui->tableView,QHeaderView::Stretch);

//                                                                custom_query_result_window_->show();
//                                                                emit custom_query(query__);

    if(!settings_->ui->custom_checkbox->isChecked()){
    db_connection::open(auth_);


    if(db_connection::set_query(query__,&model_,ui->tableView,QHeaderView::Stretch))
        emit close_custom_query_form(); // don't need anymore?
    if(!ui->tableView->model()->columnCount()) {
        show_tables();
        qDebug() << "rowCount() in model_==0::display result ignored.";

    }
    }else{

        CustomQueryResult new_result_window{auth_};
        //new_result_window.show();
//    custom_query_result_window_->show();
    new_result_window.custom_query_slot(/*auth_,*/query__/*,*/ /*new_result_window->model_,*/ /*new_result_window.ui->tableView*/);
    //if(new_result_window.ui->tableView->model()->rowCount()!=0)
    if((new_result_window.ui->tableView->model())!=nullptr) {

        qDebug() << "Number of columns in tableView->model()::"<<new_result_window.ui->tableView->model()->columnCount();
        emit close_custom_query_form(); // don't need anymore?
        if(new_result_window.ui->tableView->model()->columnCount()>0){
            new_result_window.show();
            new_result_window.exec();
        }

    }
    qDebug() << "NUMBER OF CORTEGES::"<< ((new_result_window.ui->tableView->model())==nullptr);
    }
}


void Tables::send_custom_query_slot(/*QString query__,*/Custom_Query*custom_query_window__)
{
//    //qDebug()<<"test";
//    db_connection::open(auth_);


//    db_connection::set_query(query__,model_,ui->tableView,QHeaderView::Stretch);

//                                                                custom_query_result_window_->show();
//                                                                emit custom_query(query__);

    if(!settings_->ui->custom_checkbox->isChecked()){
    db_connection::open(auth_);


    if(db_connection::set_query(/*query__*/custom_query_window__->get_text(),&model_,ui->tableView,QHeaderView::Stretch))
        ////emit close_custom_query_form();
        custom_query_window__->close_window();
    if(!ui->tableView->model()->columnCount()) {
        show_tables();
        qDebug() << "rowCount() in model_==0::display result ignored.";

    }
    }else{

        CustomQueryResult new_result_window{auth_};
        //new_result_window.show();
//    custom_query_result_window_->show();
    new_result_window.custom_query_slot(/*auth_,*//*query__*/custom_query_window__->get_text()/*,*/ /*new_result_window->model_,*/ /*new_result_window.ui->tableView*/);
    //if(new_result_window.ui->tableView->model()->rowCount()!=0)
    if((new_result_window.ui->tableView->model())!=nullptr) {

        qDebug() << "Number of columns in tableView->model()::"<<new_result_window.ui->tableView->model()->columnCount();
        ////emit close_custom_query_form();
        custom_query_window__->close_window();
        if(new_result_window.ui->tableView->model()->columnCount()>0){
            new_result_window.show();
            new_result_window.exec();
        }

    }
    qDebug() << "NUMBER OF CORTEGES::"<< ((new_result_window.ui->tableView->model())==nullptr);
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



void Tables::on_showTable_button_clicked()
{
    show_tables();
}

void Tables::on_tableView_clicked(const QModelIndex &index)
{
    ////SETUP CURRENT INDEX POSITION (first 'a' index)
    ui->tableView->setCurrentIndex(index);

    auth_.table_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();

    qDebug() << "CLICKED::::::::::::" << ui->tableView->currentIndex() << "::" << auth_.table_name_;
}

void Tables::on_select_from_table_button_clicked()
{
//    if(!ui->checkBox->isChecked()){
    if(!settings_->ui->select_checkbox->isChecked()){
    db_connection::open(auth_);


    db_connection::set_query(QString("SELECT * FROM ")+auth_.table_name_+(";"),&model_,ui->tableView,QHeaderView::Stretch);
    }else{
//        CustomQueryResult* new_select_window = new CustomQueryResult{auth_};
        CustomQueryResult new_select_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_select_window, &Custom_Query::close);

        new_select_window.setWindowTitle(auth_.table_name_+": table data");
        new_select_window.show();
    //custom_query_result_window_->show();
    new_select_window.custom_query_slot(/*auth_,*/QString("SELECT * FROM ")+auth_.table_name_+(";")/*,new_select_window->ui->tableView*//*,*/ /*new_select_window->model_,*//*new_select_window.ui->tableView*/);
    //if(model_.rowCount()!=0)
    qDebug()<<"BEFORE EXECUTION ::";
    new_select_window.exec();
    }
}

void Tables::get_custom_query_window_()
{
    //    table_query_window_->setModal(true);
    ////    table_query_window_->show();
        Custom_Query custom_query_window;

    ////    connect(&custom_query_window,SIGNAL(send_custom_query(QString)),this,SLOT(send_custom_query_slot(QString)));
            connect(&custom_query_window,static_cast<void(Custom_Query::*)(/*QString,*/Custom_Query*)>(&Custom_Query::send_custom_query),
                    this,static_cast<void(Tables::*)(/*QString,*/Custom_Query*)>(&Tables::send_custom_query_slot));
    ////    connect(this,&Tables::close_custom_query_form,[&](){ custom_query_window.close_window();});

        connect(this,&Tables::custom_query_windows_close, &custom_query_window , &Custom_Query::close);

        custom_query_window.setModal(false);
        custom_query_window.show();
        custom_query_window.exec();
        //emit show_tables_signal();
}

void Tables::get_custom_query_window_(QString __pre_query)
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

void Tables::on_Custom_Query_Button_clicked()
{
////    table_query_window_->setModal(true);
//////    table_query_window_->show();
//    Custom_Query custom_query_window;

//////    connect(&custom_query_window,SIGNAL(send_custom_query(QString)),this,SLOT(send_custom_query_slot(QString)));
//        connect(&custom_query_window,static_cast<void(Custom_Query::*)(QString,Custom_Query*)>(&Custom_Query::send_custom_query),
//                this,static_cast<void(Tables::*)(QString,Custom_Query*)>(&Tables::send_custom_query_slot));
//////    connect(this,&Tables::close_custom_query_form,[&](){ custom_query_window.close_window();});

//    connect(this,&Tables::custom_query_windows_close, &custom_query_window , &Custom_Query::close);

//    custom_query_window.setModal(false);
//    custom_query_window.show();
//    custom_query_window.exec();
//    //emit show_tables_signal();
    get_custom_query_window_();
}



void Tables::on_Query_settings_clicked()
{
    settings_->show();
}

void Tables::on_delete_table_button_clicked()
{
    delete_table_window_->setModal(true);
    delete_table_window_->show();
    emit delete_form_request();
}

void Tables::on_create_table_button_clicked()
{

    if(!constructor_->isVisible()){

    //auth_.backup_db_name();

    constructor_->setCurrentIndex(0);

    QList<QString> list_to_send; // UNIQUE NAME CHECK

    size_t size_of_list_=ui->tableView->model()->rowCount();

    qDebug() << "NUMBER OF TABLES::" << size_of_list_;

    //qDebug() << "CURRENT TEXT::"<<ui->tableView->model()->index(1,0).data().toString();
    for(size_t i=0;i!=size_of_list_;++i)
        list_to_send.append(ui->tableView->model()->index(i,0).data().toString());

    qDebug()<< "CURRENT LIST OF STRINGs::"<<list_to_send;

    emit current_tables_list_signal(list_to_send);

    //emit default_db()

//CONSTRUCTOR WINDOW AT THE CENTER OF TABLE WINDOW
    //constructor_->move(this->geometry().center());

//    QPoint centerPoint = this->geometry().center();

//    constructor_->adjustSize();
//    constructor_->move(centerPoint.x() - constructor_->width()/2, centerPoint.y() - constructor_->height()/2);
    window_center_from_another_(this,constructor_);

    ////constructor_->setParent(this);

//    constructor_->setWindowFlag(Qt::Window);
////    constructor_->setWindowFlag(Qt::Dialog);


////    constructor_->setWindowModality(Qt::WindowModal);
//    constructor_->setWindowModality(Qt::NonModal);
    constructor_->show();

    } else {

        constructor_->raise();

    }

}

void Tables::on_insert_inTable_button_clicked()
{
    //insert_constructor_->show();
    createTupleConstructor constr_window_{auth_/*,this*/};
    ++tuples_windows_counter_;
    qDebug() << "tuples constructor counter incremented; counter =="+QString::number(tuples_windows_counter_);
    constr_window_./*update_tables_handler*/sql_connection_initialize(); // because qt meta-object method restriction in constructor
    //connect(this,&Tables::tpl_cnstr_upd_tables, &constr_window_/*insert_constructor_*/, &createTupleConstructor::update_tables_handler);
////    connect(&constr_window_, /*&createTupleConstructor::*/SIGNAL(final_query_sig(QString)), this, SLOT(/*&Tables::*/send_custom_query_slot(QString)));
    connect(&constr_window_, static_cast<void (createTupleConstructor::*) (QString)>(&createTupleConstructor::final_query_sig),
            this, static_cast<void (Tables::*) (QString)>(&Tables::get_custom_query_window_));
    connect(this,&Tables::custom_query_windows_close, &constr_window_, &createTupleConstructor::close);

    connect(&constr_window_,static_cast<void (createTupleConstructor::*) (QString const &)>(&createTupleConstructor::closed),[=](QString const & con_name_/*that_mustBclosed*/){

                        if(this->tuples_windows_counter_>0){
                            --this->tuples_windows_counter_;
                            qDebug() << "tuples counter decremented; counter =="+QString::number(tuples_windows_counter_);
                        }
                        if(this->tuples_windows_counter_==0){
                            db_connection::close(con_name_);
                                    qDebug() << "tuples counter ==0 -> connection ::"+con_name_+":: closed";
                        }
                ;});

    //emit tpl_cnstr_upd_tables();
    //constr_window_. setWindowFlag(Qt::Window);
    constr_window_.setModal(false);
    constr_window_.show();
    constr_window_.exec();
    qDebug()<<"TupleConstructor window after exec() before out of scope";
}

void Tables::on_pushButton_2_clicked()
{
    ////show_table_describe_form(auth_.db_name_,auth_.table_name_,this->metaObject()->className());

    QString query_text = "DESC "+auth_.table_name_+';';

    if(!settings_->ui->select_checkbox->isChecked()){        
    db_connection::open(auth_);


    db_connection::set_query(query_text,&model_,ui->tableView,QHeaderView::Stretch);
    }else{
//        CustomQueryResult* new_select_window = new CustomQueryResult{auth_};
        CustomQueryResult new_select_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_select_window, &Custom_Query::close);

        new_select_window.setWindowTitle(auth_.table_name_+": detail table info");
        new_select_window.show();
    //custom_query_result_window_->show();
    new_select_window.custom_query_slot(query_text);
    //if(model_.rowCount()!=0)
    qDebug()<<"BEFORE EXECUTION ::";
    new_select_window.exec();
    }
}
