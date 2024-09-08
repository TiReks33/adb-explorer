#include "create_table_constructor.h"
#include "ui_create_table_constructor.h"
#include "tables.h"

QDebug operator<<(QDebug stream__, auth const &auth__){
    stream__ << "db_server::" << auth__.db_server_
             << "host::" << auth__.host_
             << "db_name::" << auth__.db_name_
             << "table_name::" << auth__.table_name_;
    return stream__;
}

CreateTableConstructor::CreateTableConstructor(auth& auth__,/*QWidget*/Tables *parent) :
    QStackedWidget(parent),
    ui(new Ui::CreateTableConstructor)
  , first_attribute_(true)
  , sql_query_("")
  , first_key_(true)
  , auth_(auth__)

  , parent_(parent)

{
    ui->setupUi(this);


    ui->atr_type_capacity_1->setMaxLength(5);

    ui->statusLine_0->setReadOnly(true);
    ui->plainTextEdit_1->setReadOnly(true);
    ui->statusLine_1->setReadOnly(true);
    ui->plainTextEdit_2->setReadOnly(true);
    ui->statusLine_2->setReadOnly(true);

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

    //SIGNALs

            signals_init();

}


CreateTableConstructor::~CreateTableConstructor()
{
    delete ui;

    db_connection::close(subconnection_name_);
    db_connection::remove(subconnection_name_);
}


void CreateTableConstructor::signals_init()
{
    connect(ui->atr_type_comboBox_1,SIGNAL(currentTextChanged(QString const&)),this,SLOT(AttrTypeChanged(QString const&)));

    connect(ui->foreign_key_checkBox_2,SIGNAL(stateChanged(int)),this,SLOT(foreignkeychecked(int)));

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


    QObject::connect(ui->ref_DB_comboBox_2,&QComboBox::currentTextChanged, this, [=](QString const&current_DB_name__){

            //submodel_2_.clear();

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
                    qDebug()<<"submodel_2_ cleared;;";

                    ui->ref_table_comboBox_2->setToolTip("*empty table*");
                    ui->ref_key_comboBox_2->setToolTip("*no corteges (empty table)*");

                }
            }
    },Qt::QueuedConnection);


    connect(ui->ref_table_comboBox_2,&QComboBox::currentTextChanged, this,[=](QString const&current_table_name__){

            qDebug() << "Current table::"<<current_table_name__;

            db_connection::set_query(QString("SHOW COLUMNS FROM `%1`").arg(QString(escape_sql_backticks(current_table_name__))), &submodel_2_,ui->ref_key_comboBox_2,subconnection_name_);

            ui->ref_table_comboBox_2->setToolTip(current_table_name__);

    },Qt::QueuedConnection);


    connect(ui->ref_key_comboBox_2,&QComboBox::currentTextChanged,this,[=](QString const&current_key_name__){
       ui->ref_key_comboBox_2->setToolTip(current_key_name__);
    },Qt::QueuedConnection);



    connect(ui->send_button,&QPushButton::clicked,[=]{
        set_cursor_to_end_(this->ui->plainTextEdit_2);

        ui->plainTextEdit_2->insertPlainText(" );");
        qDebug() << "SQL FINAL QUERY::" << ui->plainTextEdit_2->toPlainText();

        emit send_custom_query(ui->plainTextEdit_2->toPlainText());
    });


    connect(ui->next_1,&QPushButton::clicked,[=]{
        if(!first_attribute_){
            ui->plainTextEdit_2->clear();
            ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText()/*+')'*/);


            ui->foreign_key_combobox_2->addItems(attributes_);
            ui->foreign_key_combobox_2->setCurrentIndex(-1);


            //on_reload_con_button_2_clicked();


            this->setCurrentIndex(2);

        } else {
            ui->statusLine_1->setText("Please, add 1 or more attributes to current table before next step.");
        }

    qDebug()<<"END";
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
            qDebug() << "Create Table constr erase:: Yes was clicked";

            erase();

          } else {
            qDebug() << "Create Table constr erase:: cancel was clicked";
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
        set_cursor_to_end_(this->ui->plainTextEdit_2);

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
                qDebug() << "Create Table constr back_button2 dialog:: Not accepted";
                return;
              } else {
              qDebug() <<"Create Table constr back_button2 dialog:: Accepted";
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
                  qDebug() << "Create Table constr back_button1 dialog:: Not accepted";
               return;
              } else {
                  qDebug() << "Create Table constr back_button1 dialog:: Accepted";
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


}



bool CreateTableConstructor::decimal_type_more_or_eq()
{
    int M=0;
    int D=0;
    bool ok;
    QString qs = ui->atr_type_capacity_1->text();
    int length = qs.length();


    int numb1 = (QString(qs.at(0))+QString(qs.at(1))).toInt(&ok);
    if(ok) {
        M=numb1;
    } else {

        int numb1at = QString(QChar(qs.at(0))).toInt(&ok);
        if(ok){
            M=numb1at;
        }
    }

    int numb2=0;
    if(length==5){
        numb2 = (QString(qs.at(3))+QString(qs.at(4))).toInt(&ok);

        if(ok){
            D=numb2;
        }

    } else if(length==4){
        numb2 = (QString(qs.at(2))+QString(qs.at(3))).toInt(&ok);

        if(ok){
            D=numb2;
        } else {
            int numb2at = (QString(qs.at(3)).toInt(&ok));

        D=numb2at;

        }

    } else if(length==3){
        numb2 = (QString(qs.at(2)).toInt(&ok));

    D=numb2;

    }

    qDebug() << M;
    qDebug() << D;
    qDebug() << "M>=D" << (M>=D);
    return (M>=D);
}



bool CreateTableConstructor::add_attributes(QPlainTextEdit* textEdit)
{
    // if attribute name is missing
    if(ui->atr_name_line_1->text().isEmpty()){
            ui->statusLine_1->setText("Please, insert atribute name.");
        return false;
    } else if(attributes_.length()!=0){

        for(int i=0;i!=attributes_.length();++i){
            if(attributes_.at(i)==ui->atr_name_line_1->text()){
                ui->statusLine_1->setText("Atribute with same name already exist.");
                return false;
            }
        }

    }



    QRegularExpression en(".*["+en_lit+"].*");

    QRegularExpressionValidator v_en(en, 0);

    QString str = ui->atr_name_line_1->text();
    int pos=0;
    if(v_en.validate(str, pos)!=QValidator::Acceptable)
    {
        ui->statusLine_1->setText(QString("Attribute name must consist at least 1 [%1] alphabetic character(lower or APPER case).").arg(en_lit));
        return false;
    } else {

        QRegularExpression fi_v("^["+en_lit+digits_lit+spec_chars_lit+"]*$");
        QRegularExpressionValidator v(fi_v, 0);
        QString str = ui->atr_name_line_1->text();
        int pos=0;
        if(v.validate(str, pos)!=QValidator::Acceptable)
        {
            ui->statusLine_1->setText(QString("Incorrect symbols in attribute name. Please use low and upper [%1] letters, digits or special characters [%2].").arg(en_lit).arg(spec_chars_lit));
            return false;
        }

    }



    // if attribute type is missing
        if(ui->atr_type_comboBox_1->currentText().isEmpty()){
            ui->statusLine_1->setText("Please, choose atribute type.");
        return false;
        }
    // if attribute type is "VARCHAR" --> need capacity
        if(ui->atr_type_comboBox_1->currentText()=="VARCHAR"){
            if(ui->atr_type_capacity_1->text().isEmpty()){
                ui->statusLine_1->setText("Please, choose length of string(VARCHAR) attribute.");
            return false;
            } else {

            QString str=ui->atr_type_capacity_1->text();
            int pos = 0;
            QIntValidator v(0, 16383, this); // INTEGER VALIDATON (SQL's VARCHAR MAX CAPACITY)

            if(v.validate(str,pos)!=QValidator::Acceptable){

                ui->statusLine_1->setText("Please, choose correct length of string(VARCHAR) attribute.");
                return false;
            }

            }

        } /*else if(ui->atr_type_comboBox_1->currentText()=="YEAR"){

            if((!ui->atr_type_capacity_1->text().isEmpty())&&(ui->atr_type_capacity_1->text().toInt()!=2)
                    &&(ui->atr_type_capacity_1->text().toInt()!=4)){

                ui->statusLine_1->setText("Please, choose correct YEAR attribute format (4 or 2).");
                return false;
            }

        }*/ else if(ui->atr_type_comboBox_1->currentText()=="DECIMAL"){

            if(!ui->atr_type_capacity_1->text().isEmpty()){


                QRegularExpression re("([0-9]|[1-5][0-9]|6[0-5])|(([0-9]|[1-5][0-9]|6[0-5]),([0-9]|[1-2][0-9]|3[0-8]))");

                QRegularExpressionValidator v(re, 0);

                QString str = ui->atr_type_capacity_1->text();

                int pos=0;

                if(v.validate(str, pos)!=QValidator::Acceptable){

                ui->statusLine_1->setText("Please, choose correct DECIMAL type parameters (or leave empty).");
                return false;
                } else {
                    // M >= D

                    if(!decimal_type_more_or_eq()){
                        ui->statusLine_1->setText("For decimal(M,D), M must be >= D. Please, choose correct DECIMAL type parameters (or leave empty).");
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

                ui->statusLine_1->setText("Please, choose correct INT type parameters (or leave empty).");
                return false;
                }

            }

        } else if(ui->atr_type_comboBox_1->currentText()=="YEAR"){

            if(!ui->atr_type_capacity_1->text().isEmpty()){
                bool ok;
                /*int numb = */ui->atr_type_capacity_1->text().toInt(&ok);
                if(!ok) {
                    ui->statusLine_1->setText("Please, choose correct YEAR type parameters (or leave empty).");
                    return false;
                        }

                    }

        }



        set_cursor_to_end_(this->ui->plainTextEdit_1);

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
            ui->statusLine_2->setText("Please, select foreign key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        } else if(ui->ref_DB_comboBox_2->currentText().isEmpty()){
            ui->statusLine_2->setText("Please, select reference DB for key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        } else if(ui->ref_table_comboBox_2->currentText().isEmpty()){
            ui->statusLine_2->setText("Please, select reference table for key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        } else if(ui->ref_key_comboBox_2->currentText().isEmpty()){
            ui->statusLine_2->setText("Please, select reference attribute for key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        }

//        if(!first_key_)
    textEdit->insertPlainText(", ");

    textEdit->insertPlainText(QString(" FOREIGN KEY (`%1`)").arg(QString/*(escape_sql_backticks*/(ui->foreign_key_combobox_2->currentText()/*)*/)));

    textEdit->insertPlainText(QString(" REFERENCES `%1`.`%2`(`%3`)").arg(QString(escape_sql_backticks(ui->ref_DB_comboBox_2->currentText())))
                              .arg(QString(escape_sql_backticks(ui->ref_table_comboBox_2->currentText())))
                              .arg(QString(escape_sql_backticks(ui->ref_key_comboBox_2->currentText()))));


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
    } else {

        on_reload_con_button_2_clicked();

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
    }
}



void CreateTableConstructor::closeEvent(QCloseEvent *event)
{
    if(_warning_flag_){
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to close constructor window?",
                                                             QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No) {
        qDebug() << "Create Table constr close:: cancel closing";
        event->ignore();
        return;
      } else {
        qDebug() << "Create Table constr close:: closing accepted";
      }
    }

    emit closed();


    db_connection::close(subconnection_name_);

    erase();

    ui->foreign_key_combobox_2->clear();
    ui->foreign_key_checkBox_2->setChecked(false);

    ui->tbl_name_line_0->clear();
    ui->statusLine_0->clear();

    //parent_->show();

    event->accept();
}



void CreateTableConstructor::current_exist_tables_slot(QList<QString> list_)
{
    qDebug() << "CURRENT EXIST TABLES IN THIS DB::" << list_;
    exist_table_names_.clear();
    exist_table_names_=list_;
    qDebug() << "LIST OF STRING FINALE::"<<exist_table_names_;
}


void CreateTableConstructor::constructor_query_fails_handle()
{
    this->ui->statusLine_2->setText("(x)Table constructor query fails.");
}


void CreateTableConstructor::erase()
{
    ui->plainTextEdit_1->clear();
    ui->plainTextEdit_1->insertPlainText("CREATE TABLE `"+ui->tbl_name_line_0->text()+"` ( ");
    first_attribute_=true;
    attributes_.clear();
    ui->statusLine_1->clear();
    ui->number_line_1->setText(QString::number(0));
    attributes_added_=0;
}



void CreateTableConstructor::on_next_0_clicked()
{
    if(ui->tbl_name_line_0->text().isEmpty()){
        ui->statusLine_0->setText("Table name must not be empty.");
        return;
    } else {
//        QString const en_lit = "a-zA-Z";
  ////      QString const ru_lit = "ЁёА-я";
   ////     QString const ukr_lit = "А-ЩЬЮЯҐЄІЇа-щьюяґєії";
        QRegularExpression en(".*["+en_lit+"].*");//(".*[a-zA-Z].*");
 ////       QRegularExpression ru(".*["+ru_lit+"].*");//(".*[ЁёА-я].*");
  ////      QRegularExpression ukr(".*["+ukr_lit+"].*");//(".*[А-ЩЬЮЯҐЄІЇа-щьюяґєії'].*");//(".*[А-ЩЬЮЯҐЄІЇа-щьюяґєії'`’ʼ].*");
        ////^[А-ЩЬЮЯҐЄІЇ][а-щьюяґєії']*$ //!!!!
        QRegularExpressionValidator v_en(en, 0);
 ////       QRegularExpressionValidator v_ru(ru,0);
  ////      QRegularExpressionValidator v_ukr(ukr,0);
        QString str = ui->tbl_name_line_0->text();
        int pos=0;
        if(v_en.validate(str, pos)!=QValidator::Acceptable
//                && v_ru.validate(str, pos)!=QValidator::Acceptable
//                && v_ukr.validate(str, pos)!=QValidator::Acceptable
                )
        {
            ui->statusLine_0->setText(QString("Name must consist at least 1 [%1] alphabetic character(lower or APPER case).").arg(en_lit));
            return;
        } else{
            //"^[a-zA-Z0-9_.-]*$" -- ok
            ////QRegularExpression re("^[a-zA-Z0-9_]*$");
//            QString const digits_lit = "0-9";
//            QString const spec_chars_lit = "_$";

//            QRegularExpression fi_v("^"+en_lit+ru_lit+ukr_lit+digits_lit+spec_chars_lit+"*$");
            QRegularExpression fi_v("^["+en_lit/*+ru_lit+ukr_lit*/+digits_lit+spec_chars_lit+"]*$");
            QRegularExpressionValidator v(fi_v, 0);
            QString str = ui->tbl_name_line_0->text();
            int pos=0;
            if(v.validate(str, pos)!=QValidator::Acceptable)
            {
                ui->statusLine_0->setText(QString("Incorrect symbols in table's name. Please use low and upper [%1] letters, digits or special characters [%2].").arg(en_lit).arg(spec_chars_lit));
                return;
            }

        }

        size_t list_counter = exist_table_names_.size();
        for(size_t i=0;i!=list_counter;++i){
            if(exist_table_names_.at(i)==ui->tbl_name_line_0->text()){
                ui->statusLine_0->setText("Table with this name already exist in this database. Please, choose another name and"
                                            " try again.");
                return;
            }
        }

        ui->plainTextEdit_1->clear();
        ui->plainTextEdit_1->insertPlainText("CREATE TABLE `"+ui->tbl_name_line_0->text()+"` ( ");
        ////ui->plainTextEdit_1->insertPlainText(QString("CREATE TABLE `%1` ( ").arg(QString(escape_sql_backticks(ui->tbl_name_line_0->text()))));
            this->setCurrentIndex(1);
    }

}




void CreateTableConstructor::describe_table()
{
    qDebug() << "describe form == nullptr::" <<(describe_form_==nullptr);
    if(describe_form_!=nullptr)//{
        describe_form_->close();


    QString const db_name = ui->ref_DB_comboBox_2->currentText();
    QString const table_name = ui->ref_table_comboBox_2->currentText();
    db_connection::close(subconnection_name_2_); //1

    auth* __auth {new auth{auth_}}; // not static obj because 'customqueryresult' gets reference (without additional exec() local stack obj out-of-range ==>> UB (crashing app)

    __auth->db_name_ = db_name;


    QSqlDatabase::database(subconnection_name_2_,false).setDatabaseName(db_name); //2


    describe_form_.reset(new CustomQueryResult{{*__auth}/*,this*//*0*/});
    //describe_form_ = new CustomQueryResult{__auth,/*this*//*0*/nullptr};
    //describe_form_ = new CustomQueryResult{__auth,/*this*//*0*//*nullptr*/qobject_cast<Tables *>(this->parent())};
    //describe_form_ = new CustomQueryResult{__auth,/*this*//*0*//*nullptr*/parent_};
    //QPointer<Databases> grand_parent = qobject_cast<Databases *>(dump_auth_choose->parent());


    auto con1 = connect(this,&CreateTableConstructor::closed,this,[=](){
        qDebug() << "Before closing describe_form";
        if(describe_form_!=nullptr){

            describe_form_->close();

        }

        qDebug() << "After closing describe_form";
    },Qt::QueuedConnection);


//    auto con2 = connect(ui->pushButton,&QPushButton::clicked,[=]{

//        if(describe_form_!=nullptr) qDebug()<<describe_form_->auth_;

//    });


//    connect(describe_form_.data(),&CustomQueryResult::destroyed,this,[=](){ /*describe_form_ = nullptr;qDebug()<<"OBJISNULLED!1";*/
//        db_connection::close(subconnection_name_2_);

//        disconnect(con1);
//        disconnect(con2);
//        //delete __auth;
//    });


    connect(describe_form_.data(),&CustomQueryResult::closed_,[=]{
        delete __auth;
        describe_form_.reset();

        db_connection::close(subconnection_name_2_);

        disconnect(con1);
//        disconnect(con2);
    });


describe_form_->setWindowTitle(table_name);

describe_form_->custom_query_slot(QString("DESCRIBE `%1`").arg(QString(escape_sql_backticks(table_name))),subconnection_name_2_); //3

describe_form_-> setAttribute( Qt::WA_DeleteOnClose, true );

describe_form_->setModal(false);

describe_form_->show();


//describe_form_-> exec(); // preventing the 'auth__' stack temporary object out-of-scope destroyed (crashing the app because 'CustomQueryResult' holds its reference)


}


void CreateTableConstructor::on_reload_con_button_2_clicked()
{
    if(!db_connection::open(auth_)){
        qDebug() << QString("(x)There is error while update tables (connection is not established).");
        return;
    }

    db_connection::set_query("SHOW DATABASES;", &submodel_0_,ui->ref_DB_comboBox_2/*,multi_con_*//*auth_.con_name_,1*/);


    size_t size_of_list_=ui->ref_DB_comboBox_2->model()->rowCount();

    qDebug() << "NUMBER OF TABLES::" << size_of_list_;

    //qDebug() << "CURRENT TEXT::"<<ui->tableView->model()->index(1,0).data().toString();
    for(size_t i=0;i!=size_of_list_;++i){
        if(ui->ref_DB_comboBox_2->model()->index(i,0).data().toString()==auth_.db_name_){ // SET CURRENT DB AS DEFAULT
            ui->ref_DB_comboBox_2->setCurrentIndex(i);                                        // IN COMBOBOX
            break; // return or break?
        }
    }
}


void CreateTableConstructor::get_help_window(QString const & header_title__,const QString & info__,QWidget*parent__)
{
    //QMessageBox::about(this,"Data types",info);
    //static QPointer <QDialog> dialog;

////static bool win_open = false;
////    if(!win_open){
    QPointer <QDialog> dialog = new QDialog{parent__};
    dialog->setWindowTitle(header_title__);

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
    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
buttonBox->setCenterButtons(true);
    dialog->setLayout(layout);


    layout->addWidget(scroller);

    layout->addWidget(buttonBox);

    connect(dialog,&QDialog::destroyed,[&](){
        qDebug() << "~help dialog activated (destroyed).";
////        win_open=false;
    });
    dialog->setAttribute( Qt::WA_DeleteOnClose, true );
    dialog->show();
    //return dialog;
////    win_open=true;
////    } else {
////        if(dialog)dialog->raise();
////    }
}


void/*QDialog **/ CreateTableConstructor::get_help_window(QPointer<QDialog>&window_pointer,QString const & header_title__,const QString & info__,QWidget*parent__)
{
    //QMessageBox::about(this,"Data types",info);
    //static QPointer <QDialog> dialog;

////static bool win_open = false;
////    if(!win_open){
    /*QPointer <QDialog> dialog*/window_pointer = new QDialog{parent__};
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
        qDebug() << "~help dialog activated (destroyed).";
////        win_open=false;
    });
    window_pointer->setAttribute( Qt::WA_DeleteOnClose, true );
    window_pointer->show();
    //return dialog;
////    win_open=true;
////    } else {
////        if(dialog)dialog->raise();
////    }


}


