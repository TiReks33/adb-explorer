﻿#include "createtupleconstructor.h"
#include "ui_createtupleconstructor.h"

//int createTupleConstructor.multi_connection::con_counter_ = 0;
//int createTupleConstructor::unique_number_ = 0;
//QStringList createTupleConstructor::multi_con_names_;

createTupleConstructor::createTupleConstructor(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createTupleConstructor)
  , auth_(auth__)
////  , confirm_window_(new insertTupleConfirm{})
  //, non_static_connection_(new db_connection)
//  , list_selection_window_(new TwoListSelection{})
  //, con_name_{""}
{
    ui->setupUi(this);

//    SIGNALS::

    connect(ui->comboBox,&QComboBox::currentTextChanged, this, &createTupleConstructor::table_changed_handler);
    connect(ui->plainTextEdit_2,&QPlainTextEdit::textChanged, this, &createTupleConstructor::columns_selected_handler);
    connect(ui->plainTextEdit,&QPlainTextEdit::textChanged, [=] ()
            {
                    ui->statusLine->clear();
            });
    //connect(this, &createTupleConstructor::final_query_sig, confirm_window_, &insertTupleConfirm::final_query_slot);

qDebug() << "window counter::"<< multi_con_.con_counter_;
    //on_update_tables_button_clicked();



}

createTupleConstructor::~createTupleConstructor()
{
    delete ui;
    ////delete confirm_window_;
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
    tuples_.clear();
    ui->comboBox->setCurrentIndex(-1);
    ui->plainTextEdit->clear();
}

void createTupleConstructor::on_okButton_clicked()
{
    QString final_query = "INSERT INTO "+auth_.table_name_+" ("+ui->plainTextEdit_2->toPlainText()+") "+"VALUES ";

    int count = tuples_.count();
    for (int i=0;i!=count;++i)
    {
        final_query+= '(' + tuples_.at(i) + ')';
        if (i!=count-1) final_query+=", ";
    }

    final_query+=';';

    qDebug() << "final query::" <<final_query;
    emit final_query_sig(final_query);
    //confirm_window_->show();
    this->close();
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

    ////QString _con_name = db_connection::get_unique_con_name(this->metaObject(),&multi_con_);

//    if(!db_connection::open(auth_,this->metaObject(),&multi_con_)){
   // if(!db_connection::open(auth_,_con_name,&multi_con_)){
    if(!db_connection::open(auth_)){
        qDebug() << QString("(x)There is error while update tables (connection is not established).");
        return;
    }

    db_connection::set_query("SHOW TABLES;", /*multi_con_models_.at(models_cap)*/&tables_model_,ui->comboBox/*,_con_name*//*multi_con_.con_name_*//*,1*/);

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


//    multi_con_.delete_sql_connection();
    if(describe_form_)describe_form_->close();
    emit closed(subconnection_name_);
        event->accept();
}

void createTupleConstructor::on_addTupleButton_clicked()
{
    //IS EMPTY check
    if(ui->plainTextEdit->toPlainText().isEmpty()){
        ui->statusLine->setText("Tuple is empty. Nothing to add.");
        return;
    } else {
        //EXCLUDING ADDITIONAL BRACKETS FROM SINGLE TURPLE
        QRegularExpression re("^[^()]*$");
        QRegularExpressionValidator v(re, 0);
        QString str = ui->plainTextEdit->toPlainText();
        int pos=0;
        if(v.validate(str, pos)!=QValidator::Acceptable)
        {
            ui->statusLine->setText("There's no need for additional brackets in tuple.");
            return;
        } else{
            //COUNT NUMBER OF COMMAS
//            QRegularExpression re("^[a-zA-Z0-9_]*$");
//            QRegularExpressionValidator v(re, 0);
//            QString str = ui->tbl_name_line_0->text();
//            int pos=0;
//            if(v.validate(str, pos)!=QValidator::Acceptable)
//            {
//                ui->statusLine_0->setText("Incorrect symbols in table's name. Please use low and upper letters, digits or '_'");
//                return;
//            }

            int input_commas = ui->plainTextEdit_2->toPlainText().count(',');
            qDebug() << "number of commas input::"<< input_commas;
            int tuple_commas = ui->plainTextEdit->toPlainText().count(',');
            qDebug() << "number of commas in tuple::"<<tuple_commas;
                if(input_commas!=tuple_commas)
                {
                        ui->statusLine->setText("Number of elements in tuple does not match to number of selected attributes.");
                    return;
                }
        }
    }

    tuples_.append(ui->plainTextEdit->toPlainText());

    ui->label_amount->setText(QString::number(++tuples_added_));
}

void createTupleConstructor::on_describeButton_clicked()
{
    //Tables* window = qobject_cast<Tables *>(parent());
    //window->show_table_describe_form(auth_.db_name_,ui->comboBox->currentText(),auth::con_name_,this,Qt::Dialog,Qt::WindowModal);

    if(describe_form_!=nullptr)//{
    describe_form_->close();

    ////parent_->show_table_describe_form(ui->ref_DB_comboBox_2->currentText(),ui->ref_table_comboBox_2->currentText(),parent_->metaObject()->className(),this,Qt::Dialog,Qt::WindowModal);
    //QString con_name = QString(this->metaObject()->className())+" describe_form";
    //QString db_name = auth_.db_name_;
    QString table_name = ui->comboBox->currentText();
    ////db_connection::close(con_name);
    //db_connection::remove(con_name__);
    //auth __auth = auth_;
    //__auth.db_name_ = db_name;


    //QSqlDatabase::database(con_name,false).setDatabaseName(__auth.db_name_); //2


    describe_form_ = new CustomQueryResult{auth_};


    describe_form_-> setAttribute( Qt::WA_DeleteOnClose, true );


//    connect(this,static_cast<void (createTupleConstructor::*)()>(&createTupleConstructor::closed),[=](){
//        if(describe_form_)describe_form_->close();
//        emit closed(subconnection_name_);
//    });

    //Tables* parent_window = qobject_cast<Tables *>(parent());

//    connect(this,&createTupleConstructor::destroyed,[=](){ /*describe_form_ = nullptr;qDebug()<<"OBJISNULLED!1";*/
//    emit closed(con_name);
//    });


    describe_form_->setWindowTitle(table_name);


describe_form_->custom_query_slot("DESCRIBE "+table_name+(";"),subconnection_name_); //3



//new_select_window.setParent(parent__);
//new_select_window.setWindowFlag(window_type_flag__);
//new_select_window.setWindowModality(window_modality_flag__);

////
describe_form_->show();

describe_form_-> exec();
}
