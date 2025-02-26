#include "custom_query.h"
#include "ui_custom_query.h"

QString Custom_Query::font_ = "Noto Sans,10,-1,0,50,0,0,0,0,0,Regular";

bool Custom_Query::settingsFileReady_ = true;

/*bool*/dynamicbool Custom_Query::askBeforeClose_ = true;

int Custom_Query::userQueriesHistoryLengthLines_ = 1000;

QString const Custom_Query::userQueriesHistoryBinFileName_ = adb_utility::filepath_+"/.usqrhist.bin";

Custom_Query::Custom_Query(QWidget *parent, bool closeMessage/*, QFont font*/)
    : QDialog(parent)
    , ui(new Ui::Custom_Query)
    , checkCloseMessageFlag_(closeMessage)
    , userQueriesHistoryBut_{new QPushButton{"Queries history"}}

    , menuBar_{new QMenuBar{this}}
    , menuFile_{new hideMenu{Qt::Key_F10,menuBar_}}
    , saveQueryEntrie_{new QAction{"Save Query",menuFile_}}
    , exitEntrie_{new QAction{"Exit",menuFile_}}
{
    ui->setupUi(this);
    defaultFontWidget_ = new fontEmbeddedWidget{ui->plainTextEdit};

    fileOps();

    setCheckCloseMessageFlag(closeMessage);

    connections_init();

    form_init();

}

Custom_Query::~Custom_Query()
{
    //qDebug()<<"~CustomQuery activated";
    delete ui;
}

void Custom_Query::fileOps()
{
    // get/set file data only once (for multiply windows speed)
    if(Custom_Query::settingsFileReady_){
        // if read file unsuccessful -- write default settings to file
        if(!read4rom_settings_file()){
            write2_settings_file();
        }
        Custom_Query::settingsFileReady_=false;
    }
}



void Custom_Query::connections_init()
{
    // syncronize settings changes with file
    connect(defaultFontWidget_,&fontEmbeddedWidget::defaultFontChanged,[this](QString const& newDefaultFont_){
        Custom_Query::font_=newDefaultFont_;
        write2_settings_file();
    });


    // send result of query exec. to form
    connect(ui->Ok_button,&QPushButton::clicked,[=]{
        emit send_custom_query(/*ui->plainTextEdit->toPlainText(),*/this);
    });

    //{before}

    connect(ui->closeCheckBox,&QCheckBox::stateChanged,[this](int newState__){
        Custom_Query::askBeforeClose_ = newState__;
        write2_settings_file();
    });


    connect(ui->Cancel_button,&QPushButton::clicked,this,&Custom_Query::close);


    connect(ui->plainTextEdit,&QPlainTextEdit::textChanged,[this]{ ui->Ok_button->setEnabled(!ui->plainTextEdit->toPlainText().isEmpty());});

    connect(this,&Custom_Query::closeNowSig,[this]{
        checkCloseMessageFlag_=false;
        reject();
    });

    connect(userQueriesHistoryBut_, &QPushButton::clicked,this, &Custom_Query::showQueriesHistory);

    connect(menuFile_,&hideMenu::menuHiden,menuBar_,&QMenuBar::hide);

    connect(saveQueryEntrie_, &QAction::triggered,[this]{
        if(!ui->plainTextEdit->toPlainText().isEmpty()){
            save_query(QUERYSTATUS::NOTSENDED);
        }
    });

    connect(exitEntrie_, &QAction::triggered,[this]{
        close();
    });

    Custom_Query::askBeforeClose_.synchronizeCheckBox(ui->closeCheckBox);
}



void Custom_Query::form_init()
{
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);


    this->ui->Ok_button->setStyleSheet("QPushButton {background:green; color:white;} QPushButton:disabled {background-color:gray;}");


    defaultFontWidget_->setStyleSheet("QPushButton#fontButton{color: darkslategray; background: #fffffa;}");


    QFrame* fontWidgetFrame = new QFrame;
    fontWidgetFrame->setContentsMargins(0,0,0,0);

    fontWidgetFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

    fontWidgetFrame->setFrameShape(QFrame::StyledPanel);
    fontWidgetFrame->setFrameShadow(QFrame::Sunken);

    QHBoxLayout* fontWidgetLay = new QHBoxLayout{fontWidgetFrame};
    fontWidgetLay->setSpacing(1);
    fontWidgetLay->setContentsMargins(1,1,1,1);
    fontWidgetLay->setAlignment(Qt::AlignLeft);

    fontWidgetLay->addWidget(defaultFontWidget_);

    ui->buttonsLayout->insertWidget(0,fontWidgetFrame);
    ui->buttonsLayout->insertStretch(1);

    QFrame* userQueriesHistorySubFrame{new QFrame{}};
    userQueriesHistorySubFrame->setFrameShape(QFrame::StyledPanel);
    userQueriesHistorySubFrame->setFrameShadow(QFrame::Sunken);
    userQueriesHistorySubFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);


    QHBoxLayout* userQueriesHistoryLay{new QHBoxLayout{userQueriesHistorySubFrame}};

    if(plugins::cryptoModule){
        userQueriesHistoryBut_->setStyleSheet("color:white;background:orange;");
    } else{
        userQueriesHistoryBut_->setStyleSheet("background-color:gray;");
        userQueriesHistoryBut_->setEnabled(false);
        userQueriesHistoryBut_->setToolTip("Crypto plugin is not available");
    }

    auto buttonFont = userQueriesHistoryBut_->font();
    //buttonFont.setPointSize(8);
    buttonFont.setBold(true);
    userQueriesHistoryBut_->setFont(buttonFont);

    userQueriesHistoryLay->addWidget(userQueriesHistoryBut_);
    userQueriesHistoryLay->setSpacing(1);
    userQueriesHistoryLay->setContentsMargins(1,1,1,1);

    ui->buttonsLayout->insertStretch(2);
    ui->buttonsLayout->insertStretch(4);
    ui->buttonsLayout->insertWidget(5,userQueriesHistorySubFrame);
    ui->buttonsLayout->insertStretch(6);


    ui->Cancel_button->setText(/*"⌧"*/QString::fromUtf8("\U00002715"));
    ui->Cancel_button->setStyleSheet("background-color:darkred;color:white;font-size:14pt;padding-top:1px;padding-bottom:1px;");//padding-top:1px;padding-bottom:1px;");

    ClickableLabel* dotLbl = new ClickableLabel{": :"};
    dotLbl->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    dotLbl->setStyleSheet("font-weight: bold;");
    ClickableLabel* dotLbl2 = new ClickableLabel{": :"};
    dotLbl2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    dotLbl2->setStyleSheet("font-weight: bold;");
    ui->buttonBoxLayout->insertWidget(1,dotLbl);
    ui->buttonBoxLayout->insertWidget(3,dotLbl2);
    connect(dotLbl,&ClickableLabel::clicked,[this]{ ui->Cancel_button->clicked(); });
    connect(dotLbl2,&ClickableLabel::clicked,[this]{ ui->Cancel_button->clicked(); });

    ui->Ok_button->setEnabled(false);

    ui->okLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            if(obj->objectName()=="Ok_button"){
                obj->setStyleSheet(QStringLiteral("%1 %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));

            }else{
                obj->setStyleSheet(QStringLiteral("QPushButton{%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }

    QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
        foreach (auto obj, checkBoxInFormlist) {

                obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
        }

        // set default font
        QFont __font;

        __font.fromString(Custom_Query::font_);
        ui->plainTextEdit->setFont(__font);

        ui->plainTextEdit->setFocus();


        ui->verticalLayout->insertWidget(0,menuBar_);

        menuFile_->setTitle("File");

        menuFile_->addAction(saveQueryEntrie_);
        saveQueryEntrie_->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton));

        menuFile_->addAction(exitEntrie_);
        exitEntrie_->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));

        menuBar_->addMenu(menuFile_);

        menuBar_->hide();
}



void Custom_Query::reject()
{
    if(Custom_Query::askBeforeClose_ && checkCloseMessageFlag_){
        QMessageBox* questionBox = new QMessageBox(QMessageBox::Question,"Closing","The window will close. Continue?",QMessageBox::Ok | QMessageBox::Cancel,this);

        QCheckBox* notAskAgainCheckBox = new QCheckBox{"Always ask before close",questionBox};
        notAskAgainCheckBox->setChecked(Custom_Query::askBeforeClose_);
        questionBox->setCheckBox(notAskAgainCheckBox);

        connect(notAskAgainCheckBox,&QCheckBox::stateChanged,[this](int newState__){
            //Custom_Query::askBeforeClose_= newState__;
            ui->closeCheckBox->setChecked(newState__);
        });

        Custom_Query::askBeforeClose_.synchronizeCheckBox(notAskAgainCheckBox);

        questionBox->setAttribute(Qt::WA_DeleteOnClose,true);
        questionBox->setWindowModality(Qt::WindowModal);

        QFlag flags = Qt::Window & ~Qt::WindowStaysOnTopHint;
        questionBox->setWindowFlags(flags);

        questionBox->show();

        connect(this,&Custom_Query::closeNowSig,questionBox,&QMessageBox::accept);

        auto answer = questionBox->exec();
        if(answer==QMessageBox::Ok){
            QDialog::reject();
        }
    } else{
        QDialog::reject();
    }
}

void Custom_Query::keyPressEvent(QKeyEvent *e) {
    auto key = e->key();
    if(key == Qt::Key_F10){
        if(!menuBar_->isVisible()){
            menuBar_->show();
        } else{
            menuBar_->hide();
        }
    } else{
        QDialog::keyPressEvent(e);
    }
}



void Custom_Query::setCheckCloseMessageFlag(bool state__)
{
    if((checkCloseMessageFlag_=state__)){
        ui->closeCheckBox->show();
        ui->closeCheckBox->setEnabled(true);
        ui->closeCheckBox->setChecked(Custom_Query::askBeforeClose_);
    } else{
        ui->closeCheckBox->setEnabled(false);
        ui->closeCheckBox->hide();
    }
}

/*static*/ bool Custom_Query::removeUserQueriesHistory()
{
    if(adb_utility::fileExists_(userQueriesHistoryBinFileName_)){

        if(QFile::remove(userQueriesHistoryBinFileName_)){

            auto text = "SQL queries history file has been successfully removed.";
            qDebug() << text;
            std::cout << text << std::endl;

            return true;
        }

    }

    return false;
}

void Custom_Query::save_query(QUERYSTATUS queryStatus__, const QString &customTextToAdd__)
{
    if(plugins::cryptoModule){

        QString __text2Encrypt;

        switch (queryStatus__) {
        case FAILED :
            __text2Encrypt = "::[QUERY FAILED]::";
            break;
        case SUCCESS:
            __text2Encrypt = "::[QUERY WAS SUCCESSFUL]::";
            break;
        case NOTSENDED:
            __text2Encrypt = "::[QUERY NOT SENDED]::";
            break;
        }

        __text2Encrypt += "\n[START]-->>\n\n";

        if(customTextToAdd__.isNull()){

            __text2Encrypt += ui->plainTextEdit->toPlainText().simplified();

        } else{

            __text2Encrypt += customTextToAdd__.simplified();
        }


        __text2Encrypt += QStringLiteral("\n\n<<--[END]\n%1\n").arg(QStringLiteral("_").repeated(20));

        plugins::cryptoModule->encryptSomeInfoToSomeBinF(__text2Encrypt,userQueriesHistoryBinFileName_,Custom_Query::userQueriesHistoryLengthLines_);

    }

}



QString const Custom_Query::get_text() const
{
    return this->ui->plainTextEdit->toPlainText();
}


void Custom_Query::set_text(QString const& text__) const
{
    this->ui->plainTextEdit->setPlainText(text__);
}

/*void*/noteFrame* Custom_Query::add_note(const QString & message__)
{
    noteFrame* notePtr = new noteFrame(message__,this);
//    QFrame* style_frame = new QFrame{this};

//    QHBoxLayout* dynamic_note_lay = new QHBoxLayout;

//    style_frame->setLayout(dynamic_note_lay);

//    style_frame->setFrameShape(QFrame::StyledPanel);
//    style_frame->setFrameShadow(QFrame::Raised);


//    ClickableLabel* dyn_note_lbl = new ClickableLabel(message__);
//    dyn_note_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
//    dyn_note_lbl->setWordWrap(true);
//    QPushButton* dyn_note_hide_button = new QPushButton("hide");
//    dyn_note_hide_button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
//    QPushButton* dyn_note_disable_button = new QPushButton("Don't show again");
//    dyn_note_disable_button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
//    dynamic_note_lay->addWidget(dyn_note_lbl);
//    dynamic_note_lay->addWidget(dyn_note_hide_button);
//    dynamic_note_lay->addWidget(dyn_note_disable_button);

    ui->verticalLayout->insertWidget(0,notePtr);

    QList<QPushButton*> ButtonsInFormlist = notePtr->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist){

            obj->setStyleSheet(QStringLiteral("QPushButton{%1} %2")
                .arg(obj->styleSheet())
                .arg(adb_style::getbuttonKhakiHiglightSS()));

        }

////    connect(notePtr,&noteFrame::dontShowNoteAgainSig,this,&Custom_Query::dont_show_note);

    notePtr->setAttribute(Qt::WA_DeleteOnClose, true);

//    connect(dyn_note_hide_button,&QPushButton::clicked,[=]{
//        delete style_frame;
//    });

//    connect(dyn_note_disable_button,&QPushButton::clicked,[=]{
//        delete style_frame;
//        emit dont_show_note();
//    });

    return notePtr;
}

void Custom_Query::showQueriesHistory()
{

    static QPointer<QDialog> historyWindowPtr;

    if(!historyWindowPtr){

        Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                            | Qt::WindowMinimizeButtonHint
                                            | Qt::WindowMaximizeButtonHint
                                            | Qt::WindowCloseButtonHint;

        historyWindowPtr = new QDialog{this, flags & ~Qt::WindowStaysOnTopHint};


        historyWindowPtr->setWindowTitle("Custom queries history");



        auto layout = new QVBoxLayout;

        layout->setContentsMargins(1,1,1,1);
        layout->setSpacing(1);

        auto plainTextForm = new QPlainTextEdit;
        plainTextForm->setReadOnly(true);

        historyWindowPtr->setLayout(layout);

        // c++ lambda template abuse (C++14-C++20 only)
//        auto setQObjInFrame = []<typename T>(T* QObj__) -> QFrame* {
//            QFrame* QObjFrame = new QFrame;
//            QObjFrame->setContentsMargins(0,0,0,0);
//            QObjFrame->setFrameShape(QFrame::StyledPanel);
//            QObjFrame->setFrameShadow(QFrame::Sunken);
//            QHBoxLayout* QObjFrameLay = new QHBoxLayout{QObjFrame};
//            QObjFrameLay->setContentsMargins(1,1,1,1);
//            QObjFrameLay->setSpacing(0);
//            QObjFrameLay->addWidget(QObj__);
//            QObj__->setContentsMargins(0,0,0,0);
//            return QObjFrame;
//        };
        auto setQObjInFrame = [](QWidget* QObj__) -> QFrame* {
            QFrame* QObjFrame = new QFrame;
            QObjFrame->setContentsMargins(0,0,0,0);
            QObjFrame->setFrameShape(QFrame::StyledPanel);
            QObjFrame->setFrameShadow(QFrame::Sunken);
            QHBoxLayout* QObjFrameLay = new QHBoxLayout{QObjFrame};
            QObjFrameLay->setContentsMargins(1,1,1,1);
            QObjFrameLay->setSpacing(0);
            QObjFrameLay->addWidget(QObj__);
            QObj__->setContentsMargins(0,0,0,0);
            return QObjFrame;
        };

        layout->addWidget(setQObjInFrame(plainTextForm));

        auto loadHistory = []() -> QString {
            QString __decryptedQueryHistoryInfo = "";
            if(plugins::cryptoModule)
                __decryptedQueryHistoryInfo = plugins::cryptoModule->decryptSomeBinF(userQueriesHistoryBinFileName_);
            return __decryptedQueryHistoryInfo;
        };


        plainTextForm->setPlainText(loadHistory());


        QPushButton* scroll2endButton = new QPushButton(QString::fromUtf8("Go to list end \u2193"));
        auto butFont = scroll2endButton->font();
        butFont.setBold(true);
        butFont.setPointSize(14);
        scroll2endButton->setFont(butFont);

        scroll2endButton->setStyleSheet("background: darkslategray; color: white;");


        auto setLay2Frame = [](QLayout* lay__) -> QFrame* {
            QFrame* QObjFrame = new QFrame;
            QObjFrame->setContentsMargins(0,0,0,0);
            QObjFrame->setFrameShape(QFrame::StyledPanel);
            QObjFrame->setFrameShadow(QFrame::Sunken);
            lay__->setContentsMargins(1,1,1,1);
            lay__->setSpacing(0);
            QObjFrame->setLayout(lay__);
            return QObjFrame;
        };


        QHBoxLayout* buttonLay = new QHBoxLayout{/*buttonLayFrame*/};

        buttonLay->setSpacing(0);
        buttonLay->setContentsMargins(0,0,0,0);
        buttonLay->setAlignment(Qt::AlignCenter);



        QHBoxLayout* reloadLay = new QHBoxLayout;
        QPushButton* reloadButton{new QPushButton(QString::fromUtf8("\u27F3"))};

        reloadButton->setToolTip("Reload queries history list");

        connect(reloadButton,&QPushButton::clicked,[plainTextForm,loadHistory]{
            plainTextForm->clear();
            plainTextForm->setPlainText(loadHistory());
        });

        butFont = reloadButton->font();
        butFont.setBold(true);
        butFont.setPointSize(16);
        reloadButton->setFont(butFont);
        reloadButton->setStyleSheet("background:green;color:white;");

        reloadButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


        reloadLay->addWidget(setQObjInFrame(reloadButton),0,Qt::AlignCenter);
        buttonLay->addWidget(setQObjInFrame(scroll2endButton),0,Qt::AlignCenter);


        scroll2endButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);


        layout->addWidget(setLay2Frame(reloadLay));

        layout->addWidget(setLay2Frame(buttonLay));

        connect(scroll2endButton, &QPushButton::clicked,[plainTextForm]{
            plainTextForm->verticalScrollBar()->setValue(
                        plainTextForm->verticalScrollBar()->maximum());
        });


        QPushButton* exitButton = new QPushButton(QString::fromUtf8("\u2715"));


        exitButton->setStyleSheet("background-color:darkred;color:white;font-size:14pt;padding-left:6px;padding-right:6px;");

        buttonLay->addWidget(/*exitButton*//*exitFrame*/setQObjInFrame(exitButton),1,Qt::AlignRight);

        QPushButton* clearHistButton = new QPushButton{"Clear"};
        clearHistButton->setStyleSheet("font-weight:bold;color:white;background:orange;");
        clearHistButton->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
        buttonLay->insertWidget(0,setQObjInFrame(clearHistButton),1,Qt::AlignLeft);

        connect(clearHistButton,&QPushButton::clicked,[this,plainTextForm]{
            auto answer = QMessageBox::warning(historyWindowPtr,"Clear queries history", "Are you sure want to remove queries history? This action is irreversible.",QMessageBox::Ok | QMessageBox::Cancel);

            if(answer==QMessageBox::Ok){
                if(removeUserQueriesHistory())
                    plainTextForm->clear();
            }
        });

        connect(exitButton,&QPushButton::clicked,historyWindowPtr,&QDialog::close);

        // design
        QList<QPushButton*> ButtonsInFormlist = historyWindowPtr->findChildren<QPushButton*>();
            foreach (auto obj, ButtonsInFormlist) {
                    obj->setStyleSheet(QStringLiteral("QPushButton{%1} %2")
                        .arg(obj->styleSheet())
                        .arg(adb_style::getbuttonKhakiHiglightSS()));

            }

        connect(this, &Custom_Query::destroyed,[]{
            if(historyWindowPtr)
                historyWindowPtr->close();
        });

        historyWindowPtr->setAttribute( Qt::WA_DeleteOnClose, true );
        historyWindowPtr->setModal(false);
        historyWindowPtr->show();
        //QApplication::processEvents();
        historyWindowPtr->resize(450,450);

    } else{

        if(historyWindowPtr)historyWindowPtr->raise();
    }

}



bool Custom_Query::read4rom_settings_file()
{

    QMap<QString,QString> __settings_map;

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map)){
        QString temp_s;

        if((temp_s = __settings_map.value("font_"))!="")
            Custom_Query::font_ = temp_s;

//        return true;
    } else{

        qWarning() << "Error while read from"<<settings_f_name_<<"(Strings)";
        return false;
    }
    //--------------------------------------------

    QMap<QString,int> __settings_map_int;

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map_int)){
        int temp;

        if((temp = __settings_map_int.value("closeQuestion"))!=-1)
            Custom_Query::askBeforeClose_ = temp;

        if((temp = __settings_map_int.value("userQueriesHistoryLengthLines_"))!=-1)
            Custom_Query::userQueriesHistoryLengthLines_ = temp;

        return true;
    } else{

        qWarning() << "Error while read from"<<settings_f_name_;
//        return false;
    }

    return false;
}

void Custom_Query::write2_settings_file()
{
    QFile outFile(settings_f_name_);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);
    textStream << "font_" << '=' << '\"'+Custom_Query::font_+'\"' << Qt::endl;
    textStream << "closeQuestion" << '=' << QString::number(Custom_Query::askBeforeClose_) << Qt::endl;
    textStream << "userQueriesHistoryLengthLines_" << '=' << QString::number(Custom_Query::userQueriesHistoryLengthLines_) << Qt::endl;
}


