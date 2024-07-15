#include "createtupleconstructor.h"
#include "ui_createtupleconstructor.h"

int createTupleConstructor::con_counter_ = 0;
int createTupleConstructor::unique_number_ = 0;
QStringList createTupleConstructor::multi_con_names_;

createTupleConstructor::createTupleConstructor(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createTupleConstructor)
  , auth_(auth__)
  , confirm_window_(new insertTupleConfirm{})
  //, non_static_connection_(new db_connection)
  , list_selection_window_(new TwoListSelection{})
  , con_name_("")
{
    ui->setupUi(this);

//    SIGNALS::

    connect(ui->comboBox,&QComboBox::currentTextChanged, this, &createTupleConstructor::table_changed_handler);
qDebug() << "window counter::"<< con_counter_;
    //on_update_tables_button_clicked();
}

createTupleConstructor::~createTupleConstructor()
{
    delete ui;
    delete confirm_window_;
    delete list_selection_window_;

//    if(con_name_!=""){
//        db_connection::close_con(con_name_);
//        multi_con_names_.removeAll(con_name_);
//        }
//    if(con_counter_>=0)
//        --con_counter_;
//    if(con_counter_==0)
//        unique_number_=0;
    qDebug() << "TupleConstructor activated";
}

void createTupleConstructor::on_okButton_clicked()
{
    confirm_window_->show();
}

void createTupleConstructor::on_update_tables_button_clicked()
{
    if(!QSqlDatabase::database(con_name_).isOpen()){
    QString con_name = this->metaObject()->className()+QString::number(unique_number_);
    int db_con = db_connection::open(auth_,con_name,1);
    if(!db_con){
        qDebug() << QString("(x)There is error while update tables (connection %1 is not established).").arg(con_name);
        return;
    } else if (db_con==1){
        qDebug() << QString("Connection %1 successful established.").arg(con_name);

        con_name_=con_name;
        multi_con_names_.append(con_name);
        ++con_counter_;
        ++unique_number_;
    } else { //if already exist
        qDebug() << QString("Connection %1 already exist.").arg(con_name);
    }

    qDebug() << "Con counter after::"<< con_counter_;
//    QSqlQueryModel* model = new QSqlQueryModel;
//    int models_cap = multi_con_models_.length();
//    qDebug() << "length of multi_con_models_"<< models_cap;
//    multi_con_models_.append(model);
    }
    db_connection::set_query("SHOW TABLES;", /*multi_con_models_.at(models_cap)*/tables_model_,ui->comboBox,con_name_,1);

    ui->comboBox->setCurrentIndex(-1);

//      ui->frame->setStyleSheet("background: palette(window)");
}

void createTupleConstructor::update_tables_handler()
{
    on_update_tables_button_clicked();
//    ui->comboBox->setCurrentIndex(-1);
}

void createTupleConstructor::on_reset_button_clicked()
{
    ui->frame->setEnabled(true);
    ui->frame->setStyleSheet("background: white");
    ui->frame_2->setEnabled(false);
    ui->frame_2->setStyleSheet("background: palette(window)");
    ui->frame_3->setEnabled(false);
    ui->frame_3->setStyleSheet("background: palette(window)");
}

void createTupleConstructor::table_changed_handler(QString const& string_)
{
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

void createTupleConstructor::on_addColsButton_clicked()
{
//    QStringList input;
//    for (int i = 0; i < 10; i++) {
//      input << QString("item-%1").arg(i);
//    }
//    double_list->clear();
//    double_list->addAvailableItems(input);
//    double_list->show();
    list_selection_window_->show();
}

void createTupleConstructor::closeEvent(QCloseEvent *event)
{
        if(con_name_!=""){
            db_connection::close_con(con_name_);
            multi_con_names_.removeAll(con_name_);
            }
        if(con_counter_>=0)
            --con_counter_;
        if(con_counter_==0)
            unique_number_=0;
        event->accept();
}
