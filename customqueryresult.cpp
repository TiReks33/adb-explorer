#include "customqueryresult.h"
#include "ui_customqueryresult.h"


bool CustomQueryResult::headerCopy_ = false;
int CustomQueryResult::defaultScaleIndex_ = 0;
bool CustomQueryResult::settingsFileReady_ = true;

QString CustomQueryResult::defaultFont_ = "Noto Sans,10,-1,0,50,0,0,0,0,0,Regular";

/*bool*/dynamicbool CustomQueryResult::askBeforeClose_ = true;

CustomQueryResult::CustomQueryResult(auth& auth__,QWidget *parent,bool closeMessage__) :
    QDialog(parent),
    ui(new Ui::CustomQueryResult)
  , tableView(new signalTableView)
  , reload_button_{new /*QPushButton*/reloadButton{0,/*"brown","yellow",*/"darkslategray","snow",true,false,"<u>R</u>efresh"}}
  , export_button_{new QPushButton{"Export data"}}
  , copy_button_{new QPushButton{}}
  , copyButtonCheckBox_{new QCheckBox}

  , rescaleBoxWidget{adb_utility::getRescaleBox(tableView)}
  , rescaleDefaultCheckBox{new QCheckBox{"set as default"}}
  , auth_(auth__)

  , fontWidget_{new fontEmbeddedWidget{tableView}}

  , checkCloseMessageFlag_(closeMessage__)
{
    ui->setupUi(this);

    fileOps();

    setCheckCloseMessageFlag(closeMessage__);

    init_form();

    init_signals();

    defaultSettings();
}

void CustomQueryResult::fileOps()
{
    // get/set file data only once (for multiply windows speed)
    if(CustomQueryResult::settingsFileReady_){
        if(!read4rom_settings_file())
            write2_settings_file();
        CustomQueryResult::settingsFileReady_=false;
    }
}


void CustomQueryResult::init_form()
{
    Qt::WindowFlags flags = Qt::Window  | Qt::WindowSystemMenuHint
                                        | Qt::WindowMinimizeButtonHint
                                        | Qt::WindowMaximizeButtonHint
                                        | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags & ~Qt::WindowStaysOnTopHint);

    ////move(screen()->geometry().center() - frameGeometry().center());

    QFrame* tableCopyFrame = new QFrame;

    tableCopyFrame->setFrameShadow(QFrame::Raised);

    tableCopyFrame->setFrameShape(QFrame::StyledPanel);

    QVBoxLayout* tableCopyLay = new QVBoxLayout{tableCopyFrame};

    tableCopyLay->setContentsMargins(3,3,3,3);

    tableCopyLay->setSpacing(3);


    this->ui->verticalLayout->setObjectName("mainLay");

    this->ui->verticalLayout->insertWidget(0,tableView);

    this->ui->verticalLayout->insertWidget(1,tableCopyFrame);

    resize(800,640);

    export_button_->setStyleSheet("color:white;background: gray; font-weight:bold;");
    export_button_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);



    reload_button_->setObjectName("refreshButton");



    reload_button_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);



    QFrame* exportButtonsFrame = new QFrame{/*this*/};
    exportButtonsFrame->setFrameShape(QFrame::NoFrame);//(QFrame::StyledPanel);
    exportButtonsFrame->setFrameShadow(QFrame::Raised);

//    QVBoxLayout* exportButtonsFrameSubLay = new QVBoxLayout{exportButtonsFrame};
//    exportButtonsFrameSubLay->addWidget(reload_button_);


    QHBoxLayout* exportButtonsSubLay = new QHBoxLayout{exportButtonsFrame};
    exportButtonsSubLay->setContentsMargins(0,0,0,0);
    exportButtonsSubLay->setSpacing(3);

    exportButtonsSubLay->addWidget(export_button_);
    QLabel* exportButtonsSeparateLbl = new QLabel{"::"};
    exportButtonsSeparateLbl->setStyleSheet("font-weight:bold;");
    exportButtonsSeparateLbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    exportButtonsSubLay->addWidget(exportButtonsSeparateLbl);

    exportButtonsSubLay->addWidget(copy_button_);

//    tableCopyLay->addLayout(exportButtonsSubLay);
    tableCopyLay->addWidget(exportButtonsFrame);


    QFrame* reloadButtonFrame = new QFrame;
    reloadButtonFrame->setFrameShape(QFrame::StyledPanel);
    reloadButtonFrame->setFrameShadow(QFrame::Raised);

    QHBoxLayout* reloadButtonFrameSubLay = new QHBoxLayout{reloadButtonFrame};
    reloadButtonFrameSubLay->setContentsMargins(3,3,3,3);
    reloadButtonFrameSubLay->setSpacing(3);

    reloadButtonFrameSubLay->addWidget(reload_button_);

    QHBoxLayout* reloadButtonFrameLay = new QHBoxLayout;

    reloadButtonFrameLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    reloadButtonFrameLay->addWidget(reloadButtonFrame);
    reloadButtonFrameLay->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));

    ui->verticalLayout->insertLayout(2,reloadButtonFrameLay);



    // since rescaleBoxWidget added -- it control the tableView's rescale
    ui->rescaleLayout->addWidget(rescaleBoxWidget);


    //drop-down solution for copying data to clipboard
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    QLabel* copyButtonSubLbl = new QLabel("Copy selection to clipboard");
    copyButtonSubLbl->setObjectName("copyTextLbl");

    copyButtonSubLbl->setStyleSheet("padding-top:0px;padding-bottom:0px;color:darkslategray;");

    copyButtonSubLbl->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    copyButtonSubLbl->setContentsMargins(0,0,0,0);


    QFrame* copyButtonSubFrame = new QFrame;

    copyButtonSubFrame->setStyleSheet("padding-top:0px;padding-bottom:0px;");

    copyButtonSubFrame->setFrameShape(QFrame::StyledPanel);

    copyButtonSubFrame->setFrameShadow(QFrame::Raised);

    copyButtonSubFrame->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    copyButtonSubFrame->setContentsMargins(0,0,0,0);

    QHBoxLayout* copyButtonSubLay = new QHBoxLayout{};

    copyButtonSubLay->setSizeConstraint(QLayout::SetMinimumSize);

    copy_button_->setLayout(copyButtonSubLay);

    copyButtonSubLay->addWidget(copyButtonSubLbl,0,Qt::AlignCenter);

    copyButtonSubLay->addWidget(copyButtonSubFrame,0,Qt::AlignCenter);


    QHBoxLayout* copyButtonModeSubLay = new QHBoxLayout{copyButtonSubFrame};

    copyButtonModeSubLay->setSpacing(0);

    copyButtonModeSubLay->setContentsMargins(3,0,0,0);

    copyButtonModeSubLay->setSizeConstraint(QLayout::SetDefaultConstraint);


    copyButtonCheckBox_->setText("With table headers");
    copyButtonCheckBox_->setObjectName("copyHeaders");
    copyButtonCheckBox_->setContentsMargins(0,0,0,0);

    copyButtonCheckBox_->setStyleSheet("padding-top:0px;padding-bottom:0px;color:darkslategray;");

    copyButtonCheckBox_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    copyButtonModeSubLay->addWidget(copyButtonCheckBox_);




    copy_button_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

    auto copyLblFont = copyButtonSubLbl->font();
    copyLblFont.setBold(true);
    copyButtonSubLbl->setFont(copyLblFont);

    auto copyCheckBoxFont = copyButtonCheckBox_->font();
    copyCheckBoxFont.setBold(true);
    copyButtonCheckBox_->setFont(copyCheckBoxFont);

    copy_button_->setStyleSheet("padding-top:0px;padding-bottom:0px; background:floralwhite; font-weight:bold; ");



    ui->rescaleLayout->addWidget(rescaleDefaultCheckBox);

    ui->Cancel_button->setStyleSheet("background-color:darkred;color:white;font-size:14pt;padding-left:6px;padding-right:6px;");

    ui->Cancel_button->setText(QString::fromUtf8("\U00002715"));

    export_button_->setDefault(true);

    ui->fontSubLay->addWidget(fontWidget_);

    fontWidget_->setStyleSheet("QPushButton#fontButton {color: black; background: seashell;}");

    auto rescaleComboBox = rescaleBoxWidget->findChild<notifyComboBox*>();
    if(rescaleComboBox)
        rescaleComboBox->setStyleSheet(adb_style::getComboBoxKhakiHighlightSS("#fffffa","darkslategray"));

    export_button_->setFocus();

    tableView->setAlternatingRowColors(true);

    tableView->setPalette(QPalette(adb_style::whiteUndGrayColor));



    QList<QPushButton*> ButtonsInFormlist = this->findChildren<QPushButton*>();
        foreach (auto obj, ButtonsInFormlist) {
            // if button is inherited class object -> cast pointer to inherit type
            if(obj->objectName()=="refreshButton"){

                auto reloadButtonObj = qobject_cast<reloadButton*>(obj);
                reloadButtonObj->setKhakiHighlight();
            }else {
                obj->setStyleSheet(QStringLiteral("QPushButton{%1} %2")
                    .arg(obj->styleSheet())
                    .arg(adb_style::getbuttonKhakiHiglightSS()));
            }

        }



        copy_button_->installEventFilter(this);

        QList<QCheckBox*> checkBoxInFormlist = this->findChildren<QCheckBox*>();
            foreach (auto obj, checkBoxInFormlist) {

                    obj->setStyleSheet(adb_style::adbCheckBoxStyleSheet);
            }
}



void CustomQueryResult::init_signals()
{
    connect(ui->Cancel_button,&QPushButton::clicked,this,&CustomQueryResult::close);

    connect(export_button_,&QPushButton::clicked,this,[=]{
        adb_utility::exportTableViewToCSV(tableView);
    });


    connect(tableView, &signalTableView::customContextMenuRequested, this, &CustomQueryResult::slotCustomMenuRequested);

    connect(copy_button_,&QPushButton::clicked,this,[=]{
        copySelectionToClipboard();
    });

    connect(copyButtonCheckBox_,&QCheckBox::stateChanged,[=](int state__){

        if(!state__){
            ClipboardCopyHeadersEntrieText_ = "Headers off[✓]";
        } else{
            ClipboardCopyHeadersEntrieText_ = "Headers off";
        }
        CustomQueryResult::headerCopy_=state__;
        emit saveSettings();
    });



    //QObject::connect(rescaleDefaultCheckBox,&QCheckBox::destroyed,[=]{ qDebug() << "~Databases::ui->rescaleLayout::rescaleDefaultCheckBox"; });


    QPointer</*QComboBox*/notifyComboBox> rescaleComboBox = rescaleBoxWidget->findChild<notifyComboBox*>();


    connect(rescaleComboBox,&notifyComboBox::sameIndexRepeated,[=](){
        rescaleComboBox->currentIndexChanged(rescaleComboBox->currentIndex());
    });

    connect(rescaleComboBox,static_cast<void(/*QComboBox*/notifyComboBox::*)(int)>(&/*QComboBox*/notifyComboBox::currentIndexChanged),[=](int newIndex__){
        if(newIndex__==CustomQueryResult::defaultScaleIndex_){
            rescaleDefaultCheckBox->setChecked(true);
            rescaleDefaultCheckBox->setEnabled(false);
        } else{
            rescaleDefaultCheckBox->setChecked(false);
            rescaleDefaultCheckBox->setEnabled(true);
        }
    });

    connect(this,&CustomQueryResult::saveSettings, this, &CustomQueryResult::write2_settings_file);

    connect(rescaleDefaultCheckBox,&QCheckBox::stateChanged,[=](int state__){
        if(state__){
            int currentBoxIndex = rescaleComboBox->currentIndex();
            if(currentBoxIndex!=CustomQueryResult::defaultScaleIndex_){
                ////tableScaleChanged=true;
                CustomQueryResult::defaultScaleIndex_ = currentBoxIndex;
                rescaleDefaultCheckBox->setEnabled(false);

                emit saveSettings();
            }
        }
    });

    connect(reload_button_,&QPushButton::clicked,this, &CustomQueryResult::loadQuery);

    connect(fontWidget_,&fontEmbeddedWidget::defaultFontChanged,[this](QString const& newDefaultFont_){
        CustomQueryResult::defaultFont_=newDefaultFont_;
        write2_settings_file();
    });

    connect(ui->closeMessageCheckBox,&QCheckBox::stateChanged,[this](int newState__){
        CustomQueryResult::askBeforeClose_ = newState__;
        write2_settings_file();
    });

    connect(this,&CustomQueryResult::closeNowSig,[this]{
        checkCloseMessageFlag_=false;
        reject();
    });

//    auto dynamicCheck = connect(&CustomQueryResult::askBeforeClose_, &dynamicbool::boolChanged,[this]{
//        ui->closeMessageCheckBox->setChecked(CustomQueryResult::askBeforeClose_);
//    });

////    connect(this,&CustomQueryResult::destroyed,[dynamicCheck]{
//    connect(ui->closeMessageCheckBox,&QCheckBox::destroyed,[dynamicCheck]{
//        disconnect(dynamicCheck);
//    });
    CustomQueryResult::askBeforeClose_.synchronizeCheckBox(ui->closeMessageCheckBox);
}


void CustomQueryResult::defaultSettings()
{
    // set default settings
    rescaleBoxWidget->findChild<notifyComboBox*>()->setCurrentIndex(CustomQueryResult::defaultScaleIndex_);
    copyButtonCheckBox_->setChecked(CustomQueryResult::headerCopy_);

    // set default font
    QFont __font;

    __font.fromString(CustomQueryResult::defaultFont_);
    tableView->setFont(__font);
}



void CustomQueryResult::reject()
{
    if(CustomQueryResult::askBeforeClose_ && checkCloseMessageFlag_){
        QMessageBox* questionBox = new QMessageBox(QMessageBox::Question,"Closing","The window will close. Continue?",QMessageBox::Ok | QMessageBox::Cancel,this);

        QCheckBox* notAskAgainCheckBox = new QCheckBox{"Always ask before close",questionBox};
        notAskAgainCheckBox->setChecked(CustomQueryResult::askBeforeClose_);
        questionBox->setCheckBox(notAskAgainCheckBox);

        connect(notAskAgainCheckBox,&QCheckBox::stateChanged,[this](int newState__){
            //Custom_Query::askBeforeClose_= newState__;
            ui->closeMessageCheckBox->setChecked(newState__);
        });

        CustomQueryResult::askBeforeClose_.synchronizeCheckBox(notAskAgainCheckBox);
//        auto dynamicCheck = connect(&CustomQueryResult::askBeforeClose_, &dynamicbool::boolChanged,[notAskAgainCheckBox]{
//            notAskAgainCheckBox->setChecked(CustomQueryResult::askBeforeClose_);
//        });

//        connect(notAskAgainCheckBox,&QCheckBox::destroyed,[dynamicCheck]{
//            disconnect(dynamicCheck);
//        });

        questionBox->setAttribute(Qt::WA_DeleteOnClose,true);
        questionBox->setWindowModality(Qt::WindowModal);

        QFlag flags = Qt::Window & ~Qt::WindowStaysOnTopHint;
        questionBox->setWindowFlags(flags);

        questionBox->show();

        connect(this,&CustomQueryResult::closeNowSig,questionBox,&QMessageBox::accept);


        auto answer = questionBox->exec();

        if(answer==QMessageBox::Ok){
            //emit closed_();
            QDialog::reject();
        }

    } else{
        //emit closed_();
        QDialog::reject();
    }
}



void CustomQueryResult::setCheckCloseMessageFlag(bool state__)
{
    if((checkCloseMessageFlag_=state__)){
        ui->closeMsgFrame->show();
        ui->closeMsgFrame->setEnabled(true);
        ui->closeMessageCheckBox->setChecked(CustomQueryResult::askBeforeClose_);
    } else{
        ui->closeMsgFrame->setEnabled(false);
        ui->closeMsgFrame->hide();
    }
}



void CustomQueryResult::keyPressEvent(QKeyEvent *event__)
{
    if(event__->modifiers() & Qt::ControlModifier){
        if(event__->key() == Qt::Key_C)
            copySelectionToClipboard();
    }
    QDialog::keyPressEvent(event__);
}



void CustomQueryResult::custom_query_slot(QString const & query__, QString const & con_name__)
{
    this->last_query_ = query__;
    this->last_con_name_ = con_name__;
    loadQuery();
}



void CustomQueryResult::loadQuery()
{

    if(db_connection::try_to_reopen(auth_,last_con_name_))
    {
        QPointer<adbMessageBox> __warningWindowPtr = nullptr;

        db_connection::set_query(last_query_,&model_,tableView,__warningWindowPtr,last_con_name_);

        if(__warningWindowPtr){

            connect(this,&CustomQueryResult::closeNowSig,[__warningWindowPtr]{
                if(__warningWindowPtr){
                    __warningWindowPtr->close();
                }
            });
        }
    }
}



CustomQueryResult::~CustomQueryResult()
{

    delete ui;

    delete tableView;

    delete export_button_;
    delete copy_button_;


    delete rescaleBoxWidget;
    delete rescaleDefaultCheckBox;
}


void CustomQueryResult::close_window()
{
    //qDebug()<<"Close custom query form signal handled";
    this->close();
}



void CustomQueryResult::copySelectionToClipboard()
{

    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedIndexes();

    if(selectedIndexes.size() < 1)
        return;


    QModelIndex current{},next{};

    QString clipboardString="",displayText="";

    for (int i = 0; i < selectedIndexes.count(); ++i)
    {
        current = selectedIndexes[i];
        displayText = current.data(Qt::DisplayRole).toString();

        // If there exists another column beyond this one.
        if (i + 1 < selectedIndexes.count())
        {
            next = selectedIndexes[i+1];

            // If the column is on different row, the clipboard should take note.
            if (next.row() != current.row())
            {
                displayText.append("\n");
            }
            else
            {
                // Otherwise append a column separator.
                displayText.append(ClipboardCopySeparator_);
            }
        }
        clipboardString.append(displayText);
    }



    if(!copyButtonCheckBox_->isChecked()){

        QApplication::clipboard()->setText(clipboardString);

    } else{

        int first_h_indx = selectedIndexes.begin()->column();
        int last_h_indx = selectedIndexes.last().column();

        auto tableModel = tableView->model();

        QString header_data="";

        for(auto i=first_h_indx;i<=last_h_indx;++i){

            header_data +=  tableModel->headerData(i, Qt::Horizontal).toString();

            header_data+=ClipboardCopySeparator_;

        }

        header_data+="\n";



        QApplication::clipboard()->setText(header_data+clipboardString);
    }
}



void CustomQueryResult::slotCustomMenuRequested(QPoint pos)
{
    QMenu * menu = new QMenu(this);

    QAction * copySelectionEntrie = new QAction("Copy selection", this);

    connect(copySelectionEntrie, &QAction::triggered,[=]{
        copySelectionToClipboard();
    });

    menu->addAction(copySelectionEntrie);

    QMenu * CopyModeSubMenu =  menu->addMenu("Copy settings");

    menu->addMenu(CopyModeSubMenu);

    QAction * ClipboardCopyHeadersEntrie = new QAction(ClipboardCopyHeadersEntrieText_/*"Headers off[✓]"*/,this);

    connect(ClipboardCopyHeadersEntrie, &QAction::triggered,[=]{

        copyButtonCheckBox_->setChecked(!copyButtonCheckBox_->isChecked());

    });

    CopyModeSubMenu->addAction(ClipboardCopyHeadersEntrie);

    menu->popup(tableView->viewport()->mapToGlobal(pos));
}



bool CustomQueryResult::read4rom_settings_file()
{
    QMap<QString,int> __settings_map_int;

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map_int)){
        int temp;

        if((temp = __settings_map_int.value("defaultScaleMode"))!=-1){
            CustomQueryResult::defaultScaleIndex_=temp;
        }

        if((temp = __settings_map_int.value("headersCopy"))!=-1){
            CustomQueryResult::headerCopy_=temp;
        }

        if((temp = __settings_map_int.value("closeQuestion"))!=-1)
            CustomQueryResult::askBeforeClose_ = temp;

    } else{

        qWarning() << "Error while read from"<<settings_f_name_;
        return false;
    }
    //--------------------------------------------------------------------
    QMap<QString,QString> __settings_map_str;

    if(adb_utility::get_settings_4rom_file(settings_f_name_,__settings_map_str)){
        QString temp_s;

        if((temp_s = __settings_map_str.value("defaultTableFont"))!="")
            CustomQueryResult :: defaultFont_ = temp_s;

        return true;
    } else{

        qWarning() << "Error while read from"<<settings_f_name_<<"(Strings)";
    }

    return false;
}

void CustomQueryResult::write2_settings_file()
{
    QFile outFile(settings_f_name_);
    outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream textStream(&outFile);

    textStream << "defaultScaleMode" << '=' << QString::number(CustomQueryResult::defaultScaleIndex_) << Qt::endl;
    textStream << "headersCopy" << '=' << QString::number(CustomQueryResult::headerCopy_) << Qt::endl;
    textStream << "defaultTableFont" << '=' << '\"'+CustomQueryResult::defaultFont_+'\"' << Qt::endl;
    textStream << "closeQuestion" << '=' << QString::number(CustomQueryResult::askBeforeClose_) << Qt::endl;
}



bool CustomQueryResult::eventFilter(QObject *obj, QEvent *event)
{

    if(obj == (QObject*)copy_button_) {

        if (event->type() == QEvent::HoverEnter && copy_button_->isEnabled())
        {

            repaintCopyButtonText(adb_style::buttonKhakiHighlightTextHoverColor_);

        // Whatever you want to do when mouse goes over targetPushButton
        } else if(event->type() == QEvent::HoverLeave && !copy_button_->hasFocus() && copy_button_->isEnabled()){


            repaintCopyButtonText(copy_button_->palette().buttonText().color());

        } else if(event->type() == QEvent::HoverLeave && copy_button_->hasFocus()){


            repaintCopyButtonText(adb_style::buttonKhakiHighlightTextFocusColor_);

        } else if(event->type() == QEvent::FocusIn ){


            repaintCopyButtonText(adb_style::buttonKhakiHighlightTextHoverColor_);

        } else if(event->type() == QEvent::FocusOut){

            repaintCopyButtonText(copy_button_->palette().buttonText().color());
        }




    }/*else {*/

        // pass the event on to the parent class
        return QDialog::eventFilter(obj, event);
        //    }
}

void CustomQueryResult::repaintCopyButtonText(QColor newColour__)
{
    auto textSubLbl = copy_button_->findChild<QLabel*>("copyTextLbl");
    auto subComboBox = copy_button_->findChild<QCheckBox*>("copyHeaders");

    auto textLblFontPal = textSubLbl->palette();
    textLblFontPal.setColor(textSubLbl->foregroundRole(), newColour__);
    textSubLbl->setPalette(textLblFontPal);

    auto checkBoxFontPal = subComboBox->palette();
    checkBoxFontPal.setColor(subComboBox->foregroundRole(), newColour__);
    subComboBox->setPalette(checkBoxFontPal);
}


