#include "createtupleconstructor.h"
#include "ui_createtupleconstructor.h"

createTupleConstructor::createTupleConstructor(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createTupleConstructor)
  , auth_(auth__)
  , confirm_window_(new insertTupleConfirm{})
  , non_static_connection_(new db_connection)

{
    ui->setupUi(this);

//    SIGNALS::

    connect(ui->comboBox,&QComboBox::currentTextChanged, this, &createTupleConstructor::table_changed_handler);
}

createTupleConstructor::~createTupleConstructor()
{
    delete ui;
    delete confirm_window_;
}

void createTupleConstructor::on_okButton_clicked()
{
    confirm_window_->show();
}

void createTupleConstructor::on_update_tables_button_clicked()
{
    db_connection::open(auth_);


    db_connection::set_query("SHOW TABLES;",this->non_static_connection_->model_,ui->comboBox,1);

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
