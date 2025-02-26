#include "tables.h"
#include "ui_tables.h"

#include "createtupleconstructor.h"

#include "custom_query.h"

int Tables::defaultScaleIndex_ = 0;

QString Tables::defaultFont_ = "Noto Sans,10,-1,0,50,0,0,0,0,0,Regular";

Tables::Tables(auth& auth__,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tables)
  , tableView(new signalTableView)

  , statusBar(new scrolledStatusBar)
  , showTable_button{new /*BlinkinButton{0,"Reload tables list"}*/reloadButton{}}

  , auth_(auth__)

  //, custom_query_result_window_(new CustomQueryResult{auth_})
  , delete_table_window_(new delete_table)
  , constructor_(new CreateTableConstructor{auth_,this})

  , rescaleBoxWidget(adb_utility::getRescaleBox(tableView))
  , rescaleDefaultCheckBox{new QCheckBox{"set default"}}
  , backButton_{new QPushButton{"↩"}}
  , fontWidget_{new fontEmbeddedWidget{tableView}}

  , menuBar_{new QMenuBar{this}}
  , menuFile_{new /*QMenu*/hideMenu{Qt::Key_F10,menuBar_}}
  , exitEntrie_{new QAction{"Exit",menuFile_}}
  , prevEntrie_{new QAction{"Back",menuFile_}} //⮌
{
    ui->setupUi(this);

    init_form();

    init_connections();

    fileOps();

    defaultSettings();
}

Tables::~Tables()
{
    delete ui;

    ////delete custom_query_result_window_;
//    delete settings_;
    delete delete_table_window_;
    delete constructor_;
    delete rescaleBoxWidget;
    delete rescaleDefaultCheckBox;
}

void Tables::closeEvent(QCloseEvent *event)
{


    QPointer <adbMessageBox> quitMessageBox = new adbMessageBox(QMessageBox::Warning,"Return to previous form", "Are you want to leave \""+auth_.db_name_+"\" and return to database choosing form?"
                                                                    "<br>Note: <FONT COLOR='#ff0000'>All user query windows related to the current database (if any) will be closed.</FONT>",
                                                  QMessageBox::Yes | QMessageBox::No,this);

    quitMessageBox->setAttribute( Qt::WA_DeleteOnClose, true);
    //connect(quitMessageBox,&adbMessageBox::destroyed,[]{qDebug() << "~adbMessageBox";});

    QList<QPushButton*> ButtonsInFormlist = quitMessageBox->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj==quitMessageBox->button(QMessageBox::Yes)||obj==quitMessageBox->button(QMessageBox::No)){
                obj->setStyleSheet(QStringLiteral("QPushButton { background: floralwhite; color: darkslategray; font-weight:bold;} %1")
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }
        }

    quitMessageBox->setModal(true);
    quitMessageBox->show();
    int dialogAnswer = quitMessageBox->exec();

    if(dialogAnswer!=QMessageBox::Yes){

      event->ignore();
      return;
    }


    emit db_show();

    emit custom_query_windows_close();

    event->accept();
}


void Tables::init_form()
{
    ui->mainLayout->insertWidget(0,tableView);

    // since rescaleBoxWidget added -- it control the tableView's rescale
    ui->rescaleLayout->addWidget(rescaleBoxWidget);

    move(screen()->geometry().center() - frameGeometry().center());

    ui->statusBarLayout->addWidget(statusBar);

    statusBar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    setWindowIcon(QIcon(":/pic/adb-explorer_logo.png"));
//    ui->reloadLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Preferred, QSizePolicy::Expanding));

    ui->reloadLayout->addWidget(showTable_button);
    showTable_button->setDefault(true);
    showTable_button->setAutoDefault(true);

//    ui->reloadLayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Preferred, QSizePolicy::Expanding));

    QFont button_font = showTable_button->font();
    button_font.setPointSize(12);
    showTable_button->setFont(button_font);

    showTable_button->setText("<u>R</u>eload tables list");

    showTable_button->setBackgroundColour("floralwhite");//("white");

    showTable_button->setFontColour("darkslategray");

    showTable_button->setAlternateBlinkinBackgroundColour("darkslategray");

    showTable_button->setAlternateBlinkinFontColour("snow");

    showTable_button->setBold(true);


    showTable_button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);


    statusBar->get_line()->setReadOnly(true);

    Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                        | Qt::WindowMinimizeButtonHint
                                        | Qt::WindowMaximizeButtonHint
                                        | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);

    ////delete_table_window_->setWindowTitle("Remove table dialog");

    constructor_->setWindowFlag(Qt::Dialog);

    constructor_->setWindowModality(Qt::WindowModal);

    tableView->setSelectionMode(QTableView::SingleSelection);

    ui->rescaleLayout->addWidget(rescaleDefaultCheckBox);

    rescaleDefaultCheckBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    backButton_->setStyleSheet("color:white;background-color:green;font-size:14pt;padding-left:6px;padding-right:6px;");

    QFrame* backFrame = new QFrame;
    backFrame->setFrameShape(QFrame::StyledPanel);
    backFrame->setFrameShadow(QFrame::Raised);
    QHBoxLayout* backLay = new QHBoxLayout{backFrame};
    backLay->setContentsMargins(1,1,1,1); backLay->setSpacing(0);
    backLay->addWidget(backButton_);
    ui->backLayout->addWidget(backFrame);

    ui->fontLayout->addWidget(fontWidget_);

//    ui->verticalDeleteTableLayout->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    ui->verticalDeleteTableLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
//    ui->verticalSettingsLayout->setAlignment(Qt::AlignHCenter|Qt::AlignBottom);
    ui->verticalSettingsLayout->setAlignment(Qt::AlignHCenter|Qt::AlignTop);

    fontWidget_->setStyleSheet("QPushButton#fontButton {color: darkslategray; background: #fffffa;}");



    QList<QComboBox*> comboBoxInFormlist = this->findChildren<QComboBox*>();
        foreach (auto obj, comboBoxInFormlist) {

                obj->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#fffffa","darkslategray"));
        }

    ui->verticalSettingsLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->verticalDeleteTableLayout->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    ui->mainLayout->insertWidget(0,menuBar_);

    menuFile_->setTitle("File");
    menuFile_->addAction(prevEntrie_);
    menuFile_->addSeparator();
    menuFile_->addAction(exitEntrie_);

    exitEntrie_->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    prevEntrie_->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowBack));

    menuBar_->addMenu(menuFile_);

    menuBar_->hide();

    ui->select_from_table_button->setStyleSheet("QPushButton:!disabled {background: darkslategray; color: snow; font-weight:bold; padding: 6px;}");
    ui->select_from_table_button->setEnabled(true);

    ui->DescribeButton->setStyleSheet("QPushButton:!disabled {color:darkslategray; background: floralwhite; font-weight:bold; padding: 5px;}");
    ui->DescribeButton->setEnabled(true);

    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            // if button is inherited class object -> cast pointer to inherit type
            if(obj->objectName()=="reloadButtonObj"){
//                qobject_cast<reloadButton*>(obj)->setKhakiHighlight();
                auto reloadButtonObj = qobject_cast<reloadButton*>(obj);
                reloadButtonObj->setKhakiHighlight();

            } else if(obj->objectName().contains("help_button_")){
                obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS(adb_style::buttonKhakiHighlightTextHoverColor_,adb_style::buttonKhakiHighlightTextFocusColor_,0)));
            }else if(obj->objectName().contains("describe_tbl_")||obj->objectName().contains("plus_button_")){
                obj->setStyleSheet(QStringLiteral("%1 %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS(adb_style::buttonKhakiHighlightTextHoverColor_,adb_style::buttonKhakiHighlightTextFocusColor_,0)));
            }else if(obj->objectName()=="select_from_table_button" || obj->objectName()=="DescribeButton"){
                obj->setStyleSheet(QStringLiteral("%1 %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }else {
                obj->setStyleSheet(QStringLiteral("QPushButton {%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }

        ui->select_from_table_button->setFocus();

        tableView->setAlternatingRowColors(true);

        tableView->setPalette(QPalette(adb_style::whiteUndGrayColor));


        QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
            foreach (auto obj, checkBoxInFormlist) {

                    obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
            }


}

void Tables::fileOps()
{
    if(!read4rom_settings_file())
        write2_settings_file();
}

void Tables::defaultSettings()
{
    // set default font
    QFont __font;

    __font.fromString(Tables::defaultFont_);
    tableView->setFont(__font);

    // set default settings
    rescaleBoxWidget->findChild<notifyComboBox*>()->setCurrentIndex(Tables::defaultScaleIndex_);
}

void Tables::keyPressEvent(QKeyEvent *e__)
{
    auto key = e__->key();
    if(key == Qt::Key_Escape){
        close();
    } else if(key == Qt::Key_F10){

        if(!menuBar_->isVisible()){
            menuBar_->show();
        } else{
            menuBar_->hide();
        }

    } else {/* minimize */
        //qDebug()<<"escape pressed (tables)";
        QDialog::keyPressEvent(e__);
    }
}

void Tables::init_connections()
{
    connect(this,SIGNAL(delete_form_request()),delete_table_window_,SLOT(delete_form_request_slot()));

    connect(delete_table_window_,&delete_table::delete_form_send,[this](QComboBox* comboBox__){

        QString const __queryText = "SHOW TABLES";

        delete_table_window_->setComboBoxLoadQueryText(__queryText);

        connect(delete_table_window_,&delete_db::reloadSig,[this,comboBox__]{

            db_connection::open(auth_);

            db_connection::set_query(delete_table_window_->comboBoxLoadQueryText(),&model_,comboBox__);

            comboBox__->setCurrentIndex(-1); //for blank cell default
        });

        delete_table_window_->reload();
    });

    connect(delete_table_window_,&delete_table::delete_entity,[=](QComboBox*comboBox__){
        db_connection::open(auth_);

        QString const chosen_table = comboBox__->currentText();
        QString const query_text = QString("DROP TABLE `%1`").arg(QString(adb_utility::escape_sql_backticks(chosen_table)));

        if(!db_connection::set_query(query_text,&model_,comboBox__))
        {
//            QMessageBox::warning(this,"Warning","Table is not dropped. May be it was been droped earlier?");
            statusBar->get_line()->setText(QString("(x)Table `%1` is not dropped.").arg(chosen_table));
            qDebug() << QString("(x)Table `%1` is not dropped.").arg(chosen_table);
        } else {
                qDebug() << QString("Table `%1` successfully deleted.").arg(chosen_table);
                statusBar->get_line()->setText(QString("(✓)Table `%1` successfully deleted.").arg(chosen_table));
        }
        show_tables();      // view updated tables list after table deletion
        //select_cells(0,0, tableView);
    });

    // transfer tables list to 'create table' constructor
    connect(this,SIGNAL(current_tables_list_signal(QList<QString>)),constructor_,SLOT(current_exist_tables_slot(QList<QString>)));

    //CUSTOM CREATE TABLE CONSTRUCTOR
    connect(constructor_,
            static_cast<void (CreateTableConstructor::*)(QString const&,QString const&)>(&CreateTableConstructor::send_custom_query),
            [=](QString const&query__,QString const&newtable_name__){
        db_connection::open(auth_);

        if(db_connection::set_query(query__,&model_,tableView/*,QHeaderView::Stretch*/)) //TABLE MAIN WINDOW MODEL
            emit constructor_query_success(newtable_name__);
        else
            emit constructor_query_fails(newtable_name__);
        show_tables();
    });

    // quit from constructor without warning
    connect(this,&Tables::constructor_query_success, [=] (QString const& newtable_name__) {

        QString const text = QString("(✓)New table `%1` successfully created by `Create Table Constructor`.").arg(newtable_name__);
        qDebug() << text;
        statusBar->get_line()->setText(text);

        constructor_->set_warning_flag_(false);
        constructor_->close();
        constructor_->set_warning_flag_(true);

    });

    // if final query from table constructor fails while exec.
    connect(this,&Tables::constructor_query_fails, this,[=](QString const& newtable_name__){

        constructor_->constructor_query_fails_handle();
        QString const text = QString("(x)`Create table constructor` query fails. Table `%1` is not created.").arg(newtable_name__);
        qDebug() << text;
        statusBar->get_line()->setText(text);

    },Qt::QueuedConnection);


    connect(showTable_button,&QPushButton::clicked,this,&Tables::show_tables);

    // disable some buttons if tables list empty && stop timer for custom blinkin button
    connect(this,&Tables::tables_reloaded,[=]{

        if(!tableView->currentIndex().data().toString().isEmpty()){

            //ui->select_from_table_button->setStyleSheet(QStringLiteral(" QPushButton {color:darkslategray;background: white; font-weight:bold;padding: 6px;} %1").arg(adb_style::getbuttonKhakiHiglightSS()/*ui->select_from_table_button->styleSheet()*/));
            ui->select_from_table_button->setEnabled(true);

            //ui->DescribeButton->setStyleSheet(QStringLiteral(" QPushButton {background: darkslategray; color: snow;padding: 6px;} %1").arg(adb_style::getbuttonKhakiHiglightSS()/*ui->DescribeButton->styleSheet()*/));
            ui->DescribeButton->setEnabled(true);

        } else {

            //ui->select_from_table_button->setStyleSheet("background: palette(window)");
            ui->select_from_table_button->setEnabled(false);

            //ui->DescribeButton->setStyleSheet("background: palette(window)");
            ui->DescribeButton->setEnabled(false);

        }

        showTable_button->stop_blinkin();
    });

    // if separate select, describe and custom query settings disabled, disable some buttons after query
    connect(this,&Tables::disable_select_until_reload,[=]{
        ui->select_from_table_button->setEnabled(false);
        ui->DescribeButton->setEnabled(false);
        showTable_button->start_blinkin(890,25000);
    });

    connect(tableView,&QTableView::clicked,[=](const QModelIndex & index){

        tableView->setCurrentIndex(index);

        statusBar->get_line()->clear();
    });

    connect(tableView,&QTableView::activated,[=](const QModelIndex & index){

        tableView->setCurrentIndex(index);

        statusBar->get_line()->clear();
    });

    connect(tableView,&QTableView::doubleClicked,[=](const QModelIndex & index){

        tableView->setCurrentIndex(index);

        statusBar->get_line()->clear();

        ui->select_from_table_button->click();
    });

    connect(ui->select_from_table_button,&QPushButton::clicked,this,&Tables::show_table_content);


    connect(ui->Custom_Query_Button,&QPushButton::clicked,[this]{
        get_custom_query_window_("",true);
    }); //this, /*static_cast<void (Tables::*)()>(&Tables::get_custom_query_window_)*/&Tables::get_custom_query_window_);

    // load settings from file and set new by gui form
    connect(ui->query_settings_button,&QPushButton::clicked,[this]{
        if(!settings_){
            if((settings_ = new CustomQuerySettings)){

                settings_->setWindowTitle("Tables::Settings");

                settings_->setAttribute(Qt::WA_DeleteOnClose,true);


                QMap<QString,int> __settings_map;

                adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map);

                settings_->setForeignSettingsOnForm(__settings_map);

                connect(settings_,static_cast<void (CustomQuerySettings::*)(QMap<QString,int>,bool)>(&CustomQuerySettings::settingsExportSig),
                        [this](QMap<QString,int>settings_map__,bool changes__)
                {

                    if(changes__){

                        importSettings(settings_map__);

                        write2_settings_file();
                    }

                });

                connect(this,&Tables::custom_query_windows_close,[this]{ if(settings_)settings_->close();});
            }
        }
        if(settings_){
            settings_->show();
            settings_->raise();
        }
    });

    connect(ui->delete_table_button,&QPushButton::clicked,[=]{
        delete_table_window_->setModal(true);
        delete_table_window_->show();
        emit delete_form_request();
    });

    connect(ui->create_table_button,&QPushButton::clicked, this, &Tables::get_table_constructor);

    connect(ui->insert_inTable_button,&QPushButton::clicked,this, &Tables::get_tuple_constructor_instance);


    connect(ui->DescribeButton,&QPushButton::clicked,this,&Tables::/*get_describe_table_instance*/show_table_description);


    connect(this,&Tables::empty_set,[=]{
        if(MSG_SHOW_IF_BLANK_RESULT)
        {
            adb_utility::get_information_window(QMessageBox::Information,"Empty set","(✓) [Note] :: Query result not contain/doesn't imply"
                                   " contain of displayable result.", this);
        }
    });


    //QObject::connect(rescaleDefaultCheckBox,&QCheckBox::destroyed,[=]{ qDebug() << "~Tables::ui->rescaleLayout::rescaleDefaultCheckBox"; });


    QPointer</*QComboBox*/notifyComboBox> rescaleComboBox = rescaleBoxWidget->findChild<notifyComboBox*>();


    connect(rescaleComboBox,&notifyComboBox::sameIndexRepeated,[=](){
        rescaleComboBox->currentIndexChanged(rescaleComboBox->currentIndex());
    });

    connect(rescaleComboBox,static_cast<void(/*QComboBox*/notifyComboBox::*)(int)>(&/*QComboBox*/notifyComboBox::currentIndexChanged),[=](int newIndex__){
        if(newIndex__==Tables::defaultScaleIndex_){
            rescaleDefaultCheckBox->setChecked(true);
            rescaleDefaultCheckBox->setEnabled(false);
        } else{
            rescaleDefaultCheckBox->setChecked(false);
            rescaleDefaultCheckBox->setEnabled(true);
        }
    });

    connect(rescaleDefaultCheckBox,&QCheckBox::stateChanged,[=](int state__){
        if(state__){
            int currentBoxIndex = rescaleComboBox->currentIndex();
            if(currentBoxIndex!=Tables::defaultScaleIndex_){
                //tableScaleChanged=true;
                Tables::defaultScaleIndex_ = currentBoxIndex;
                rescaleDefaultCheckBox->setEnabled(false);

                write2_settings_file();
            }
        }
    });

    connect(backButton_,&QPushButton::clicked,this,&Tables::close);

    connect(fontWidget_,&fontEmbeddedWidget::defaultFontChanged,[this](QString const& newDefaultFont_){
        Tables::defaultFont_=newDefaultFont_;
        write2_settings_file();
    });

    connect(menuFile_,&hideMenu::menuHiden,menuBar_,&QMenuBar::hide);


    connect(exitEntrie_, &QAction::triggered,[this]{

        if(!adb_utility::showExitAppDialog(this)){

            return;
        } else {

            this->custom_query_windows_close();
            QApplication::exit(0);
        }

    });


    connect(prevEntrie_, &QAction::triggered, this, &Tables::close);
}




void Tables::show_tables()
{
//QElapsedTimer timer;
//timer.start();


    db_connection::try_to_reopen(auth_);


    if(db_connection::set_query("SHOW TABLES;",&model_,tableView/*,QHeaderView::Stretch*/)){

        if((tableView->model())!=nullptr){

            select_cells(0,0, tableView);

            //qDebug() << "Number of existing tables::" <<(model_.rowCount());

            //qDebug()<< "tableView->currentIndex().data().toString()::" <<tableView->currentIndex().data().toString();

        }

        emit tables_reloaded();
    }

//qDebug() << "The Set query model operation took" << timer.elapsed() << "milliseconds";
//qDebug() << "The Set query model operation took" << timer.nsecsElapsed() << "nanoseconds";
}



void Tables::send_custom_query_slot(/*QString query__,*/Custom_Query *custom_query_window__)
{

    auto QueryRawText = custom_query_window__->get_text();

    auto QueriesList = adb_utility::unpack_(QueryRawText,";",true);

    auto queryFailFlag = false;

    auto skipAfterFail = false;

    // check flag of separate window for user's query result
    if(!separate_query_window){

        if(db_connection::open(auth_)){

            for(auto i=0;i!=QueriesList.size();++i){

                if(skipAfterFail){
                    custom_query_window__->save_query(QUERYSTATUS::NOTSENDED,QueriesList.at(i));
                    continue;
                }

//                std::cout << QueriesList.at(i).toStdString() << " ";// << std::endl;
//                //qDebug() << QueriesList.at(i);
//                std::cout << QueriesList.at(i).toStdString().size() << std::endl;

                if(db_connection::set_query(QueriesList.at(i),&model_,tableView/*,QHeaderView::Stretch*/)){

                    //custom_query_window__->close();
                    custom_query_window__->save_query(QUERYSTATUS::SUCCESS,QueriesList.at(i));


                    if(!tableView->model()->rowCount()&&!BLANK_RESULT) {

                        show_tables();

                        statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                                " contain of displayable result. Current DB tables list reloaded.");

                        emit empty_set();

                    } else{
                        emit disable_select_until_reload();
                        statusBar->get_line()->clear();
                    }

                } else{

                    custom_query_window__->save_query(QUERYSTATUS::FAILED,QueriesList.at(i));

                    if(showNoteIfNotSeparateWindowResult){

                        auto notSeparateWindowNoteName = "notSeparateWindowNote";

                        auto previousNotSeparateWindowNote = custom_query_window__->findChild<noteFrame*>(notSeparateWindowNoteName);

                        // remove previous note
                        if(previousNotSeparateWindowNote)
                            previousNotSeparateWindowNote->close();

                        noteFrame* notSeparateWindowNotePtr = custom_query_window__->add_note(
                                    QStringLiteral("*Note: Your successfully proceeded query results places in '%1' window. "
                                                                "You can switch this behaviour in settings.").arg(this->metaObject()->className()));

                        notSeparateWindowNotePtr->setObjectName(notSeparateWindowNoteName);

                        connect(notSeparateWindowNotePtr,&noteFrame::dontShowNoteAgainSig,[this]{
                            showNoteIfNotSeparateWindowResult = false;
                            write2_settings_file();
                        });
                    }

                    queryFailFlag = true;

                    if(MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL){

                        skipAfterFail = true;
                        continue;
                    }
                }
            }
        }

    } else{

        for(auto i=0;i!=QueriesList.size();++i){

            if(skipAfterFail){
                custom_query_window__->save_query(QUERYSTATUS::NOTSENDED,QueriesList.at(i));
                continue;
            }

//            std::cout << QueriesList.at(i).toStdString() << " ";// << std::endl;
//            //qDebug() << QueriesList.at(i);
//            std::cout << QueriesList.at(i).toStdString().size() << std::endl;


            QPointer<CustomQueryResult> new_result_window{new CustomQueryResult{auth_,nullptr,true}};

            auto newTitle = this->metaObject()->className() + QStringLiteral("::") +new_result_window->windowTitle()+QStringLiteral(" № %1").arg(i);

            new_result_window->setWindowTitle(newTitle);

            new_result_window->setAttribute(Qt::WA_DeleteOnClose, true);

            // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
            connect(this,&Tables::custom_query_windows_close, new_result_window, &CustomQueryResult::closeNowSig, Qt::QueuedConnection);

            new_result_window->custom_query_slot(QueriesList.at(i));

            if((new_result_window->tableView->model())!=nullptr) {


                custom_query_window__->save_query(QUERYSTATUS::SUCCESS,QueriesList.at(i));


                if(!new_result_window->tableView->model()->rowCount()&&!BLANK_RESULT){


                    statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                            " contain of displayable result.");

                    emit empty_set();

                    new_result_window->deleteLater();

                } else{

                    statusBar->get_line()->clear();

                    new_result_window->show();
                }

            }else {

                custom_query_window__->save_query(QUERYSTATUS::FAILED,QueriesList.at(i));

                queryFailFlag = true;

                if(MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL){
                    //break;
                    skipAfterFail = true;
                    continue;
                }
            }

        }

    }


    if(QueriesList.size()){

        if(!queryFailFlag){
            custom_query_window__->close();

        } else{

            if(queryFailNote){

                auto failNoteName = "failNote";

                auto previousFailNote = custom_query_window__->findChild<noteFrame*>(failNoteName);

                // remove previous (duplicate) fail note to prevent window's notes-spamming
                if(previousFailNote)
                    previousFailNote->close();

                noteFrame* failNotePtr = custom_query_window__->add_note("*Note: Error/s occurred while proceed your SQL transaction/s.");

                failNotePtr->setObjectName(failNoteName);

                connect(failNotePtr,&noteFrame::dontShowNoteAgainSig,[this]{
                    queryFailNote = false;
                    write2_settings_file();
                });

            }

        }
    }
}



void Tables::get_custom_query_window_(QString const& __pre_query, bool closeMessage)
{
        Custom_Query custom_query_window{nullptr,closeMessage};

            connect(&custom_query_window,static_cast<void(Custom_Query::*)(/*QString,*/Custom_Query*)>(&Custom_Query::send_custom_query),
                    this,[&custom_query_window,this](Custom_Query* customQueryWndw__){
                custom_query_window.setCheckCloseMessageFlag(false);
                send_custom_query_slot(customQueryWndw__);
                custom_query_window.setCheckCloseMessageFlag(true);
            });

        connect(this,&Tables::custom_query_windows_close, &custom_query_window , &Custom_Query::closeNowSig);

        if(!__pre_query.isEmpty())
            custom_query_window.set_text(__pre_query);

        custom_query_window.setModal(false);
        custom_query_window.show();
        custom_query_window.exec();
}



void Tables::show_table_content()
{

    QString const curIndStr = adb_utility::escape_sql_backticks(tableView->currentIndex().data().toString());

    if(!separate_content_window){

        db_connection::open(auth_);


        db_connection::set_query(QString("SELECT * FROM ")+/*auth_.table_name_*/curIndStr+(";"),&model_,tableView/*,QHeaderView::Stretch*/);


        if(!tableView->model()->rowCount()&&!BLANK_RESULT) {
            show_tables();

            statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                    " contain of displayable result. Current DB tables list reloaded.");

            emit empty_set();

        } else{

            emit disable_select_until_reload();
            statusBar->get_line()->clear();
        }



    }else{

        CustomQueryResult new_select_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_select_window, &Custom_Query::close);

        new_select_window.setWindowTitle(/*auth_.table_name_*/curIndStr+": table data");

        new_select_window.custom_query_slot(QString("SELECT * FROM ")+/*auth_.table_name_*/curIndStr+(";"));

        if((new_select_window.tableView->model())!=nullptr) {


            if(!new_select_window.tableView->model()->rowCount()&&!BLANK_RESULT){

                statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                        " contain of displayable result.");

                emit empty_set();

            } else{

            statusBar->get_line()->clear();

            new_select_window.show();
            new_select_window.exec();

            }

        }

    }

}



void Tables::get_table_constructor()
{
    if(!constructor_->isVisible()){

        constructor_->setCurrentIndex(0);

        // UNIQUE NAME CHECK for constructor [
        QList<QString> list_to_send;

        size_t size_of_list_=tableView->model()->rowCount();


        for(size_t i=0;i!=size_of_list_;++i)
            list_to_send.append(tableView->model()->index(i,0).data().toString());


        emit current_tables_list_signal(list_to_send);

        // ]

        //CONSTRUCTOR WINDOW AT THE CENTER OF TABLE WINDOW
        adb_utility::window_center_from_another_(this,constructor_);

        constructor_->setWindowFlag(Qt::Dialog);

        constructor_->show();

    } else {

        constructor_->raise();

    }
}



void Tables::get_tuple_constructor_instance()
{
    if(!tableView->model()->rowCount())
    {

        adb_utility::get_information_window(QMessageBox::Information,"Current database is empty","Current database is empty. You must create at least 1 table to insert data,"
                                                            " or choose another DB.",this);
        return;
    }

    QPointer<createTupleConstructor> constr_window_{new createTupleConstructor{auth_,nullptr/*,this*/}};


    connect(constr_window_, static_cast<void (createTupleConstructor::*) (QString const &)>(&createTupleConstructor::final_query_sig),
            this,[this](QString const& preQuery__){
        get_custom_query_window_(preQuery__,true);
    }); //static_cast<void (Tables::*) (QString const &)>(&Tables::get_custom_query_window_));

    connect(this,&Tables::custom_query_windows_close,constr_window_, &createTupleConstructor::closeNowSig);



    constr_window_->setAttribute( Qt::WA_DeleteOnClose, true );

    constr_window_->show();

}



void Tables::show_table_description()
{
    QString const curIndStr = adb_utility::escape_sql_backticks(tableView->currentIndex().data().toString());
    QString query_text = "DESC "+curIndStr+';';

    if(!separate_describe_window){

        db_connection::open(auth_);

        db_connection::set_query(query_text,&model_,tableView);


        if(!tableView->model()->rowCount() && !BLANK_RESULT) {
            show_tables();

            statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                    " contain of displayable result. Current DB tables list reloaded.");

            emit empty_set();

        } else{

            emit disable_select_until_reload();
            statusBar->get_line()->clear();

        }

    }else{

        CustomQueryResult new_select_window{auth_};

        // correct closing when 'Table' window closed; preventing crashing while switching between DBs in QSqlDatabase connection
        connect(this,&Tables::custom_query_windows_close, &new_select_window, &Custom_Query::close);

        new_select_window.setWindowTitle(/*auth_.table_name_*/curIndStr+": detail table info");

        new_select_window.custom_query_slot(query_text);


        if((new_select_window.tableView->model())!=nullptr) {


            if(!new_select_window.tableView->model()->rowCount()&&!BLANK_RESULT){


                statusBar->get_line()->setText("(✓) [Note] :: Query result not contain/doesn't imply"
                                        " contain of displayable result.");

                emit empty_set();

            } else{

                statusBar->get_line()->clear();

                new_select_window.show();
                new_select_window.exec();
            }

        }
    }

}



void Tables::importSettings(QMap<QString, int> settings_map__, QMap<QString, QString> settings_map_str__)
{
    int temp;

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_content_window]))!=-1)
        separate_content_window = temp;

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_describe_window]))!=-1)
        separate_describe_window = temp;

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_query_window]))!=-1)
        separate_query_window = temp;

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::BLANK_RESULT]))!=-1)
        BLANK_RESULT = temp;

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::MSG_SHOW_IF_BLANK_RESULT]))!=-1)
        MSG_SHOW_IF_BLANK_RESULT = temp;

    if((temp = settings_map__.value(CustomQuerySettings::paramEnumToStr[tblQuerSet::MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL]))!=-1)
        MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL = temp;

    if((temp = settings_map__.value("defaultScaleMode"))!=-1){
        Tables::defaultScaleIndex_=temp;
        rescaleBoxWidget->findChild<notifyComboBox*>()->setCurrentIndex(temp);
    }

    if((temp = settings_map__.value("showNoteIfNotSeparateWindowResult")!=-1))
        showNoteIfNotSeparateWindowResult = temp;

    if((temp = settings_map__.value("queryFailNote")!=-1))
        queryFailNote = temp;

    //------------------------------------------------------------------

    if(!settings_map_str__.isEmpty()){
        QString temp_s;

        if((temp_s = settings_map_str__.value("defaultTableFont"))!="")
            Tables::defaultFont_ = temp_s;
    }

}


void Tables::mousePressEvent(QMouseEvent *event)
{
    auto key = event->button();
    if(key == Qt::LeftButton){
        statusBar->get_line()->clear();
    }
    QDialog::mousePressEvent(event);
}



bool Tables::read4rom_settings_file()
{
    int __res = 0;

    QMap<QString,int> __settings_map;

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map))
        __res++;
    else{

        qWarning() << "Error while read from"<<settings_f_name_;
    }
    //------------------------------------------------------------------

    QMap<QString,QString> __settings_map_str;

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map_str))
        __res++;
    else{

        qWarning() << "Error while read from"<<settings_f_name_<<"(Strings)";
    }

    if(__res)
        importSettings(__settings_map,__settings_map_str);

    return __res;
}



void Tables::write2_settings_file()
{
    QFile outFile(settings_f_name_);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_content_window] << '=' << QString::number(separate_content_window) << Qt::endl;
    textStream << CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_describe_window] << '=' << QString::number(separate_describe_window) << Qt::endl;
    textStream << CustomQuerySettings::paramEnumToStr[tblQuerSet::separate_query_window] << '=' << QString::number(separate_query_window) << Qt::endl;
    textStream << CustomQuerySettings::paramEnumToStr[tblQuerSet::BLANK_RESULT] << '=' << QString::number(BLANK_RESULT) << Qt::endl;
    textStream << CustomQuerySettings::paramEnumToStr[tblQuerSet::MSG_SHOW_IF_BLANK_RESULT] << '=' << QString::number(MSG_SHOW_IF_BLANK_RESULT) << Qt::endl;
    textStream << CustomQuerySettings::paramEnumToStr[tblQuerSet::MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL] << '=' << QString::number(MULTIPLY_USER_QUERIES_TERMINATE_AFTER_FAIL) << Qt::endl;
    textStream << "defaultScaleMode" << '=' << QString::number(Tables::defaultScaleIndex_) << Qt::endl;
    textStream << "defaultTableFont" << '=' << '\"'+Tables::defaultFont_+'\"' << Qt::endl;
    textStream << "showNoteIfNotSeparateWindowResult" << '=' << QString::number(showNoteIfNotSeparateWindowResult) << Qt::endl;
    textStream << "queryFailNote" << '=' << QString::number(queryFailNote) << Qt::endl;
}


