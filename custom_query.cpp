#include "custom_query.h"
#include "ui_custom_query.h"

QString Custom_Query::font_ = "";

bool Custom_Query::settingsFileReady_ = true;

bool Custom_Query::askBeforeClose_ = true;

Custom_Query::Custom_Query(QWidget *parent, bool closeMessage/*, QFont font*/)
    : QDialog(parent)
    , ui(new Ui::Custom_Query)
    , checkCloseMessageFlag_(closeMessage)
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

    fontWidgetFrame->setFrameShape(QFrame::StyledPanel);
    fontWidgetFrame->setFrameShadow(QFrame::Sunken);

    QHBoxLayout* fontWidgetLay = new QHBoxLayout{fontWidgetFrame};
    fontWidgetLay->setSpacing(1);
    fontWidgetLay->setContentsMargins(1,1,1,1);

    fontWidgetLay->addWidget(defaultFontWidget_);

    ui->buttonsLayout->insertWidget(0,fontWidgetFrame);

    ui->Cancel_button->setText("⌧");
    ui->Cancel_button->setStyleSheet("background-color:darkred;color:white;font-size:16pt;padding-top:1px;padding-bottom:1px;");//padding-top:1px;padding-bottom:1px;");

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

    // set default font
    QFont __font;
    qDebug() << "STATIC FONT:" <<Custom_Query::font_;
    __font.fromString(Custom_Query::font_);
    ui->plainTextEdit->setFont(__font);


    ui->plainTextEdit->setFocus();

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



QString const Custom_Query::get_text() const
{
    return this->ui->plainTextEdit->toPlainText();
}



void Custom_Query::close_window()
{
    //qDebug()<<"Close custom query form signal handled";
    this->close();
}

void Custom_Query::set_text(QString const& text__) const
{
    this->ui->plainTextEdit->setPlainText(text__);
}

void Custom_Query::add_note(const QString & message__)
{

    QFrame* style_frame = new QFrame{this};

    QHBoxLayout* dynamic_note_lay = new QHBoxLayout;

    style_frame->setLayout(dynamic_note_lay);

    style_frame->setFrameShape(QFrame::StyledPanel);
    style_frame->setFrameShadow(QFrame::Raised);


    ClickableLabel* dyn_note_lbl = new ClickableLabel(message__);
    dyn_note_lbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    dyn_note_lbl->setWordWrap(true);
    QPushButton* dyn_note_hide_button = new QPushButton("hide");
    dyn_note_hide_button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    QPushButton* dyn_note_disable_button = new QPushButton("Don't show again");
    dyn_note_disable_button->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    dynamic_note_lay->addWidget(dyn_note_lbl);
    dynamic_note_lay->addWidget(dyn_note_hide_button);
    dynamic_note_lay->addWidget(dyn_note_disable_button);

    ui->verticalLayout->insertWidget(0,style_frame);

    QList<QPushButton*> ButtonsInFormlist = style_frame->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist){

            obj->setStyleSheet(QStringLiteral("QPushButton{%1} %2")
                .arg(obj->styleSheet())
                .arg(adb_style::getbuttonKhakiHiglightSS()));

        }

    connect(dyn_note_hide_button,&QPushButton::clicked,[=]{
        delete style_frame;
    });

    connect(dyn_note_disable_button,&QPushButton::clicked,[=]{
        delete style_frame;
        emit dont_show_note();
    });
}



bool Custom_Query::read4rom_settings_file()
{

    QMap<QString,QString> __settings_map;

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map)){
        QString temp_s;
qDebug() << __settings_map;
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
}


