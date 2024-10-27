#include "create_table_constructor.h"
#include "ui_create_table_constructor.h"
#include "tables.h"

QDebug operator<<(QDebug stream__, auth const &auth__){
    stream__ << "db_server::" << auth__.db_driver_
             << "host::" << auth__.host_
             << "db_name::" << auth__.db_name_
//             << "table_name::" << auth__.table_name_
                ;
    return stream__;
}

CreateTableConstructor::CreateTableConstructor(auth& auth__,/*QWidget*/Tables *parent) :
    QStackedWidget(parent),
    ui(new Ui::CreateTableConstructor)
  , statusBar_0(new scrolledStatusBar)
  , statusBar_1(new scrolledStatusBar)
  , statusBar_2(new scrolledStatusBar)
  , first_attribute_(true)
  , first_key_(true)
  , auth_(auth__)

  , parent_(parent)

  , reload_con_button_2{new reloadButton{this,"darkslategray","snow",true,false,"<u>R</u>eload DB list"}}

{
    initForm();

    //SIGNALs

    signals_init();

}


CreateTableConstructor::~CreateTableConstructor()
{
    delete ui;

    delete statusBar_0;
    delete statusBar_1;
    delete statusBar_2;

    // close and remove additional sql server connection by destructor
    db_connection::close(subconnection_name_);
    db_connection::remove(subconnection_name_);
}


void CreateTableConstructor::signals_init()
{
    connect(ui->atr_type_comboBox_1,SIGNAL(currentTextChanged(QString const&)),this,SLOT(AttrTypeChanged(QString const&)));

    connect(ui->foreign_key_checkBox_2,SIGNAL(stateChanged(int)),this,SLOT(foreignkeychecked(int)),Qt::QueuedConnection);

    connect(ui->on_delete_checkBox_2,&QCheckBox::stateChanged,[=](int state){
        if(state==0)
            ui->onDelete_comboBox_2->setEnabled(false);
         else
            ui->onDelete_comboBox_2->setEnabled(true);
    });

    connect(ui->on_update_checkBox_2,&QCheckBox::stateChanged,[=](int state){
        if(state==0)
            ui->onUpdate_comboBox_2->setEnabled(false);
         else
            ui->onUpdate_comboBox_2->setEnabled(true);
    });

    connect(ui->ref_DB_comboBox_2,static_cast<void (QComboBox::*)(int)> (&QComboBox::currentIndexChanged), this, [=] (int current_index__){
        //qDebug() << "ui->ref_DB_comboBox_2 current index changed";
        ui->ref_DB_comboBox_2->currentTextChanged(ui->ref_DB_comboBox_2->itemText(current_index__));
    });

    QObject::connect(ui->ref_DB_comboBox_2,&QComboBox::currentTextChanged, this, [=](QString const&current_DB_name__){

            //submodel_2_.clear();
        ////if(!current_DB_name__.isEmpty()){

                ui->ref_DB_comboBox_2->setToolTip(current_DB_name__);

                db_connection::close(subconnection_name_);

                auth auth_copy=auth_;

                auth_copy.db_name_=current_DB_name__;

                QSqlDatabase::database(subconnection_name_,false).setDatabaseName(auth_copy.db_name_);


                db_connection::try_to_reopen(auth_copy,subconnection_name_);



                if(db_connection::set_query("SHOW TABLES;", &submodel_1_,ui->ref_table_comboBox_2,subconnection_name_)){
                    // if no tables in DB -->> clear previous keys comboBox model (preventing model's nullptr calling)
                    if(!ui->ref_table_comboBox_2->count())
                    {
                        submodel_2_.clear();

                        ui->ref_table_comboBox_2->setToolTip("*empty table*");
                        ui->ref_key_comboBox_2->setToolTip("*no corteges (empty table)*");

                        ui->describe_tbl_button_2->setEnabled(false);

                    } else{
                        ui->describe_tbl_button_2->setEnabled(true);
                    }
                }
          ////}
    },Qt::QueuedConnection); // !!!!!!!!!!!!!!!!!!!


    connect(ui->ref_table_comboBox_2,&QComboBox::currentTextChanged, this,[=](QString const&current_table_name__){

            db_connection::set_query(QString("SHOW COLUMNS FROM `%1`").arg(QString(adb_utility::escape_sql_backticks(current_table_name__))), &submodel_2_,ui->ref_key_comboBox_2,subconnection_name_);

            ui->ref_table_comboBox_2->setToolTip(current_table_name__);

    },Qt::QueuedConnection);


    connect(ui->ref_key_comboBox_2,&QComboBox::currentTextChanged,this,[=](QString const&current_key_name__){
       ui->ref_key_comboBox_2->setToolTip(current_key_name__);
    },Qt::QueuedConnection);



    connect(ui->send_button,&QPushButton::clicked,[=]{
        adb_utility::set_cursor_to_end_(this->ui->plainTextEdit_2);

        ui->plainTextEdit_2->insertPlainText(" );");
        //qDebug() << "SQL FINAL QUERY::" << ui->plainTextEdit_2->toPlainText();

        emit send_custom_query(ui->plainTextEdit_2->toPlainText(),current_t_name_);
    });


    connect(ui->next_1,&QPushButton::clicked,[=]{
        if(!first_attribute_){
            ui->plainTextEdit_2->clear();
            ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText()/*+')'*/);


            ui->foreign_key_combobox_2->addItems(attributes_);
            ui->foreign_key_combobox_2->setCurrentIndex(-1);


            //reloadDatabaseComboBox();


            this->setCurrentIndex(2);

        } else {
            statusBar_1->get_line()->setText("Please, add 1 or more attributes to current table before next step.");
        }

    });


    connect(ui->plus_button_1,&QPushButton::clicked,[=]{
        if(add_attributes(ui->plainTextEdit_1)){ // IF CHECK STRING FUNCTION IS OK (REG. EXP. CHECK)

            first_attribute_=false;

            ui->atr_name_line_1->setText("");
            ui->atr_type_comboBox_1->setCurrentIndex(-1);
            ui->atr_type_capacity_1->setText("");

            ui->primary_key_checkBox_1->setChecked(false);
            ui->NOT_NULL_checkBox_1->setChecked(false);
            ui->AUTO_INCREMENT_checkBox_1->setChecked(false);

            ui->number_line_1->setText(QString::number(++attributes_added_));
        }
    });


    connect(ui->erase_button_1,&QPushButton::clicked,[=]{
        QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to start"
                                    " adding attributes from the beginning? All current progress will be lost.",
                                            QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes) {
            //qDebug() << "Create Table constr erase:: Yes was clicked";

            erase();

          } else {
            //qDebug() << "Create Table constr erase:: cancel was clicked";
          }

    });


    connect(ui->help_button_2,&QPushButton::clicked,[=]{

        static QPointer <QDialog> check_ref;

        if(!check_ref){

        QString reference_info=":/txt/references_help_2.txt";

        QString info;
        QFile file(reference_info);

            if(file.open(QIODevice::ReadOnly))
                {
                QTextStream in(&file);
            info="<pre style=\"white-space: pre-wrap;\">"+in.readAll()+"</pre>";
            file.close();
            }

        //QMessageBox::about(this,"Referential Actions",info);
            get_help_window(check_ref,"Referential Actions",info);
        } else {
            if(check_ref)check_ref->raise();
        }
    });


    connect(ui->reset_button_2,&QPushButton::clicked,[=]{
        ui->plainTextEdit_2->clear();
        ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText());
    });


    connect(ui->plus_button_2,&QPushButton::clicked,[=]{
        adb_utility::set_cursor_to_end_(this->ui->plainTextEdit_2);

        if(add_keys(ui->plainTextEdit_2)){

            first_key_=false;

        }
    });


    connect(ui->cancel_2,&QPushButton::clicked,[=]{
        this->close();
    });


    connect(ui->back_button_2,&QPushButton::clicked,[=]{

        if(ui->plainTextEdit_2->toPlainText()!=ui->plainTextEdit_1->toPlainText()){
            QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?","This action erases current"
                                                                    " inserted foreign keys for the table. Continue?",
                                            QMessageBox::Yes|QMessageBox::No);
              if (reply != QMessageBox::Yes) {
                //qDebug() << "Create Table constr back_button2 dialog:: Not accepted";
                return;
              } else {
              //qDebug() <<"Create Table constr back_button2 dialog:: Accepted";
              }
        }
//        erase();

        this->setCurrentIndex(1);

        ui->foreign_key_combobox_2->clear();
        ui->foreign_key_checkBox_2->setChecked(false);

    });


    connect(ui->back_button_1,&QPushButton::clicked,[=]{
        if(attributes_added_){
            QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?","This action erases current"
                                                                        " inserted attributes for the table. Continue?",
                                                QMessageBox::Yes|QMessageBox::No);
              if (reply != QMessageBox::Yes) {
                  //qDebug() << "Create Table constr back_button1 dialog:: Not accepted";
               return;
              } else {
                  //qDebug() << "Create Table constr back_button1 dialog:: Accepted";
              }
        }

        erase();
        this->setCurrentIndex(0);

    });


    connect(ui->cancel_0,&QPushButton::clicked,[=]{
        this->close();
    });


    connect(ui->help_button_1,&QPushButton::clicked,[=]{
        //static bool win_open = false;
        static QPointer <QDialog> check_ref;
        if(!/*win_open*/check_ref){
        QString reference_info=":/txt/references_help_1.txt";
//static int i=3;
//qDebug() << &i;
        QString info;
        QFile file(reference_info);

            if(file.open(QIODevice::ReadOnly))
                {
                QTextStream in(&file);
            info="<pre style=\"white-space: pre-wrap;\">"+in.readAll()+"</pre>";
            file.close();
            }

        //QMessageBox::about(this,"Data types",info);


        get_help_window(check_ref,"Data types",info);
        //win_open = true;
        } else {
            if(check_ref)check_ref->raise();
        }

    });


    connect(ui->cancel_1,&QPushButton::clicked,[=]{
        this->close();
    });


    connect(ui->describe_tbl_button_2,&QPushButton::clicked,this,&CreateTableConstructor::describe_table, Qt::QueuedConnection);


    connect(ui->tbl_name_line_0,&QLineEdit::textChanged,statusBar_0->get_line(),&statusLineEdit::clear);

    connect(ui->atr_name_line_1,&QLineEdit::textChanged,statusBar_1->get_line(),&statusLineEdit::clear);
    connect(ui->atr_type_capacity_1,&QLineEdit::textChanged,statusBar_1->get_line(),&statusLineEdit::clear);

    connect(reload_con_button_2,&reloadButton::clicked, this, &CreateTableConstructor::reloadDatabaseComboBox);

}


void CreateTableConstructor::initForm()
{
    ui->setupUi(this);

    ui->page->layout()->addWidget(statusBar_0);
    ui->page_2->layout()->addWidget(statusBar_1);
    ui->page_3->layout()->addWidget(statusBar_2);


    setWindowIcon(QIcon(":/pic/anthead2.png"));

    ui->atr_type_capacity_1->setMaxLength(5);

    statusBar_0->get_line()->setReadOnly(true);
    ui->plainTextEdit_1->setReadOnly(true);
    statusBar_1->get_line()->setReadOnly(true);
    ui->plainTextEdit_2->setReadOnly(true);
    statusBar_2->get_line()->setReadOnly(true);

    //"Help buttons"
    ui->help_button_2->setFixedSize(67,67);
    QRect rect(0,0,67,67);
    QRegion region(rect, QRegion::Ellipse);
    ui->help_button_2->setMask(region);
    ui->help_button_2->setStyleSheet("border: none;"
        "background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #e7ebef, stop: 1 #cde3f2);"
                    "color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #5598c9, stop: 1 #a9d5f5);"
    );

    ui->help_button_1->setFixedSize(67,67);
    rect=QRect(0,0,67,67);
    region=QRegion(rect, QRegion::Ellipse);
    ui->help_button_1->setMask(region);
    ui->help_button_1->setStyleSheet("border: none;"
        "background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #e7ebef, stop: 1 #cde3f2);"
                    "color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #5598c9, stop: 1 #a9d5f5);"
    );

    ui->reloadFrameHorizontalLayout_2->addWidget(reload_con_button_2);

    reload_con_button_2->setEnabled(false);

}



bool CreateTableConstructor::decimal_type_more_or_eq()
{
    int M=0, D=0, numb=0; //M --int part, D -- float part

    bool ok=false;

    QString qs = ui->atr_type_capacity_1->text();

    int length = qs.length();



    if(length>=2){

        numb = (QString(qs.at(0))+QString(qs.at(1))).toInt(&ok);

        if(ok) {
            M=numb;

        } else {

            numb = QString(QChar(qs.at(0))).toInt(&ok);
            if(ok){
                M=numb;
            }

        }

    } else if(length<2){ /*new*/

        numb = QString(QChar(qs.at(0))).toInt(&ok);

        if(ok){
            M=numb;
        }

    }



    if(length==5){
        numb = (QString(qs.at(3))+QString(qs.at(4))).toInt(&ok);

        if(ok){
            D=numb;
        }

    } else if(length==4){
        numb = (QString(qs.at(2))+QString(qs.at(3))).toInt(&ok);

        if(ok){
            D=numb;

        }else {

            numb = (QString(qs.at(3)).toInt(&ok));

            if(ok){
                D = numb;
            }

        }

    } else if(length==3){

        numb = (QString(qs.at(2)).toInt(&ok));

        if(ok){
            D = numb;
        }
    }


    return (M>=D);
}



bool CreateTableConstructor::add_attributes(QPlainTextEdit* textEdit)
{
    // if attribute name is missing
    if(ui->atr_name_line_1->text().isEmpty()){
            statusBar_1->get_line()->setText("Please, insert atribute name.");
        return false;
    } else if(attributes_.length()!=0){

        for(int i=0;i!=attributes_.length();++i){
            if(attributes_.at(i)==ui->atr_name_line_1->text()){
                statusBar_1->get_line()->setText("Atribute with same name already exist.");
                return false;
            }
        }

    }



    QRegularExpression en(".*["+adb_utility:: en_lit+"].*");

    QRegularExpressionValidator v_en(en, 0);

    QString str = ui->atr_name_line_1->text();
    int pos=0;
    if(v_en.validate(str, pos)!=QValidator::Acceptable)
    {
        statusBar_1->get_line()->setText(QString("Attribute name must consist at least 1 [%1] alphabetic character(lower or APPER case).").arg(adb_utility:: en_lit));
        return false;

    } else {

        QRegularExpression fi_v("^["+adb_utility:: en_lit+adb_utility:: digits_lit+adb_utility:: spec_chars_lit+"]*$");
        QRegularExpressionValidator v(fi_v, 0);
        QString str = ui->atr_name_line_1->text();
        int pos=0;
        if(v.validate(str, pos)!=QValidator::Acceptable)
        {
            statusBar_1->get_line()->setText(QString("Incorrect symbols in attribute name. Please use low and upper [%1] letters, digits or special characters [%2].").arg(adb_utility:: en_lit).arg(adb_utility:: spec_chars_lit));
            return false;
        }

    }



    // if attribute type is missing
        if(ui->atr_type_comboBox_1->currentText().isEmpty()){
            statusBar_1->get_line()->setText("Please, choose atribute type.");
        return false;
        }
    // if attribute type is "VARCHAR" --> need capacity
        if(ui->atr_type_comboBox_1->currentText()=="VARCHAR"){

            if(ui->atr_type_capacity_1->text().isEmpty()){
                statusBar_1->get_line()->setText("Please, choose length of string(VARCHAR) attribute.");
                return false;

            } else {

                QString str=ui->atr_type_capacity_1->text();
                int pos = 0;
                QIntValidator v(0, 16383, this); // INTEGER VALIDATON (SQL's VARCHAR MAX CAPACITY)

                if(v.validate(str,pos)!=QValidator::Acceptable){

                    statusBar_1->get_line()->setText("Please, choose correct length of string(VARCHAR) attribute.");
                    return false;
                }

            }

        } /*else if(ui->atr_type_comboBox_1->currentText()=="YEAR"){

            if((!ui->atr_type_capacity_1->text().isEmpty())&&(ui->atr_type_capacity_1->text().toInt()!=2)
                    &&(ui->atr_type_capacity_1->text().toInt()!=4)){

                statusBar_1->get_line()->setText("Please, choose correct YEAR attribute format (4 or 2).");
                return false;
            }

        }*/ else if(ui->atr_type_comboBox_1->currentText()=="DECIMAL"){

                if(!ui->atr_type_capacity_1->text().isEmpty()){

                    QString str = ui->atr_type_capacity_1->text();

                    QRegularExpression re("([0-9]|[1-5][0-9]|6[0-5])|(([0-9]|[1-5][0-9]|6[0-5]),([0-9]|[1-2][0-9]|3[0-8]))");

                    QRegularExpressionValidator v(re, 0);

                    int pos=0;

                    if(v.validate(str, pos)!=QValidator::Acceptable){

                        /*new*/if(str.contains('.')){
                                   statusBar_1->get_line()->setText("Use `,` instead of `.` to separate integer and float part of number.");
                                                           return false;
                               }

                        statusBar_1->get_line()->setText("Please, choose correct DECIMAL type parameters (or leave empty).");
                        return false;

                    } else {
                        // M >= D

                        if(!decimal_type_more_or_eq()){
                            statusBar_1->get_line()->setText("For decimal(M,D), M must be >= D. Please, choose correct DECIMAL type parameters (or leave empty).");
                                                    return false;
                        }



                    }

                }

            } else if(ui->atr_type_comboBox_1->currentText()=="INT"){

                if(!ui->atr_type_capacity_1->text().isEmpty()){

                    QRegularExpression re("([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])");

                    QRegularExpressionValidator v(re, 0);

                    QString str = ui->atr_type_capacity_1->text();

                    int pos=0;

                    if(v.validate(str, pos)!=QValidator::Acceptable){

                    statusBar_1->get_line()->setText("Please, choose correct INT type parameters (or leave empty).");
                    return false;
                    }

                }

        } else if(ui->atr_type_comboBox_1->currentText()=="YEAR"){

            if(!ui->atr_type_capacity_1->text().isEmpty()){
                bool ok;
                /*int numb = */ui->atr_type_capacity_1->text().toInt(&ok);
                if(!ok) {
                    statusBar_1->get_line()->setText("Please, choose correct YEAR type parameters (or leave empty).");
                    return false;
                }

            }

        }



        adb_utility::set_cursor_to_end_(this->ui->plainTextEdit_1);

        //attributes_.append(QString("`%1`").arg(ui->atr_name_line_1->text()));
        attributes_.append(ui->atr_name_line_1->text());

        if(!first_attribute_)
        textEdit->insertPlainText(",");

        textEdit->insertPlainText('`'+ui->atr_name_line_1->text()+"` "+ui->atr_type_comboBox_1->currentText());

    if(ui->atr_type_comboBox_1->currentText()=="VARCHAR"||(ui->atr_type_comboBox_1->currentText()=="DECIMAL"&&(!ui->atr_type_capacity_1->text().isEmpty()))
            ||(ui->atr_type_comboBox_1->currentText()=="INT"&&(!ui->atr_type_capacity_1->text().isEmpty()))
            ||(ui->atr_type_comboBox_1->currentText()=="YEAR"&&(!ui->atr_type_capacity_1->text().isEmpty()))
                        )
                        textEdit->insertPlainText('('+ui->atr_type_capacity_1->text()+')');
    else textEdit->insertPlainText(" ");

    if(ui->primary_key_checkBox_1->isChecked())
                        textEdit->insertPlainText(" PRIMARY KEY ");
    if(ui->AUTO_INCREMENT_checkBox_1->isChecked())
                        textEdit->insertPlainText(" AUTO_INCREMENT ");
    if(ui->NOT_NULL_checkBox_1->isChecked())
                        textEdit->insertPlainText(" NOT NULL ");

    return true;
}



bool CreateTableConstructor::add_keys(QPlainTextEdit *textEdit)
{

    if(ui->foreign_key_checkBox_2->isChecked()){

        if(ui->foreign_key_combobox_2->currentText().isEmpty()){
            statusBar_2->get_line()->setText("Please, select foreign key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        } else if(ui->ref_DB_comboBox_2->currentText().isEmpty()){
            statusBar_2->get_line()->setText("Please, select reference DB for key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        } else if(ui->ref_table_comboBox_2->currentText().isEmpty()){
            statusBar_2->get_line()->setText("Please, select reference table for key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        } else if(ui->ref_key_comboBox_2->currentText().isEmpty()){
            statusBar_2->get_line()->setText("Please, select reference attribute for key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        }

//        if(!first_key_)
    textEdit->insertPlainText(", ");

    textEdit->insertPlainText(QString(" FOREIGN KEY (`%1`)").arg(QString/*(escape_sql_backticks*/(ui->foreign_key_combobox_2->currentText()/*)*/)));

    textEdit->insertPlainText(QString(" REFERENCES `%1`.`%2`(`%3`)").arg(QString(adb_utility::escape_sql_backticks(ui->ref_DB_comboBox_2->currentText())))
                              .arg(QString(adb_utility::escape_sql_backticks(ui->ref_table_comboBox_2->currentText())))
                              .arg(QString(adb_utility::escape_sql_backticks(ui->ref_key_comboBox_2->currentText()))));


    if(ui->on_delete_checkBox_2->isChecked())
    textEdit->insertPlainText(" ON DELETE "+ui->onDelete_comboBox_2->currentText());

    if(ui->on_update_checkBox_2->isChecked())
    textEdit->insertPlainText(" ON UPDATE "+ui->onUpdate_comboBox_2->currentText());


    }


    return true;
}



void CreateTableConstructor::AttrTypeChanged(const QString &type_str)
{
    if(type_str=="INT")
        ui->AUTO_INCREMENT_checkBox_1->setEnabled(true);
    else{
        ui->AUTO_INCREMENT_checkBox_1->setEnabled(false);
        ui->AUTO_INCREMENT_checkBox_1->setChecked(false);
    }

           if(type_str=="INT"){
        ui->atr_type_capacity_1->setEnabled(true);
        ui->atr_type_hint_1->setText("INT(M) -- you may specify a number width (0-255).");
        ui->atr_type_capacity_1->setPlaceholderText("255");
    } else if(type_str=="DECIMAL"){
        ui->atr_type_capacity_1->setEnabled(true);
        ui->atr_type_hint_1->setText("DECIMAL(M,D) -- you may specify a (M) -- max. number of"
                                     " digits(1-65); and (D) -- number of digits to the right"
                                     " of decimal point(0-38). [M>=D]");
        ui->atr_type_capacity_1->setPlaceholderText("65,38");
    } else if(type_str=="VARCHAR"){
        ui->atr_type_capacity_1->setEnabled(true);
        ui->atr_type_hint_1->setText("VARCHAR(M) -- you MUST define a length of string (0-16383).");
        ui->atr_type_capacity_1->setPlaceholderText("16383");
    } else if(type_str=="YEAR"){
        ui->atr_type_capacity_1->setEnabled(true);
        ui->atr_type_hint_1->setText("YEAR(M) -- you may specify year in 2-digit or a"
                                    " 4-digit (by default) format.");
        ui->atr_type_capacity_1->setPlaceholderText("4");
    }

    else{
        ui->atr_type_capacity_1->setEnabled(false);
        ui->atr_type_hint_1->setText("");
        ui->atr_type_capacity_1->setPlaceholderText("");
    }
           ui->atr_type_hint_1->adjustSize();
}


void CreateTableConstructor::foreignkeychecked(int state)
{
    if(!state)
    {
        ui->foreign_key_combobox_2->setEnabled(false);
        ui->ref_DB_comboBox_2->setEnabled(false);
        ui->ref_table_comboBox_2->setEnabled(false);
        ui->ref_key_comboBox_2->setEnabled(false);

        ui->on_delete_checkBox_2->setEnabled(false);
        ui->on_update_checkBox_2->setEnabled(false);
        ui->onDelete_comboBox_2->setEnabled(false);
        ui->onUpdate_comboBox_2->setEnabled(false);

        ui->describe_tbl_button_2->setEnabled(false);

        ui->plus_button_2->setEnabled(false);

        reload_con_button_2->setEnabled(false);

    } else {

        reloadDatabaseComboBox();

        ui->foreign_key_combobox_2->setEnabled(true);
        ui->ref_DB_comboBox_2->setEnabled(true);
        ui->ref_table_comboBox_2->setEnabled(true);
        ui->ref_key_comboBox_2->setEnabled(true);

        ui->on_delete_checkBox_2->setEnabled(true);
        ui->on_update_checkBox_2->setEnabled(true);

        if(ui->on_delete_checkBox_2->isChecked())
            ui->onDelete_comboBox_2->setEnabled(true);

        if(ui->on_update_checkBox_2->isChecked())
            ui->onUpdate_comboBox_2->setEnabled(true);

        ui->describe_tbl_button_2->setEnabled(true);

        ui->plus_button_2->setEnabled(true);

        reload_con_button_2->setEnabled(true);

    }
}



void CreateTableConstructor::closeEvent(QCloseEvent *event)
{
    if(_warning_flag_){
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to close constructor window?",
                                                             QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No) {
        //qDebug() << "Create Table constr close:: cancel closing";
        event->ignore();
        return;
      } else {
        //qDebug() << "Create Table constr close:: closing accepted";
      }
    }

    emit closed();


    db_connection::close(subconnection_name_);

    erase();

    ui->foreign_key_combobox_2->clear();
    ui->foreign_key_checkBox_2->setChecked(false);

    current_t_name_.clear();
    ui->tbl_name_line_0->clear();
    statusBar_0->get_line()->clear();


    event->accept();
}



void CreateTableConstructor::current_exist_tables_slot(QList<QString> list_)
{
    ////qDebug() << "CURRENT EXIST TABLES IN THIS DB::" << list_;
    exist_table_names_.clear();
    exist_table_names_=list_;
    ////qDebug() << "LIST OF STRING FINALE::"<<exist_table_names_;
}


void CreateTableConstructor::constructor_query_fails_handle()
{
    this->statusBar_2->get_line()->setText(QString("(x)`Create table constructor` query fails. Table `%1` is not created.").arg(current_t_name_));
}


void CreateTableConstructor::erase()
{
    ui->plainTextEdit_1->clear();
    ui->plainTextEdit_1->insertPlainText("CREATE TABLE `"+ui->tbl_name_line_0->text()+"` ( ");
    first_attribute_=true;
    attributes_.clear();
    statusBar_1->get_line()->clear();
    ui->number_line_1->setText(QString::number(0));
    attributes_added_=0;
}



void CreateTableConstructor::on_next_0_clicked()
{
    if(ui->tbl_name_line_0->text().isEmpty()){
        statusBar_0->get_line()->setText("Table name must not be empty.");
        return;

     } else {

        QRegularExpression en(".*["+adb_utility:: en_lit+"].*");//(".*[a-zA-Z].*");

        QRegularExpressionValidator v_en(en, 0);

        QString str = ui->tbl_name_line_0->text();
        int pos=0;
        if(v_en.validate(str, pos)!=QValidator::Acceptable)
        {
            statusBar_0->get_line()->setText(QString("Name must consist at least 1 [%1] alphabetic character(lower or APPER case).").arg(adb_utility:: en_lit));
            return;

        } else{

            QRegularExpression fi_v("^["+adb_utility:: en_lit/*+ru_lit+ukr_lit*/+adb_utility:: digits_lit+adb_utility:: spec_chars_lit+"]*$");
            QRegularExpressionValidator v(fi_v, 0);
            QString str = ui->tbl_name_line_0->text();
            int pos=0;
            if(v.validate(str, pos)!=QValidator::Acceptable)
            {
                statusBar_0->get_line()->setText(QString("Incorrect symbols in table's name. Please use low and upper [%1] letters, digits or special characters [%2].").arg(adb_utility:: en_lit).arg(adb_utility:: spec_chars_lit));
                return;
            }

        }

        size_t list_counter = exist_table_names_.size();
        for(size_t i=0;i!=list_counter;++i){
            if(exist_table_names_.at(i)==ui->tbl_name_line_0->text()){
                statusBar_0->get_line()->setText("Table with this name already exist in this database. Please, choose another name and"
                                            " try again.");
                return;
            }
        }

        current_t_name_ = ui->tbl_name_line_0->text();

        ui->plainTextEdit_1->clear();
        ui->plainTextEdit_1->insertPlainText("CREATE TABLE `"+/*ui->tbl_name_line_0->text()*/current_t_name_+"` ( ");
        ////ui->plainTextEdit_1->insertPlainText(QString("CREATE TABLE `%1` ( ").arg(QString(escape_sql_backticks(ui->tbl_name_line_0->text()))));
            this->setCurrentIndex(1);
    }

}




void CreateTableConstructor::describe_table()
{

    if(!describe_form_.isNull())//{
        describe_form_.reset();


    QString const db_name = ui->ref_DB_comboBox_2->currentText();
    QString const table_name = ui->ref_table_comboBox_2->currentText();
    db_connection::close(subconnection_name_2_); //1

    auth* __auth {nullptr}; // not stack obj because 'customqueryresult' gets reference of auth, not value (without additional exec() local stack obj out-of-range ==>> UB (crashing app)
qDebug() << (__auth==nullptr);
    __auth = new auth{auth_};


    __auth->db_name_ = db_name;


    QSqlDatabase::database(subconnection_name_2_,false).setDatabaseName(db_name); //2


    describe_form_.reset(new CustomQueryResult{{*__auth}/*,this*//*0*/});



    auto con1 = connect(this,&CreateTableConstructor::closed,this,[=](){

        if(!describe_form_.isNull()){

            describe_form_->close();

        }


    },Qt::QueuedConnection);





    connect(describe_form_.data(),&CustomQueryResult::destroyed,[=]{
        delete __auth;

        //describe_form_.reset();

        db_connection::close(subconnection_name_2_);

        disconnect(con1); //disconnect signal connection (not SQLDB connection)

    });


describe_form_->setWindowTitle(table_name);

describe_form_->custom_query_slot(QString("DESCRIBE `%1`").arg(QString(adb_utility::escape_sql_backticks(table_name))),subconnection_name_2_); //3

//describe_form_-> setAttribute( Qt::WA_DeleteOnClose, true );

describe_form_->setModal(false);

describe_form_->show();


////describe_form_-> exec(); // preventing the 'auth__' stack temporary object out-of-scope destroyed (crashing the app because 'CustomQueryResult' holds its reference)


}


void CreateTableConstructor::reloadDatabaseComboBox()
{
    if(!db_connection::open(auth_)){
        qDebug() << QString("(x)There is error while update database list (connection is not established).");
        return;
    }

    // preventing load tables in ref_table comboBox until current DB was choosed
    ui->ref_DB_comboBox_2->blockSignals(true);

    db_connection::set_query("SHOW DATABASES;", &submodel_0_,ui->ref_DB_comboBox_2/*,multi_con_*//*auth_.con_name_,1*/);


    size_t size_of_list_=ui->ref_DB_comboBox_2->model()->rowCount();


    for(size_t i=0;i!=size_of_list_;++i){
        if(ui->ref_DB_comboBox_2->model()->index(i,0).data().toString()==auth_.db_name_){ // SET CURRENT DB AS DEFAULT IN COMBOBOX
            ui->ref_DB_comboBox_2->blockSignals(false);
            QApplication::processEvents();
            ui->ref_DB_comboBox_2->/*setCurrentText(ui->ref_DB_comboBox_2->model()->index(i,0).data().toString());//*/setCurrentIndex(i);
            ui->ref_DB_comboBox_2->currentIndexChanged(i);
            break; // return or break?
        }
    }

    if(ui->ref_DB_comboBox_2->signalsBlocked())
        ui->ref_DB_comboBox_2->blockSignals(false);
    //ui->ref_DB_comboBox_2->setCurrentIndex(-1);

}



void CreateTableConstructor::get_help_window(QPointer<QDialog>&window_pointer,QString const & header_title__,const QString & info__,QWidget*parent__)
{
    window_pointer = new QDialog{parent__};
    window_pointer->setWindowTitle(header_title__);

    auto layout = new QVBoxLayout;

    auto label = new QLabel;
    label->setText(info__);
    label->setWordWrap(true);

    auto scroller = new QScrollArea;
    scroller->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroller->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroller->setWidgetResizable(true);
    scroller->setWidget(label);



    auto buttonBox = new QDialogButtonBox{QDialogButtonBox::Ok};
    connect(buttonBox, &QDialogButtonBox::accepted, window_pointer, &QDialog::accept);
    buttonBox->setCenterButtons(true);
    window_pointer->setLayout(layout);


    layout->addWidget(scroller);

    layout->addWidget(buttonBox);

    connect(window_pointer,&QDialog::destroyed,[&](){

    });
    window_pointer->setAttribute( Qt::WA_DeleteOnClose, true );
    window_pointer->show();



}


