#include "createtupleconstructor.h"
#include "ui_createtupleconstructor.h"



createTupleConstructor::createTupleConstructor(auth& auth__,QWidget *parent) :
    QWidget/*QDialog*/(parent),
    ui(new Ui::createTupleConstructor)
  , auth_(auth__)

{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/pic/anthead2.png"));

//    SIGNALS::

        signals_init();


}

createTupleConstructor::~createTupleConstructor()
{
    delete ui;

    qDebug() << "TupleConstr ~Destructor activated";
}


void createTupleConstructor::signals_init()
{
    connect(ui->comboBox,&QComboBox::currentTextChanged, [=](QString const& string_){
//        auth_.table_name_=string_;
//        qDebug() << "auth table name after textchanged()"<<auth_.table_name_;
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
    });


    connect(ui->plainTextEdit_2,&QPlainTextEdit::textChanged, [=](){
        qDebug() << "(✓)columns selected";
        if (ui->plainTextEdit_2->toPlainText()!=""){
            ui->frame_3->setEnabled(true);
            ui->frame_3->setStyleSheet("background: white");
            ui->frame_4->setEnabled(true);
            ui->frame_4->setStyleSheet("background: white");
            ui->frame_2->setEnabled(false);
            ui->frame_2->setStyleSheet("background: palette(window)");
        }
    });


    connect(ui->plainTextEdit,&QPlainTextEdit::textChanged, [=](){
                    ui->statusLine->clear();
            });


    connect(ui->okButton,&QPushButton::clicked,[=]{
//        QString final_query = "INSERT INTO "+auth_.table_name_+" ("+ui->plainTextEdit_2->toPlainText()+") "+"VALUES ";
        QString final_query = QString("INSERT INTO `%1` (`%2`) VALUES ").arg(QString(escape_sql_backticks(/*auth_.table_name_*/ui->comboBox->currentText())))
                                                                        .arg(QString(escape_sql_backticks(ui->plainTextEdit_2->toPlainText())));

        int count = tuples_.count();
        for (int i=0;i!=count;++i)
        {
            final_query+= '(' + tuples_.at(i) + ')';
            if (i!=count-1) final_query+=", ";
        }

        final_query+=';';

        qDebug() << "final query::" <<final_query;
        emit final_query_sig(final_query);

        this->close();
    });


    connect(ui->update_tables_button,&QPushButton::clicked,this,&createTupleConstructor::update_tables_list);

    connect(ui->reset_button,&QPushButton::clicked,[=]{
        QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to start"
                                    " adding fields from the beginning? All current progress will be lost.",
                                                                 QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";
            reset();
          } else {
            qDebug() << "cancel";
          }
    });

    connect(ui->addColsButton,&QPushButton::clicked,[=]{
        TwoListSelection doublelist{auth_};
    //    connect(&doublelist,SIGNAL(export_list(QStringList)),this,SLOT(import_list(QStringList)));
        connect(&doublelist,&TwoListSelection::export_list,[=](QStringList list__){
            qDebug() << "List from double list::" << list__;
            ui->plainTextEdit_2->setPlainText(pack_(list__));
        });
        QString const query = QString("SHOW COLUMNS FROM `%1`").arg(QString(escape_sql_backticks(/*auth_.table_name_*/ui->comboBox->currentText())));
        doublelist.update_doublelist(query);
        doublelist.setModal(true);
        doublelist/*list_selection_window_->*/.show();
        doublelist.exec();
    });

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

void createTupleConstructor::show()
{
    QWidget::show();
    //QDialog::show();
    update_tables_list();
    qDebug() << "createTupleConstructor's 'QWidget::Show()' overloaded method called.";
}

void createTupleConstructor::update_tables_list()
{
    if(!db_connection::open(auth_)){
        qDebug() << QString("(x)There is error while update tables (connection is not established).");
        return;
    }

    db_connection::set_query("SHOW TABLES;", &tables_model_,ui->comboBox);

    ui->comboBox->setCurrentIndex(-1);
}


//void createTupleConstructor::import_list(QStringList list__)
//{
//    qDebug() << "List from double list::" << list__;
//    ui->plainTextEdit_2->setPlainText(pack_(list__));
//}

//void createTupleConstructor::on_reset_button_clicked()
//{
//    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to start"
//                                " adding fields from the beginning? All current progress will be lost.",
//                                                             QMessageBox::Yes|QMessageBox::No);
//      if (reply == QMessageBox::Yes) {
//        qDebug() << "Yes was clicked";
//        reset();
//      } else {
//        qDebug() << "cancel";
//      }

//}


//void createTupleConstructor::columns_selected_handler()
//{
//    qDebug() << "(✓)columns selected";
//    if (ui->plainTextEdit_2->toPlainText()!=""){
//        ui->frame_3->setEnabled(true);
//        ui->frame_3->setStyleSheet("background: white");
//        ui->frame_4->setEnabled(true);
//        ui->frame_4->setStyleSheet("background: white");
//        ui->frame_2->setEnabled(false);
//        ui->frame_2->setStyleSheet("background: palette(window)");
//    }
//}

//void createTupleConstructor::on_addColsButton_clicked()
//{
//    TwoListSelection doublelist{auth_};
////    connect(&doublelist,SIGNAL(export_list(QStringList)),this,SLOT(import_list(QStringList)));
//    connect(&doublelist,&TwoListSelection::export_list,[=](QStringList list__){
//        qDebug() << "List from double list::" << list__;
//        ui->plainTextEdit_2->setPlainText(pack_(list__));
//    });
//    doublelist.update_doublelist();
//    doublelist.setModal(true);
//    doublelist/*list_selection_window_->*/.show();
//    doublelist.exec();
//}

void createTupleConstructor::closeEvent(QCloseEvent *event)
{
    if(describe_form_)describe_form_->close();
    //emit closed(this->subconnection_name_);
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

    ui->plainTextEdit->clear();
}

void createTupleConstructor::on_describeButton_clicked()
{
    //Tables* window = qobject_cast<Tables *>(parent());
    //window->show_table_describe_form(auth_.db_name_,ui->comboBox->currentText(),auth::con_name_,this,Qt::Dialog,Qt::WindowModal);

    if(describe_form_!=nullptr)//{
    describe_form_->close();


    QString table_name = ui->comboBox->currentText();


    describe_form_ = new CustomQueryResult{auth_,this};


    describe_form_-> setAttribute( Qt::WA_DeleteOnClose, true );


    describe_form_->setWindowTitle(table_name);


//describe_form_->custom_query_slot("DESCRIBE "+table_name+(";"),subconnection_name_); //3
    describe_form_->custom_query_slot(QString("DESCRIBE `%1`").arg(QString(escape_sql_backticks(table_name)))/*,subconnection_name_*/);

    describe_form_->setModal(false);

describe_form_->show();

//describe_form_-> exec();
}
