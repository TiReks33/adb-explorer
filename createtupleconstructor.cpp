#include "createtupleconstructor.h"
#include "ui_createtupleconstructor.h"

//int createTupleConstructor.multi_connection::con_counter_ = 0;
//int createTupleConstructor::unique_number_ = 0;
//QStringList createTupleConstructor::multi_con_names_;

createTupleConstructor::createTupleConstructor(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createTupleConstructor)
  , auth_(auth__)
  , confirm_window_(new insertTupleConfirm{})
  //, non_static_connection_(new db_connection)
//  , list_selection_window_(new TwoListSelection{})
  //, con_name_{""}
{
    ui->setupUi(this);

//    SIGNALS::

    connect(ui->comboBox,&QComboBox::currentTextChanged, this, &createTupleConstructor::table_changed_handler);
    connect(ui->plainTextEdit_2,&QPlainTextEdit::textChanged, this, &createTupleConstructor::columns_selected_handler);


qDebug() << "window counter::"<< multi_con_.con_counter_;
    //on_update_tables_button_clicked();
}

createTupleConstructor::~createTupleConstructor()
{
    delete ui;
    delete confirm_window_;
//    delete list_selection_window_;

//    if(con_name_!=""){
//        db_connection::close_con(con_name_);
//        multi_con_names_.removeAll(con_name_);
//        }
//    if(con_counter_>=0)
//        --con_counter_;
//    if(con_counter_==0)
//        unique_number_=0;
    qDebug() << "TupleConstr ~Destructor activated";
}

void createTupleConstructor::reset()
{
    ui->frame->setEnabled(true);
    ui->frame->setStyleSheet("background: white");
    ui->frame_2->setEnabled(false);
    ui->frame_2->setStyleSheet("background: palette(window)");
    ui->frame_3->setEnabled(false);
    ui->frame_3->setStyleSheet("background: palette(window)");
    ui->frame_4->setEnabled(false);
    ui->frame_4->setStyleSheet("background: palette(window)");
    ui->plainTextEdit_2->clear();
    ui->label_amount->setText(QString::number(0));
    tuples_added_=0;
}

void createTupleConstructor::on_okButton_clicked()
{
    confirm_window_->show();
}

void createTupleConstructor::on_update_tables_button_clicked()
{
//    if(!QSqlDatabase::database(multi_con_.con_name_).isOpen()){
//    QString con_name = this->metaObject()->className()+QString::number(multi_con_.unique_number_);
//    int db_con = db_connection::open(auth_,con_name,1);
//    if(!db_con){
//        qDebug() << QString("(x)There is error while update tables (connection %1 is not established).").arg(con_name);
//        return;
//    } else if (db_con==1){
//        qDebug() << QString("Connection %1 successful established.").arg(con_name);

//        multi_con_.con_name_=con_name;
//        multi_con_.multi_con_names_.append(con_name);
//        ++multi_con_.con_counter_;
//        ++multi_con_.unique_number_;
//    } else { //if already exist
//        qDebug() << QString("Connection %1 already exist.").arg(con_name);
//    }

//    qDebug() << "Con counter after::"<< multi_con_.con_counter_;
////    QSqlQueryModel* model = new QSqlQueryModel;
////    int models_cap = multi_con_models_.length();
////    qDebug() << "length of multi_con_models_"<< models_cap;
////    multi_con_models_.append(model);
//    }

    if(!db_connection::open(auth_,this->metaObject(),&multi_con_)){
        qDebug() << QString("(x)There is error while update tables (connection is not established).");
        return;
    }

    db_connection::set_query("SHOW TABLES;", /*multi_con_models_.at(models_cap)*/tables_model_,ui->comboBox,multi_con_.con_name_,1);

    ui->comboBox->setCurrentIndex(-1);

//      ui->frame->setStyleSheet("background: palette(window)");
}

void createTupleConstructor::/*update_tables_handler*/sql_connection_initialize()
{
    //multi_con_.con_name_=this->metaObject()->className()+QString::number(multi_con_.unique_number_);
    on_update_tables_button_clicked();
    //    ui->comboBox->setCurrentIndex(-1);
}

void createTupleConstructor::import_list(QStringList list__)
{
    qDebug() << "List from double list::" << list__;
    ui->plainTextEdit_2->setPlainText(pack_(list__));
}

void createTupleConstructor::on_reset_button_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to start"
                                " adding fields from the beginning? All current progress will be lost.",
                                                             QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";
        reset();
      } else {
        qDebug() << "cancel";
      }

}

void createTupleConstructor::table_changed_handler(QString const& string_)
{
    auth_.table_name_=string_;
    qDebug() << "auth table name after textchanged()"<<auth_.table_name_;
    if (string_==""){
        ui->frame->setEnabled(true);
        ui->update_tables_button->setEnabled(true);
        ui->frame->setStyleSheet("background: white");
        ui->frame_2->setEnabled(false);
        ui->frame_2->setStyleSheet("background: palette(window)");
        ui->frame_3->setEnabled(false);
        ui->frame_3->setStyleSheet("background: palette(window)");
    } else {
        ui->frame->setEnabled(false);
        ui->update_tables_button->setEnabled(false);
        ui->frame->setStyleSheet("background: palette(window)");
        ui->frame_2->setEnabled(true);
        ui->frame_2->setStyleSheet("background: white");
    }
}

void createTupleConstructor::columns_selected_handler()
{
    qDebug() << "(✓)columns selected";
    if (ui->plainTextEdit_2->toPlainText()!=""){
        ui->frame_3->setEnabled(true);
        ui->frame_3->setStyleSheet("background: white");
        ui->frame_4->setEnabled(true);
        ui->frame_4->setStyleSheet("background: white");
        ui->frame_2->setEnabled(false);
        ui->frame_2->setStyleSheet("background: palette(window)");
    }
}

void createTupleConstructor::on_addColsButton_clicked()
{
//    QStringList input;
//    for (int i = 0; i < 10; i++) {
//      input << QString("item-%1").arg(i);
//    }
//    double_list->clear();
//    double_list->addAvailableItems(input);
//    double_list->show();
    /*auth_.db_name_=ui->comboBox->currentText();*/

    TwoListSelection doublelist{auth_};
connect(&doublelist,SIGNAL(export_list(QStringList)),this,SLOT(import_list(QStringList)));
    doublelist.update_doublelist();
    doublelist.setModal(true);
    doublelist/*list_selection_window_->*/.show();
    doublelist.exec();
}

void createTupleConstructor::closeEvent(QCloseEvent *event)
{
//        if(multi_con_.con_name_!=""){
//            db_connection::close_con(multi_con_.con_name_);
//            multi_con_.multi_con_names_.removeAll(multi_con_.con_name_);
//            }
//        if(multi_con_.con_counter_>=0)
//            --multi_con_.con_counter_;
//        if(multi_con_.con_counter_==0)
//            multi_con_.unique_number_=0;
    multi_con_.delete_sql_connection();
        event->accept();
}

void createTupleConstructor::on_addTupleButton_clicked()
{



    ui->label_amount->setText(QString::number(++tuples_added_));
}
