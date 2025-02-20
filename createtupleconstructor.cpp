#include "createtupleconstructor.h"
#include "ui_createtupleconstructor.h"



createTupleConstructor::createTupleConstructor(auth& auth__,QWidget *parent) :
    QWidget/*QDialog*/(parent),
    ui(new Ui::createTupleConstructor)
  , statusBar(new scrolledStatusBar)
  , auth_(auth__)
  , update_tables_button{new reloadButton{0,"darkslategray","snow",true,false,"Update tables list"}}
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/pic/adb-explorer_logo.png"));

    initForm();

    signals_init();

}

createTupleConstructor::~createTupleConstructor()
{
    delete ui;

    delete statusBar;
    //qDebug() << "TupleConstr ~Destructor activated";
}

void createTupleConstructor::initForm()
{
    ui->verticalLayout->addWidget(statusBar);

    ui->CancelButton->setText("⌧");
    ui->CancelButton->setStyleSheet("QPushButton:!disabled {background-color:darkred;color:white;font-size:18pt;padding-left:6px;padding-right:6px;}");//padding-top:1px;padding-bottom:1px;");
    ui->okButton->setStyleSheet("QPushButton {background:green; color:white;} QPushButton:disabled {background-color:gray;}");

    ui->reloadVerticalLayout->addWidget(update_tables_button);

    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj->objectName()=="reloadButtonObj"){
//                qobject_cast<reloadButton*>(obj)->setKhakiHighlight();
                auto reloadButtonObj = qobject_cast<reloadButton*>(obj);
                reloadButtonObj->setKhakiHighlight();

            }else{
                obj->setStyleSheet(QStringLiteral("%1 %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }


    ui->comboBox->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#fffffa","darkslategray"));


    ui->frame->setObjectName("whiteFrame0");

    ui->frame->setStyleSheet(" QFrame#whiteFrame0 { background: white; } ");

}


void createTupleConstructor::signals_init()
{
    connect(ui->comboBox,&QComboBox::currentTextChanged, [=](QString const& string_){

        if (string_==""){

            ui->frame->setStyleSheet(" QFrame#whiteFrame0 { background: white; } ");

            ui->frame->setEnabled(true);

            ui->frame_2->setEnabled(false);
            ui->frame_2->setStyleSheet("background: palette(window)");
            ui->frame_3->setEnabled(false);
            ui->frame_3->setStyleSheet("background: palette(window)");

        } else {

            ui->frame->setStyleSheet(" QFrame#whiteFrame0 { background: palette(window); }   ");

            ui->frame->setEnabled(false);


            ui->frame_2->setEnabled(true);
            ui->frame_2->setStyleSheet("background: white");
        }

    });


    connect(ui->plainTextEdit_2,&QPlainTextEdit::textChanged, [=](){

        if (ui->plainTextEdit_2->toPlainText()!=""){
            ui->frame_2->setEnabled(false);
            ui->frame_2->setStyleSheet("background: palette(window)");

            ui->frame_3->setEnabled(true);
            ui->frame_3->setStyleSheet("background: white");
            ui->frame_4->setEnabled(true);
            ui->frame_4->setStyleSheet("background: white");
        }
    });


    connect(ui->plainTextEdit,&QPlainTextEdit::textChanged, [=](){
                    statusBar->get_line()->clear();
            });


    connect(ui->okButton,&QPushButton::clicked,[=]{

        if(!tuples_added_){
            statusBar->get_line()->setText("You don't add any values to table!");
            return;
        }


//        QString final_query = QString("INSERT INTO `%1` (%2) VALUES ").arg(QString(adb_utility::escape_sql_backticks(/*auth_.table_name_*/ui->comboBox->currentText())))
//                                                                        .arg(QString(adb_utility::escape_sql_backticks(ui->plainTextEdit_2->toPlainText())));

//        int count = tuples_.count();
//        for (int i=0;i!=count;++i)
//        {
//            final_query+= '(' + tuples_.at(i) + ')';
//            if (i!=count-1) final_query+=", ";
//        }

        QString final_query = QString("INSERT INTO `%1` (").arg(QString(adb_utility::escape_sql_backticks(/*auth_.table_name_*/ui->comboBox->currentText())));

        int count = chosenAttributes_.count();
        for (auto i=0;i!=count;++i)
        {
            final_query+='`'+chosenAttributes_.at(i)+'`';
            if (i!=count-1) final_query+=", ";
        }

        final_query+=") VALUES ";

        count = tuples_.count();
        for (auto i=0;i!=count;++i)
        {
            final_query+= '(' + tuples_.at(i) + ')';
            if (i!=count-1) final_query+=", ";
        }


        final_query+=';';

//        this->close();
        emit this->closeNowSig();

        std::cout << "final query::" <<final_query.toStdString() << std::endl;

        emit final_query_sig(final_query);

    });


    connect(update_tables_button,&QPushButton::clicked,this,&createTupleConstructor::update_tables_list);

    connect(ui->reset_button,&QPushButton::clicked,[=]{
        QMessageBox::StandardButton reply = QMessageBox::warning(this, "Are you sure?", "Do you want to start"
                                    " adding fields from the beginning? All current progress will be lost.",
                                                                 QMessageBox::Yes|QMessageBox::No);
          if (reply == QMessageBox::Yes) {
           // qDebug() << "Yes was clicked";
            resetButtonHandler();
          } else {
            //qDebug() << "cancel";
          }
    });

    connect(ui->addColsButton,&QPushButton::clicked,[=]{
        TwoListSelection doublelist{auth_};

        connect(&doublelist,&TwoListSelection::export_list,[=](QStringList const & list__){
            ////qDebug() << "List from double list::" << list__;
            chosenAttributes_=list__;
            ui->plainTextEdit_2->setPlainText(adb_utility::pack_(list__));
        });

        QString const query = QString("SHOW COLUMNS FROM `%1`").arg(QString(adb_utility::escape_sql_backticks(/*auth_.table_name_*/ui->comboBox->currentText())));
        doublelist.update_doublelist(query);
        doublelist.setModal(true);
        doublelist/*list_selection_window_->*/.show();
        doublelist.exec();
    });

    connect(ui->CancelButton,&QPushButton::clicked,this,&createTupleConstructor::close);

    connect(this,&createTupleConstructor::closeNowSig,[this]{
        closeMessageFlag_=false;
        close();
    });
}


void createTupleConstructor::resetButtonHandler()
{
    ui->frame->setEnabled(true);
    ui->frame->setStyleSheet("QFrame#whiteFrame0{background: white}");
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
    chosenAttributes_.clear();
    ui->comboBox->setCurrentIndex(-1);
    ui->plainTextEdit->clear();
}

void createTupleConstructor::show()
{
    QWidget::show();
    //QDialog::show();
    update_tables_list();
    //qDebug() << "createTupleConstructor's 'QWidget::Show()' overloaded method called.";
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



void createTupleConstructor::closeEvent(QCloseEvent *event)
{

    if(closeMessageFlag_){

        QMessageBox* reply = new QMessageBox(QMessageBox::Warning,"Are you sure?", "Do you want to close constructor window?",QMessageBox::Yes|QMessageBox::No,this);


        reply->setAttribute(Qt::WA_DeleteOnClose,true);
        reply->setWindowModality(Qt::WindowModal);

        QFlag flags = Qt::Window & ~Qt::WindowStaysOnTopHint;
        reply->setWindowFlags(flags);

        reply->show();

        connect(this,&createTupleConstructor::closeNowSig,reply,&QMessageBox::accept);

        auto answer = reply->exec();
        if(answer==QMessageBox::No){


            event->ignore();
            return;
        }

        event->accept();
    }
}



void createTupleConstructor::on_addTupleButton_clicked()
{
    //IS EMPTY check
    if(ui->plainTextEdit->toPlainText().isEmpty()){
        statusBar->get_line()->setText("Tuple is empty. Nothing to add.");
        return;
    } else {
        //EXCLUDING ADDITIONAL BRACKETS FROM SINGLE TURPLE
        QRegularExpression re("^[^()]*$");
        QRegularExpressionValidator v(re, 0);
        QString str = ui->plainTextEdit->toPlainText();
        int pos=0;
        if(v.validate(str, pos)!=QValidator::Acceptable)
        {
            statusBar->get_line()->setText("There's no need for additional brackets in tuple.");
            return;
        } else{

            ////int input_commas = ui->plainTextEdit_2->toPlainText().count(',');
            auto attributesCount = chosenAttributes_.count()-1;
                //qDebug() << "chosenAttributes::" << attributesCount;
            //qDebug() << "number of commas input::"<< input_commas;
            int tuple_commas = ui->plainTextEdit->toPlainText().count(',');
            //qDebug() << "number of commas in tuple::"<<tuple_commas;

            if(/*input_commas*/attributesCount!=tuple_commas){
                statusBar->get_line()->setText("Number of elements in tuple does not match to number of selected attributes.");
                return;
            }

            if(ui->plainTextEdit->toPlainText().simplified().back()==','){
                statusBar->get_line()->setText("Unnecessary comma or missing data after/before comma.");
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

    if(describe_form_!=nullptr)
        describe_form_->close();


    QString table_name = ui->comboBox->currentText();


    describe_form_ = new CustomQueryResult{auth_,this};


    describe_form_-> setAttribute( Qt::WA_DeleteOnClose, true );


    describe_form_->setWindowTitle(table_name);


    describe_form_->custom_query_slot(QString("DESCRIBE `%1`").arg(QString(adb_utility::escape_sql_backticks(table_name)))/*,subconnection_name_*/);

    describe_form_->setModal(false);

    describe_form_->show();

    //describe_form_-> exec();
}


void createTupleConstructor::setCloseMessageFlag(bool state__)
{
    closeMessageFlag_=state__;
}


