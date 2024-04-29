#include <tables.h>
#include "ui_tables.h"

#include <QCheckBox>
#include <QLabel>
//#include <QStackedLayout>
#include <QGraphicsEffect>

Tables::Tables(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tables)
//  , db_server_("QMYSQL")
  , auth_(auth__)
  , table_query_window_(new Custom_Query)
  , custom_query_result_window_(new CustomQueryResult{auth_})
  , settings_(new CustomQuerySettings)
{
    ui->setupUi(this);

    ui->statusLine->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

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
    connect(table_query_window_,SIGNAL(send_custom_query(QString)),this,SLOT(send_custom_query_slot(QString)));

    connect(this,SIGNAL(custom_query(QString)),custom_query_result_window_,SLOT(custom_query_slot(QString)));

}

Tables::~Tables()
{
    delete ui;
    delete table_query_window_;
    delete custom_query_result_window_;
}

void Tables::closeEvent(QCloseEvent *event)
{
    emit db_show();
    event->accept();
    table_query_window_->close();
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


    db_connection::set_query(query__,model_,ui->tableView,QHeaderView::Stretch);
    }else{

    custom_query_result_window_->show();
    emit custom_query(query__);

    }
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

    custom_query_result_window_->show();
    emit custom_query(QString("SELECT * FROM ")+auth_.table_name_+(";"));

    }
}

//void Tables::on_pushButton_clicked()
//{

//}

void Tables::on_Custom_Query_Button_clicked()
{
//    table_query_window_->setModal(true);
    table_query_window_->show();
    //emit show_tables_signal();
}

void Tables::on_Query_settings_clicked()
{
    settings_->show();
}
