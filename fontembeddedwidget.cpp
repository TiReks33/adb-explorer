#include "fontembeddedwidget.h"

fontEmbeddedWidget::fontEmbeddedWidget(QWidget *parent__)
    : QWidget(parent__)
    , parent_{parent__}
    , setFontButton_{new QPushButton{}}
    , defaultFontCheckBox_{new QCheckBox{}}
{

    initSignals();
    initForm();

}

fontEmbeddedWidget::~fontEmbeddedWidget()
{

}


void fontEmbeddedWidget::initSignals()
{
    connect(setFontButton_,&QPushButton::clicked,[this]{
        bool ok;

        QFont oldFont = parent_->font();
        qDebug() << "OLD::"<<oldFont.toString();
        QFont newFont = QFontDialog::getFont(
                        &ok, oldFont, this);
        if (ok) {
            if(newFont/*.toString()*/!=oldFont/*.toString()*/){
                // the user clicked OK and font is set to the font the user selected
                parent_->setFont(newFont);
                qDebug() << "NEW::"<<newFont.toString();
                //emit fontChanged(font.toString());

                defaultFontCheckBox_->setChecked(false);
                defaultFontCheckBox_->setEnabled(true);
            } /*else{
                qDebug() << "same";
            }*/
        } else {
            // the user canceled the dialog; font is set to the initial value
        }
    });


    defaultFontCheckBox_->setChecked(true);
    defaultFontCheckBox_->setEnabled(false);

    connect(defaultFontCheckBox_,&QCheckBox::clicked,[this](bool checked__){
        if(checked__){
            emit defaultFontChanged(parent_->font().toString());
            qDebug() << "setDefaultSignal emited";
            defaultFontCheckBox_->setEnabled(false);
        }
    });
}



void fontEmbeddedWidget::initForm()
{

    QHBoxLayout* mainLay = new QHBoxLayout{};
    setLayout(mainLay);

    mainLay->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    mainLay->setContentsMargins(0,0,0,0);
    mainLay->setSpacing(1);

    QHBoxLayout* buttonLay = new QHBoxLayout{};
buttonLay->setContentsMargins(6,6,6,3);
buttonLay->setSpacing(1);
    buttonLay->setSizeConstraint(QLayout::SetDefaultConstraint);//SetMinimumSize);

    setFontButton_->setLayout(buttonLay);
    setFontButton_->setObjectName("fontButton");
//    setFontButton_->setContentsMargins(0,0,0,0);



//    QLabel* buttonLbl = new QLabel{"<u>🖉</u> Set Font"};

//    buttonLbl->setAttribute(Qt::WA_TransparentForMouseEvents, true);
//    buttonLbl->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    buttonLbl->setStyleSheet("padding-top:0px;padding-bottom:0px;");
//    buttonLbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

//    buttonLay->addWidget(buttonLbl);

        auto __buttonFont = setFontButton_->font();
//        __buttonFont.setBold(true);
        __buttonFont.setUnderline(true);

        QPixmap pixmap(16,16);
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        QString string;
        string = "🖉";
        painter.setFont(__buttonFont);
        painter.drawText(0,0,16,16,Qt::AlignHCenter | Qt::AlignVCenter, string);


        setFontButton_->setText("Set Font");
        setFontButton_->setIcon(pixmap);

        __buttonFont.setUnderline(false);
        __buttonFont.setBold(true);
        setFontButton_->setFont(__buttonFont);

    mainLay->addWidget(setFontButton_);

    setFontButton_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setFontButton_->setContentsMargins(0,0,0,0);


    mainLay->setContentsMargins(0,0,0,0);



    defaultFontCheckBox_->setText("set default");
//    QHBoxLayout* checkboxSubLay = new QHBoxLayout;
//    checkboxSubLay->setContentsMargins(0,0,0,0);
//    checkboxSubLay->setSpacing(0);

//    QLabel* checkBoxSubLbl = new QLabel{"set default"};
//    checkBoxSubLbl->setContentsMargins(0,0,0,0);
//    checkBoxSubLbl->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

////    checkboxSubLay->addWidget(defaultFontCheckBox_);
//    checkboxSubLay->addWidget(checkBoxSubLbl);

    defaultFontCheckBox_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    defaultFontCheckBox_->setContentsMargins(0,0,0,0);



//    mainLay->addLayout(checkboxSubLay);
    mainLay->addWidget(defaultFontCheckBox_);


}


