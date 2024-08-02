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
//  , non_static_connection_2_(new db_connection)
{
    ui->setupUi(this);


//    auth_autonome_.host_=auth_.host_;
//    auth_autonome_.login_=auth_.login_;
//    auth_autonome_.passw_=auth_.passw_;


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
    connect(ui->atr_type_comboBox_1,SIGNAL(currentTextChanged(QString const&)),this,SLOT(AttrTypeChanged(QString const&)));
//ui->atr_type_comboBox_1->currentTextChange
    connect(ui->foreign_key_checkBox_2,SIGNAL(stateChanged(int)),this,SLOT(foreignkeychecked(int)));
    connect(ui->on_delete_checkBox_2,SIGNAL(stateChanged(int)),this, SLOT(onDeleteChecked(int)));
    connect(ui->on_update_checkBox_2,SIGNAL(stateChanged(int)),this, SLOT(onUpdateChecked(int)));

    //constructor
//    connect(ui->ref_DB_comboBox_2,SIGNAL(stateChanged(int)),this, SLOT(add_tbl_constructor_db2table_slot(int)));
    connect(ui->ref_DB_comboBox_2,SIGNAL(currentTextChanged(QString const&)),this, SLOT(add_tbl_constructor_db2table_slot(const QString &)),Qt::QueuedConnection);
    connect(ui->ref_table_comboBox_2,SIGNAL(currentTextChanged(QString const&)),this, SLOT(add_tbl_constructor_table2atribute_slot(const QString &)),Qt::QueuedConnection);
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

//TextCursor things
//        QTextCursor cursor = this->ui->plainTextEdit_1->textCursor();
//        cursor.movePosition(QTextCursor::End);
//        this->ui->plainTextEdit_1->setTextCursor(cursor);
        set_cursor_to_end_(this->ui->plainTextEdit_1);

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

    textEdit->insertPlainText(" FOREIGN KEY ("+ui->foreign_key_combobox_2->currentText()+')');

    textEdit->insertPlainText(" REFERENCES " + ui->ref_DB_comboBox_2->currentText() + '.' +
                              ui->ref_table_comboBox_2->currentText()+'('+ui->ref_key_comboBox_2->currentText()+')');


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
        ui->ref_DB_comboBox_2->setEnabled(false);
        ui->ref_table_comboBox_2->setEnabled(false);
        ui->ref_key_comboBox_2->setEnabled(false);
//ui->onDeleteLayout_2->setEnabled(false);
        ui->on_delete_checkBox_2->setEnabled(false);
        ui->on_update_checkBox_2->setEnabled(false);
        ui->onDelete_comboBox_2->setEnabled(false);
        ui->onUpdate_comboBox_2->setEnabled(false);

        ui->plus_button_2->setEnabled(false);
    } else
    {
        ui->foreign_key_combobox_2->setEnabled(true);
        ui->ref_DB_comboBox_2->setEnabled(true);
        ui->ref_table_comboBox_2->setEnabled(true);
        ui->ref_key_comboBox_2->setEnabled(true);
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

void CreateTableConstructor::add_tbl_constructor_db2table_slot(const QString &current_DB_/*item*/)
{
//    {
//    if(non_dflt_conction_names_.isEmpty())non_dflt_conction_names_.append("first");
//    //db_connection::set_query("USE "+current_item_+"; " + "SHOW TABLES;",this->non_static_connection_2_->model_,ui->ref_table_comboBox_2,1);
//    //  QSqlDatabase firstDB = QSqlDatabase::database("first");
//        {
//            if(non_dflt_conction_names_.length()>=2){
//                if(QSqlDatabase::database(non_dflt_conction_names_.at(1)).isOpen())
//                    db_connection::close_con(non_dflt_conction_names_.at(1));//<<-- close connection
//            }

//        }
//    if(QSqlDatabase::database(non_dflt_conction_names_.at(0)).isOpen())
//        db_connection::close_con(non_dflt_conction_names_.at(0));//<<-- close connection
//    }

//    {
//        QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth_.db_server_,non_dflt_conction_names_.at(0));

//            db_connection_.setUserName(auth_.login_);

//            db_connection_.setPassword(auth_.passw_);

//            //db_connection_.setHostName("localhost");//<-remote IP

//            db_connection_.setDatabaseName(current_item_);

//            if(!db_connection_.open()){
////                qDebug() << ("(x)Error connection to database.");
//                qDebug()<<QSqlDatabase::database(non_dflt_conction_names_.at(0)).lastError();
//                return;
//            }
//            else{
//                qDebug()<<("Database ::"+non_dflt_conction_names_.at(0) +":: succesfull connected.");
//                qDebug()<<non_dflt_conction_names_.at(0);
//                qDebug()<<current_item_;
//                //return;
//            }
//    }
    auth_.db_name_=current_DB_;
//    if(!db_connection::open(auth_,this->metaObject(),&multi_con_)){
    if(!db_connection::open(auth_)){
        qDebug() << QString("(x)There is error while update tables (connection is not established).");
        return;
    }

    db_connection::set_query("SHOW DATABASES;", submodel_0_,ui->ref_DB_comboBox_2,/*multi_con_*/auth_.con_name_,1);

    ui->ref_key_comboBox_2->setCurrentIndex(-1); //?

    //SET QUERY-->>

//    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(non_dflt_conction_names_.at(0)));

//    qry.prepare("SHOW TABLES;"); //MY_SQL_QUERY

//    if(qry.exec()){
////    non_static_connection_->model_.setQuery(qry);

//    submodel_1_.setQuery(qry);

//    ui->ref_table_comboBox_2->setModel(&submodel_1_);

//    qDebug() << "QUERY TO COMBOBOX SUCCESS";
//    return ;
//    } else
//    {
//        qDebug()<<"111111111111"<<QSqlDatabase::database(non_dflt_conction_names_.at(0)).lastError();
//        return;
//    }

}

void CreateTableConstructor::add_tbl_constructor_table2atribute_slot(const QString &current_item_)
{
//    qDebug() << "table is changed"<<(non_dflt_conction_names_.at(1)==nullptr);

    {
    if(non_dflt_conction_names_.length()<2)non_dflt_conction_names_.append("second");
    //db_connection::set_query("USE "+current_item_+"; " + "SHOW TABLES;",this->non_static_connection_2_->model_,ui->ref_table_comboBox_2,1);
    //  QSqlDatabase firstDB = QSqlDatabase::database("first");
    if(QSqlDatabase::database(non_dflt_conction_names_.at(1)).isOpen())
        db_connection::close/*_con*/(non_dflt_conction_names_.at(1));//<<-- close connection
    }

    {
        QSqlDatabase db_connection_=QSqlDatabase::addDatabase(auth_.db_server_,non_dflt_conction_names_.at(1));

            db_connection_.setUserName(auth_.login_);

            db_connection_.setPassword(auth_.passw_);

            //db_connection_.setHostName("localhost");//<-remote IP

            db_connection_.setDatabaseName(ui->ref_DB_comboBox_2->currentText());

            if(!db_connection_.open()){
//                qDebug() << ("(x)Error connection to database.");
                qDebug()<<QSqlDatabase::database(non_dflt_conction_names_.at(1)).lastError();
                return;
            }
            else{
                qDebug()<<("Database ::"+non_dflt_conction_names_.at(1) +":: succesfull connected.");
                qDebug()<<non_dflt_conction_names_.at(1);
                qDebug()<<current_item_;
                //return;
            }
    }


    //QUERY

    QSqlQuery qry = QSqlQuery(QSqlDatabase::database(non_dflt_conction_names_.at(1)));
//SHOW `columns` FROM `your-table`;
    qry.prepare("SHOW COLUMNS FROM "+current_item_+";"); //MY_SQL_QUERY

    if(qry.exec()){
//    non_static_connection_->model_.setQuery(qry);

    submodel_2_.setQuery(qry);

    ui->ref_key_comboBox_2->setModel(&submodel_2_);

    qDebug() << "QUERY TO COMBOBOX SUCCESS";
    return ;
    } else
    {
        qDebug()<<"2222222222"<<QSqlDatabase::database(non_dflt_conction_names_.at(1)).lastError();
        return;
    }
}

//void CreateTableConstructor::close_con(QString const &con)
//{
//        {
//            QSqlDatabase db = QSqlDatabase::database(con);
//            if(db.isOpen())db.close();
//        }
//    QSqlDatabase::removeDatabase( con/*QSqlDatabase::database().connectionName()*/ );
//    qDebug() << "Connection was closed.";
//}

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


//    if(!non_dflt_conction_names_.isEmpty()){
//    close_con(non_dflt_conction_names_.at(0));
//    non_dflt_conction_names_.clear();
//    }
    if(!non_dflt_conction_names_.isEmpty()){
    size_t con_sum=non_dflt_conction_names_.size();
        for(size_t i=0;i!=con_sum;++i)
        {
            db_connection::close/*_con*/(non_dflt_conction_names_.at(i));
        }
        non_dflt_conction_names_.clear();
    }

    //first_attribute_=true;
    erase();
    //attributes_.clear();
    ui->foreign_key_combobox_2->clear();
    ui->foreign_key_checkBox_2->setChecked(false);

    ui->tbl_name_line_0->clear();
    ui->statusLine_0->clear();



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
    db_connection::open(auth_);


    db_connection::set_query("SHOW DATABASES;",this->non_static_connection_->model_,ui->ref_DB_comboBox_2,1);

    this->ui->statusLine_2->setText("(x)Table constructor query fails.");
}

void CreateTableConstructor::erase()
{
    ui->plainTextEdit_1->clear();
    ui->plainTextEdit_1->insertPlainText("CREATE TABLE "+ui->tbl_name_line_0->text()+" ( ");
    first_attribute_=true;
    attributes_.clear();
    ui->statusLine_1->clear();
    ui->number_line_1->setText(QString::number(0));
    attributes_added_=0;
}

//void CreateTableConstructor::add_tbl_constructor_db2table_slot(int)
//{

//}

void CreateTableConstructor::on_next_0_clicked()
{
    if(ui->tbl_name_line_0->text().isEmpty()){
        ui->statusLine_0->setText("Table name must not be empty.");
        return;
    } else {
        QRegularExpression re(".*[a-zA-Z].*");
        QRegularExpressionValidator v(re, 0);
        QString str = ui->tbl_name_line_0->text();
        int pos=0;
        if(v.validate(str, pos)!=QValidator::Acceptable)
        {
            ui->statusLine_0->setText("Name must consist at least 1 alphabetic character(lower or APPER case).");
            return;
        } else{
            //"^[a-zA-Z0-9_.-]*$" -- ok
            QRegularExpression re("^[a-zA-Z0-9_]*$");
            QRegularExpressionValidator v(re, 0);
            QString str = ui->tbl_name_line_0->text();
            int pos=0;
            if(v.validate(str, pos)!=QValidator::Acceptable)
            {
                ui->statusLine_0->setText("Incorrect symbols in table's name. Please use low and upper letters, digits or '_'");
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
        ui->plainTextEdit_1->insertPlainText("CREATE TABLE "+ui->tbl_name_line_0->text()+" ( ");
            this->setCurrentIndex(1);
    }

}

void CreateTableConstructor::on_next_1_clicked()
{
    if(!first_attribute_){
        ui->plainTextEdit_2->clear();
        ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText()/*+')'*/);
    //    if(!add_attributes(ui->plainTextEdit_2))
    //        return;

        ui->foreign_key_combobox_2->addItems(attributes_);
        ui->foreign_key_combobox_2->setCurrentIndex(-1);


//            CustomQueryResult new_select_window{auth_};
//            new_select_window.show();
//            //custom_query_result_window_->show();
//            new_select_window.custom_query_slot("SHOW DATABASES; ",ui->ref_DB_comboBox_2);

        //////////////////////////////////////

//        db_connection::open(auth_); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11


//        db_connection::set_query("SHOW DATABASES;",this->non_static_connection_->model_,ui->ref_DB_comboBox_2,1);


        ///////////////////////////////////

        //if(!db_connection::open(auth_,this->metaObject(),&multi_con_)){
        if(!db_connection::open(auth_)){
            qDebug() << QString("(x)There is error while update tables (connection is not established).");
            return;
        }

        db_connection::set_query("SHOW DATABASES;", submodel_0_,ui->ref_DB_comboBox_2,/*multi_con_*/auth_.con_name_,1);


        /////////////////////////////////////////////





        size_t size_of_list_=ui->ref_DB_comboBox_2->model()->rowCount();

        qDebug() << "NUMBER OF TABLES::" << size_of_list_;

        //qDebug() << "CURRENT TEXT::"<<ui->tableView->model()->index(1,0).data().toString();
        for(size_t i=0;i!=size_of_list_;++i){
            if(ui->ref_DB_comboBox_2->model()->index(i,0).data().toString()==auth_.db_name_){ // SET CURRENT DB AS DEFAULT
                ui->ref_DB_comboBox_2->setCurrentIndex(i);                                        // IN COMBOBOX
                break; // return or break?
            }
        }
        //ui->ref_DB_comboBox_2->setCurrentIndex(-1);
        //}




//        ui->ref_DB_comboBox_2->setCurrentIndex(-1);

        //new_select_window.exec();

        this->setCurrentIndex(2);

    } else
    {
        ui->statusLine_1->setText("Please, add 1 or more attributes to current table before next step.");
    }



}

//void Hints::on_pushButton_2_clicked()
//{
//    this->setCurrentWidget(ui->page_2);
//}

void CreateTableConstructor::on_send_button_clicked()
{
    set_cursor_to_end_(this->ui->plainTextEdit_2);
    //qDebug() << "SQL FINAL QUERY::" << ui->plainTextEdit_2->toPlainText()+" );";
    ui->plainTextEdit_2->insertPlainText(" );");
    qDebug() << "SQL FINAL QUERY::" << ui->plainTextEdit_2->toPlainText();

    //this->close();

    emit send_custom_query(ui->plainTextEdit_2->toPlainText());

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

}


void CreateTableConstructor::on_erase_button_1_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to start"
                                " adding attributes from the beginning? All current progress will be lost.",
                                                             QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        qDebug() << "Create Table constr erase:: Yes was clicked";
        erase();
      } else {
        qDebug() << "Create Table constr erase:: cancel was clicked";
      }

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

    QString reference_info=":/txt/references_help_2.txt";
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
    QString info;
    QFile file(reference_info);
//qDebug() << dir;
        if(file.open(QIODevice::ReadOnly))
            {
            QTextStream in(&file);
        info="<pre style=\"white-space: pre-wrap;\">"+in.readAll()+"</pre>";
        file.close();
        }

    QMessageBox::about(this,"Referential Actions",info);
}

void CreateTableConstructor::on_reset_button_2_clicked()
{
    ui->plainTextEdit_2->clear();
    ui->plainTextEdit_2->insertPlainText(ui->plainTextEdit_1->toPlainText());
}


void CreateTableConstructor::on_plus_button_2_clicked()
{
    set_cursor_to_end_(this->ui->plainTextEdit_2);

    if(add_keys(ui->plainTextEdit_2)){

        first_key_=false;

    }

}


void CreateTableConstructor::on_describe_tbl_button_2_clicked()
{
    auth __auth = auth_;
    __auth.db_name_=ui->ref_DB_comboBox_2->currentText();
qDebug() << "COUNTER BEFORE::" << window_counter_;
    ++window_counter_;

    CustomQueryResult new_select_window{__auth};

    //__auth.db_name_=ui->ref_DB_comboBox_2->currentText()+QString::number(window_counter_);
    QString _con_name=ui->ref_DB_comboBox_2->currentText()+QString::number(window_counter_);

    new_select_window.setWindowTitle(ui->ref_table_comboBox_2->currentText());
    new_select_window.show();
//custom_query_result_window_->show();
new_select_window.custom_query_slot(QString(/*"USE "+ui->ref_DB_comboBox_2->currentText()+';'+' '+*/"DESCRIBE ")+
                                    ui->ref_table_comboBox_2->currentText()+(";")/*,new_select_window->ui->tableView*/,
                                    /*new_select_window->model_,*/new_select_window.ui->tableView, _con_name/*__auth.db_name_*/);
//if(model_.rowCount()!=0)
new_select_window.exec();


db_connection::close/*_con*/(_con_name/*__auth.db_name_*/);
--window_counter_;
qDebug() << "COUNTER AFTER::" << window_counter_;
}

void CreateTableConstructor::on_cancel_2_clicked()
{

}

void CreateTableConstructor::on_back_button_2_clicked()
{
//    ui->plainTextEdit_1->clear();
//    ui->plainTextEdit_1->insertPlainText("CREATE TABLE "+ui->tbl_name_line_0->text()+" ( ");
//    first_attribute_=true;
//    attributes_.clear();
//    ui->statusLine_1->clear();
//    ui->number_line_1->setText(QString::number(0));
//    attributes_added_=0;
    erase();

    this->setCurrentIndex(1);

    ui->foreign_key_combobox_2->clear();
    ui->foreign_key_checkBox_2->setChecked(false);
}

void CreateTableConstructor::on_back_button_1_clicked()
{
    this->setCurrentIndex(0);
    ui->plainTextEdit_1->clear();
    first_attribute_=true;
    attributes_.clear();
    ui->foreign_key_combobox_2->clear();
}

void CreateTableConstructor::on_cancel_0_clicked()
{
    this->close();
}

void CreateTableConstructor::on_help_button_1_clicked()
{
    QString reference_info=":/txt/references_help_1.txt";

    QString info;
    QFile file(reference_info);

        if(file.open(QIODevice::ReadOnly))
            {
            QTextStream in(&file);
        info="<pre style=\"white-space: pre-wrap;\">"+in.readAll()+"</pre>";
        file.close();
        }

    QMessageBox::about(this,"Referential Actions",info);
}
