#include <tables.h>
#include "ui_tables.h"

#include <QCheckBox>
#include <QLabel>
//#include <QStackedLayout>
#include <QGraphicsEffect>

#include <customqueryresult.h>
#include <ui_customqueryresult.h>

Tables::Tables(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tables)
//  , db_server_("QMYSQL")
  , auth_(auth__)
////  , table_query_window_(new Custom_Query)
  , custom_query_result_window_(new CustomQueryResult{auth_})
  , settings_(new CustomQuerySettings)
  , delete_table_window_(new delete_table)
  , constructor_(new CreateTableConstructor{auth_})
//  , insert_constructor_{new createTupleConstructor{auth_}}
{
    ui->setupUi(this);

    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    delete_table_window_->setWindowTitle("Remove table dialog");

    //QCheckBox* checkbox = new QCheckBox(ui->select_from_table_button);
//        checkbox_ = new QCheckBox(ui->select_from_table_button);
//        checkbox_->setText("123");
//        checkbox_->move(ui->select_from_table_button->size().width(),0);

//                                QVBoxLayout* select_button_layout = new QVBoxLayout(ui->select_from_table_button);
//                                //ui->select_from_table_button->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

//                                QLabel* select_button_text = new QLabel{};
//                                select_button_text->setText("Select button meh meh");
//                            //    select_button_text->setMinimumHeight(ui->select_from_table_button->size().height());
//                            //    select_button_text->setMinimumWidth(ui->select_from_table_button->size().width());
//                                //select_button_text->move(0,0);
//                                select_button_layout->addWidget(select_button_text);


//                                checkbox_ = new QCheckBox(ui->select_from_table_button);
//                                checkbox_->setText("123");
//                                //checkbox_->setStyleSheet("background: green;");
//                                select_button_layout->addWidget(checkbox_);

//                            //    int w = ui->select_from_table_button->size().width();
//                                int h = ui->select_from_table_button->size().height();
//                                ui->select_from_table_button->setMinimumHeight(h*1.7);



//        QHBoxLayout * layout1 = new QHBoxLayout(centralWidget1);
//        centralWidget1->setMinimumSize(640,480);
//        centralWidget1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        layout1->addWidget(mte);
//        layout1->addWidget(ui->search_button);
//        setCentralWidget(centralWidget1);
//        centralWidget1->show();










    //SIGNALS
////    connect(table_query_window_,SIGNAL(send_custom_query(QString)),this,SLOT(send_custom_query_slot(QString)));

    connect(this,SIGNAL(custom_query(QString)),custom_query_result_window_,SLOT(custom_query_slot(QString)));

    connect(this,SIGNAL(custom_query(QString,QTableView*)),custom_query_result_window_,SLOT(custom_query_slot(QString,QTableView*)));

//    connect(this,SIGNAL(custom_query(QString,QSqlQueryModel,QTableView*)),custom_query_result_window_,SLOT(custom_query_slot(QString,QSqlQueryModel,QTableView*)));

    //connect(this,SIGNAL(custom_query(QString,int)),custom_query_result_window_,SLOT(custom_query_slot(QString,int)));


    connect(this,SIGNAL(delete_form_request()),delete_table_window_,SLOT(delete_form_request_slot()));
//    connect(this,&Tables::delete_form_request,  delete_table_window_,&delete_table::delete_form_request_slot);

    connect(delete_table_window_,SIGNAL(delete_form_send(QComboBox*)),this,SLOT(delete_form_send_slot(QComboBox*)));
//    connect(delete_table_window_,&delete_table::delete_form_send,this,&Tables::delete_form_send_slot);

    connect(delete_table_window_,SIGNAL(delete_table_sig(QComboBox*)),this,SLOT(delete_table_slot(QComboBox*)));
//    connect(delete_table_window_,&delete_table::delete_table_sig,this,&Tables::delete_table_slot);

    connect(this,SIGNAL(current_tables_list_signal(QList<QString>)),constructor_,SLOT(current_exist_tables_slot(QList<QString>)));

    //CUSTOM CREATE TABLE CONSTRUCTOR
    connect(constructor_,SIGNAL(send_custom_query(QString)),this,SLOT(constructor_create_tbl_query_slot(QString)));

////    connect(this,SIGNAL(close_custom_query_form()),table_query_window_,SLOT(close_window()));

    connect(this,&Tables::constructor_query_success, [=] () { constructor_->close(); });

    connect(this,&Tables::constructor_query_fails, constructor_, &CreateTableConstructor::constructor_query_fails_handle,Qt::QueuedConnection);

//    connect(this,&Tables::tpl_cnstr_upd_tables, insert_constructor_, &createTupleConstructor::update_tables_handler);


}

Tables::~Tables()
{
    delete ui;
////    delete table_query_window_;
    delete custom_query_result_window_;
    delete settings_;
    delete delete_table_window_;
    delete constructor_;
//    delete insert_constructor_;
}

void Tables::closeEvent(QCloseEvent *event)
{
    emit db_show();
    event->accept();
////    table_query_window_->close();
    emit custom_query_windows_close();

    custom_query_result_window_->close();
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

void Tables::show_tables()
{
    //if(QSqlDatabase::database().databaseName().isNull())
        db_connection::close(); // if database name in connection is not added
    // ==> close current and open new connect

    db_connection::open(auth_);


    db_connection::set_query("SHOW TABLES;",model_,ui->tableView,QHeaderView::Stretch);


    select_cells(0,0, ui->tableView);

    auth_.table_name_=ui->tableView->model()->data(ui->tableView->currentIndex()).toString();


        qDebug() << "Number of existinf DBs::" <<(model_.rowCount());

        //ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        //ui->tableView->selectionModel()->select( ui->tableView->model()->index(0,0), QItemSelectionModel::ClearAndSelect );
        //ui->tableView->selectionModel()->select(ui->tableView->model()->index(0,0), QItemSelectionModel::Select);
        //ui->tableView->setCurrentIndex(ui->tableView->model()->index(0,0));
        //ui->tableView->selectionModel()->select(ui->tableView->model()->index(0,0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);

        if(!auth_.table_name_.isNull()){
            ui->select_from_table_button->setEnabled(true);
            ui->select_from_table_button->setStyleSheet("background: green; color: white;");
        }
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


    if(db_connection::set_query(query__,model_,ui->tableView,QHeaderView::Stretch))
        emit close_custom_query_form();
    if(!ui->tableView->model()->columnCount()) {
        show_tables();
        qDebug() << "rowCount() in model_==0::display result ignored.";

    }
    }else{

        CustomQueryResult new_result_window{auth_};
        //new_result_window.show();
//    custom_query_result_window_->show();
    new_result_window.custom_query_slot(query__, /*new_result_window->model_,*/ new_result_window.ui->tableView);
    //if(new_result_window.ui->tableView->model()->rowCount()!=0)
    if((new_result_window.ui->tableView->model())!=nullptr) {

        qDebug() << "Number of columns in tableView->model()::"<<new_result_window.ui->tableView->model()->columnCount();
        emit close_custom_query_form();
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


    if(db_connection::set_query(/*query__*/custom_query_window__->get_text(),model_,ui->tableView,QHeaderView::Stretch))
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
    new_result_window.custom_query_slot(/*query__*/custom_query_window__->get_text(), /*new_result_window->model_,*/ new_result_window.ui->tableView);
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


void Tables::delete_form_send_slot(QComboBox *comboBox__)
{
    db_connection::open(auth_);


    db_connection::set_query("SHOW TABLES;",model_,comboBox__);

    comboBox__->setCurrentIndex(-1); //for blank cell default
}

void Tables::delete_table_slot(QComboBox *comboBox__)
{
    db_connection::open(auth_);

    bool query_success=db_connection::set_query("DROP TABLE "+comboBox__->currentText()+";",model_,comboBox__);

    if(query_success){

        db_connection::close();
        on_showDB_button_clicked();
        //select_cells(ui->tableView->model()->index(0, 0, QModelIndex()));
        select_cells(0,0, ui->tableView);

    } else {

        QMessageBox::warning(this,"Warning","Table is not droped. May be it was been droped earlier?");

    }
    on_showDB_button_clicked(); // view tables after deletion
}

void Tables::constructor_create_tbl_query_slot(QString query__)
{
    db_connection::open(auth_);


//    if(!db_connection::set_query(query__,model_,ui->tableView,QHeaderView::Stretch))//;
//    {
//        QSqlQuery qry = QSqlQuery(QSqlDatabase::database().connectionName());
//        QSqlError err_=qry.lastError(); //<<-- THIS NOT WORKING, ERROR IS EMPTY BUT IT MUSTN'T
//        ui->statusLine->setText(err_.nativeErrorCode());
//        //qDebug() << QSqlDatabase::database().lastError();
//        qDebug() << err_.text();
//        //qDebug() << "!!!" << (QSqlQuery(QSqlDatabase::database().connectionName(auth_.db_server_)).lastError());

//    }
    if(db_connection::set_query(query__,model_,ui->tableView,QHeaderView::Stretch)) //TABLE MAIN WINDOW MODEL
        emit constructor_query_success();
    else
        emit constructor_query_fails();
    show_tables();
}


void Tables::on_showDB_button_clicked()
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


    db_connection::set_query(QString("SELECT * FROM ")+auth_.table_name_+(";"),model_,ui->tableView,QHeaderView::Stretch);
    }else{
//        CustomQueryResult* new_select_window = new CustomQueryResult{auth_};
        CustomQueryResult new_select_window{auth_};
        new_select_window.show();
    //custom_query_result_window_->show();
    new_select_window.custom_query_slot(QString("SELECT * FROM ")+auth_.table_name_+(";")/*,new_select_window->ui->tableView*/, /*new_select_window->model_,*/new_select_window.ui->tableView);
    //if(model_.rowCount()!=0)
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

void Tables::on_pushButton_clicked()
{
    delete_table_window_->setModal(true);
    delete_table_window_->show();
    emit delete_form_request();
}

void Tables::on_create_table_button_clicked()
{
    constructor_->setCurrentIndex(0);

    QList<QString> list_to_send;

    size_t size_of_list_=ui->tableView->model()->rowCount();

    qDebug() << "NUMBER OF TABLES::" << size_of_list_;

    //qDebug() << "CURRENT TEXT::"<<ui->tableView->model()->index(1,0).data().toString();
    for(size_t i=0;i!=size_of_list_;++i)
        list_to_send.append(ui->tableView->model()->index(i,0).data().toString());

    qDebug()<< "CURRENT LIST OF STRINGs::"<<list_to_send;

    emit current_tables_list_signal(list_to_send);

    //emit default_db()

    constructor_->show();
}

void Tables::on_insert_inTable_button_clicked()
{

    //insert_constructor_->show();
    createTupleConstructor constr_window_{auth_};
    constr_window_./*update_tables_handler*/sql_connection_initialize(); // because qt meta-object method restriction in constructor
    //connect(this,&Tables::tpl_cnstr_upd_tables, &constr_window_/*insert_constructor_*/, &createTupleConstructor::update_tables_handler);
////    connect(&constr_window_, /*&createTupleConstructor::*/SIGNAL(final_query_sig(QString)), this, SLOT(/*&Tables::*/send_custom_query_slot(QString)));
    connect(&constr_window_, static_cast<void (createTupleConstructor::*) (QString)>(&createTupleConstructor::final_query_sig),
            this, static_cast<void (Tables::*) (QString)>(&Tables::get_custom_query_window_));
    //emit tpl_cnstr_upd_tables();
    constr_window_.setModal(false);
    constr_window_.show();
    constr_window_.exec();
}
