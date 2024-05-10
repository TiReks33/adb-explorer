#include "create_table_constructor.h"
#include "ui_create_table_constructor.h"

CreateTableConstructor::CreateTableConstructor(auth& auth__,QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::CreateTableConstructor)
  , first_attribute_(true)
  , sql_query_("")
  , first_key_(true)
  , auth_(auth__)
  , non_static_connection_(new db_connection)
{
    ui->setupUi(this);

    ui->atr_type_capacity_1->setMaxLength(5);


    ui->plainTextEdit_1->setReadOnly(true);
    ui->plainTextEdit_2->setReadOnly(true);

    //"Search button"
       ui->help_button_2->setFixedSize(67,67);
        QRect rect(0,0,67,67);
       QRegion region(rect, QRegion::Ellipse);
       ui->help_button_2->setMask(region);
        ui->help_button_2->setStyleSheet("border: none;"
            "background-color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #e7ebef, stop: 1 #cde3f2);"
                        "color: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,    stop: 0 #5598c9, stop: 1 #a9d5f5);"
                                );

    //SIGNALs
    connect(ui->atr_type_comboBox_1,SIGNAL(currentTextChanged(QString const&)),this,SLOT(AttrTypeChanged(QString const&)));
//ui->atr_type_comboBox_1->currentTextChange
    connect(ui->foreign_key_checkBox_2,SIGNAL(stateChanged(int)),this,SLOT(foreignkeychecked(int)));
    connect(ui->on_delete_checkBox_2,SIGNAL(stateChanged(int)),this, SLOT(onDeleteChecked(int)));
    connect(ui->on_update_checkBox_2,SIGNAL(stateChanged(int)),this, SLOT(onUpdateChecked(int)));

}

CreateTableConstructor::~CreateTableConstructor()
{
    delete ui;
    delete non_static_connection_;
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

    QRegularExpression re(".*[a-zA-Z].*");
    QRegularExpressionValidator v(re, 0);
    QString str = ui->atr_name_line_1->text();
    int pos=0;
    if(v.validate(str, pos)!=QValidator::Acceptable)
    {
        ui->statusLine_1->setText("Name must consist at least 1 alphabetic character(lower or APPER case).");
        return false;
    }


//    attributes_.append(ui->atr_name_line_1->text());

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
            //qDebug() << "VALIDATION" << v.validate(str, pos);
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

//                QRegularExpression re("([1-9]|[15][0-9]|6[0-5]),([1-9]|[12][0-9]|3[0-8])");
//                QRegularExpression re("([0-9]|[1-5][0-9]|6[0-5]),([0-9]|[1-2][0-9]|3[0-8])");
                QRegularExpression re("([0-9]|[1-5][0-9]|6[0-5])|(([0-9]|[1-5][0-9]|6[0-5]),([0-9]|[1-2][0-9]|3[0-8]))");

                QRegularExpressionValidator v(re, 0);

                QString str = ui->atr_type_capacity_1->text();

                int pos=0;

                if(v.validate(str, pos)!=QValidator::Acceptable){

                ui->statusLine_1->setText("Please, choose correct DECIMAL type parameters (or leave empty).");
                return false;
                } else {
                    // M >= D
//                    #define text    (ui->atr_type_capacity_1->text())
//                    QString first = ui->atr_type_capacity_1->text().at(0);first+=ui->atr_type_capacity_1->text().at(1);
//                    QString second = ui->atr_type_capacity_1->text().at(3);second+=ui->atr_type_capacity_1->text().at(4);
//                    if(second.toInt()>first.toInt()){
//                        ui->statusLine_1->setText("For decimal(M,D), M must be >= D. Please, choose correct DECIMAL type parameters (or leave empty).");
//                        return;
//                    }
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


        // AUTO_INCREMENT set avaliable for Integer type of attribute
        //if(ui->atr_type_comboBox_1->currentText()=="INT")ui->AUTO_INCREMENT_checkBox_1->setEnabled(true);



//    ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText()+ui->atr_name_line_1->text()+' '
//                                         +ui->atr_type_comboBox_1->currentText());
        //textEdit->insertPlainText(ui->plainTextEdit_1->toPlainText());

        attributes_.append(ui->atr_name_line_1->text());

        if(!first_attribute_)
        textEdit->insertPlainText(",");

        textEdit->insertPlainText(ui->atr_name_line_1->text()+' '+ui->atr_type_comboBox_1->currentText());

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
//    textEdit->clear();

//    textEdit->insertPlainText(ui->plainTextEdit_1->toPlainText());


    if(ui->foreign_key_checkBox_2->isChecked()){
//        ui->foreign_key_line_2->setEnabled(true);
//        ui->references_line_2->setEnabled(true);
        if(ui->foreign_key_combobox_2->currentText().isEmpty()){
            ui->statusLine_2->setText("Please, select foreign key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
    } else if(ui->ref_table_line_2->text().isEmpty()){
            ui->statusLine_2->setText("Please, select reference table for key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        } else if(ui->ref_key_line_2->text().isEmpty()){
            ui->statusLine_2->setText("Please, select reference attribute for key; OR unchecked the 'FOREIGN KEY' checkbox.");
        return false;
        }

//        if(!first_key_)
    textEdit->insertPlainText(", ");

    textEdit->insertPlainText(" FOREIGN KEY ("+ui->foreign_key_combobox_2->currentText()+')');

    textEdit->insertPlainText(" REFERENCES "+ui->ref_table_line_2->text()+'('+ui->ref_key_line_2->text()+')');


    if(ui->on_delete_checkBox_2->isChecked())
    textEdit->insertPlainText(" ON DELETE "+ui->onDelete_comboBox_2->currentText());

    if(ui->on_update_checkBox_2->isChecked())
    textEdit->insertPlainText(" ON UPDATE "+ui->onUpdate_comboBox_2->currentText());


    }


//    textEdit->insertPlainText(");");

    //qDebug() << "FINAL SQL-QUERY::"<<sql_query_;
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
    //qDebug()<<"state: "<<state;
    if(state==0)
    {
        ui->foreign_key_combobox_2->setEnabled(false);
        ui->ref_table_line_2->setEnabled(false);
        ui->ref_key_line_2->setEnabled(false);
//ui->onDeleteLayout_2->setEnabled(false);
        ui->on_delete_checkBox_2->setEnabled(false);
        ui->on_update_checkBox_2->setEnabled(false);
        ui->onDelete_comboBox_2->setEnabled(false);
        ui->onUpdate_comboBox_2->setEnabled(false);

        ui->plus_button_2->setEnabled(false);
    } else
    {
        ui->foreign_key_combobox_2->setEnabled(true);
        ui->ref_table_line_2->setEnabled(true);
        ui->ref_key_line_2->setEnabled(true);
//ui->onDeleteLayout_2->setEnabled(true);
        ui->on_delete_checkBox_2->setEnabled(true);
        ui->on_update_checkBox_2->setEnabled(true);

        if(ui->on_delete_checkBox_2->isChecked())
            ui->onDelete_comboBox_2->setEnabled(true);

        if(ui->on_update_checkBox_2->isChecked())
            ui->onUpdate_comboBox_2->setEnabled(true);

        ui->plus_button_2->setEnabled(true);
    }
}

void CreateTableConstructor::onDeleteChecked(int state)
{
    if(state==0)
        ui->onDelete_comboBox_2->setEnabled(false);
     else
        ui->onDelete_comboBox_2->setEnabled(true);
}

void CreateTableConstructor::onUpdateChecked(int state)
{
    if(state==0)
        ui->onUpdate_comboBox_2->setEnabled(false);
     else
        ui->onUpdate_comboBox_2->setEnabled(true);
}

void CreateTableConstructor::on_next_0_clicked()
{
    if(ui->tbl_name_line_0->text().isEmpty()){
        ui->statusLine_0->setText("Table name must not be empty.");
        return;
    } else
    {
        QRegularExpression re(".*[a-zA-Z].*");
        QRegularExpressionValidator v(re, 0);
        QString str = ui->tbl_name_line_0->text();
        int pos=0;
        if(v.validate(str, pos)!=QValidator::Acceptable)
        {
            ui->statusLine_0->setText("Name must consist at least 1 alphabetic character(lower or APPER case).");
            return;
        }

        ui->plainTextEdit_1->clear();
        ui->plainTextEdit_1->insertPlainText("CREATE TABLE "+ui->tbl_name_line_0->text()+" ( ");
            this->setCurrentIndex(1);
    }

}

void CreateTableConstructor::on_next_1_clicked()
{
    if(!first_attribute_){
        ui->plainTextEdit_2->clear();
        ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText());
    //    if(!add_attributes(ui->plainTextEdit_2))
    //        return;

        ui->foreign_key_combobox_2->addItems(attributes_);
        ui->foreign_key_combobox_2->setCurrentIndex(-1);


//            CustomQueryResult new_select_window{auth_};
//            new_select_window.show();
//            //custom_query_result_window_->show();
//            new_select_window.custom_query_slot("SHOW DATABASES; ",ui->ref_DB_comboBox_2);

        db_connection::open(auth_);


        db_connection::set_query("SHOW DATABASES;",this->non_static_connection_->model_,ui->ref_DB_comboBox_2,1);

        ui->ref_DB_comboBox_2->setCurrentIndex(-1);

        //new_select_window.exec();

        this->setCurrentIndex(2);

    } else
    {
        ui->statusLine_1->setText("Please, add 1 or more attributes to current table before next step.");
    }
    //}
}

//void Hints::on_pushButton_2_clicked()
//{
//    this->setCurrentWidget(ui->page_2);
//}

void CreateTableConstructor::on_send_button_clicked()
{
//    sql_query_.clear();

//    sql_query_.append(ui->plainTextEdit_2->toPlainText());


//    if(ui->foreign_key_checkBox_2->isChecked()){
////        ui->foreign_key_line_2->setEnabled(true);
////        ui->references_line_2->setEnabled(true);
//        if(ui->foreign_key_line_2->text().isEmpty()){
//            ui->statusLine_1->setText("Please, select foreign key; OR unchecked the 'FOREIGN KEY' checkbox.");
//        return;
//    } else if(ui->ref_table_line_2->text().isEmpty()){
//            ui->statusLine_1->setText("Please, select reference table for key; OR unchecked the 'FOREIGN KEY' checkbox.");
//        return;
//        } else if(ui->ref_key_line_2->text().isEmpty()){
//            ui->statusLine_1->setText("Please, select reference attribute for key; OR unchecked the 'FOREIGN KEY' checkbox.");
//        return;
//        }
//    sql_query_.append(", ");

//    sql_query_.append(" FOREIGN KEY ("+ui->foreign_key_line_2->text()+')');

//    sql_query_.append(" REFERENCES "+ui->ref_table_line_2->text()+'('+ui->ref_key_line_2->text()+')');


//    if(ui->on_delete_checkBox_2->isChecked())
//    sql_query_.append(" ON DELETE "+ui->onDelete_comboBox_2->currentText());

//    if(ui->on_update_checkBox_2->isChecked())
//    sql_query_.append(" ON UPDATE "+ui->onDelete_comboBox_2->currentText());


//    }


//    sql_query_.append(");");

//    qDebug() << "FINAL SQL-QUERY::"<<sql_query_;


    //ui->plainTextEdit_2->clear();
    //ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText());
    qDebug() << "SQL FINAL QUERY::" << ui->plainTextEdit_2->toPlainText()+" );";

}


void CreateTableConstructor::on_plus_button_1_clicked(){



//    if(!first_attribute_)
//    ui->plainTextEdit_1->insertPlainText(",\n\t\t");

//    ui->plainTextEdit_1->insertPlainText(ui->atr_name_line_1->text()+' '
//                                         +ui->atr_type_comboBox_1->currentText());

//    if(ui->atr_type_comboBox_1->currentText()=="VARCHAR"||(ui->atr_type_comboBox_1->currentText()=="DECIMAL"&&(!ui->atr_type_capacity_1->text().isEmpty())))
//                        ui->plainTextEdit_1->insertPlainText('('+ui->atr_type_capacity_1->text()+')');
//    else ui->plainTextEdit_2->insertPlainText(" ");

//    if(ui->primary_key_checkBox_1->isChecked())
//                        ui->plainTextEdit_1->insertPlainText(" PRIMARY KEY ");
//    if(ui->AUTO_INCREMENT_checkBox_1->isChecked())
//                        ui->plainTextEdit_1->insertPlainText(" AUTO_INCREMENT ");
//    if(ui->NOT_NULL_checkBox_1->isChecked())
//                        ui->plainTextEdit_1->insertPlainText(" NOT NULL ");

    if(add_attributes(ui->plainTextEdit_1)){

        first_attribute_=false;

        ui->atr_name_line_1->setText("");
        ui->atr_type_comboBox_1->setCurrentIndex(-1);
        ui->atr_type_capacity_1->setText("");

        ui->primary_key_checkBox_1->setChecked(false);
        ui->NOT_NULL_checkBox_1->setChecked(false);
        ui->AUTO_INCREMENT_checkBox_1->setChecked(false);

    }

}


void CreateTableConstructor::on_erase_button_1_clicked()
{
    ui->plainTextEdit_1->clear();
    ui->plainTextEdit_1->insertPlainText("CREATE TABLE "+ui->tbl_name_line_0->text()+" ( ");
    first_attribute_=true;
    attributes_.clear();
    ui->statusLine_1->clear();
}

void CreateTableConstructor::on_help_button_2_clicked()
{
//    QString success=":/rec/icons/rec/icons/success.png";
//    success="<img src='"+success+"' height='32' width='32'>";

//        QFile file(":/rec/icons/rec/icons/logo.txt");
//    //qDebug() << dir;
//            if(!file.open(QIODevice::ReadOnly))
//                ;
//            else{
//                QTextStream in(&file);
//            logo="<pre style=\"white-space: pre-wrap;\">"+in.readAll()+"</pre>";
//            file.close();
//            }

//        QMessageBox::about(0,"Search Pattern about",note+loop+tux+kbmouse+abc+success+
//                             "<p style=\"font-family:'Serif';font-size:22px;\">Plain text edit with "
//                             "multiply occurrences search functional. "
//                             "Program uses <b>std::string</b>'s very optimized methods in search algorithm;"
//                             " was made JFF and Qt/C++ learning basic technics goal.:)</p><br>"
//                                              +logo);

    QString reference_info=":/txt/references_help.txt";
    //    success="<img src='"+success+"' height='32' width='32'>";

    //        QFile file(":/rec/icons/rec/icons/logo.txt");
    //    //qDebug() << dir;
    //            if(!file.open(QIODevice::ReadOnly))
    //                ;
    //            else{
    //                QTextStream in(&file);
    //            logo="<pre style=\"white-space: pre-wrap;\">"+in.readAll()+"</pre>";
    //            file.close();
    //            }

    QMessageBox::about(this,"Referential Actions",reference_info);
}

void CreateTableConstructor::on_reset_button_2_clicked()
{
    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText());
}


void CreateTableConstructor::on_plus_button_2_clicked()
{

    if(add_keys(ui->plainTextEdit_2)){

        first_key_=false;



    }

}
