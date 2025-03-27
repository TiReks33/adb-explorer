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

        QFont newFont = QFontDialog::getFont(
                        &ok, oldFont, this);
        if (ok) {
            if(newFont!=oldFont){

                parent_->setFont(newFont);


                defaultFontCheckBox_->setChecked(false);
                defaultFontCheckBox_->setEnabled(true);
            }

        }
    });


    defaultFontCheckBox_->setChecked(true);
    defaultFontCheckBox_->setEnabled(false);

    connect(defaultFontCheckBox_,&QCheckBox::clicked,[this](bool checked__){
        if(checked__){
            emit defaultFontChanged(parent_->font().toString());

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


    setFontButton_->setObjectName("fontButton");


    auto __buttonFont = setFontButton_->font();
//            __buttonFont.setBold(true);
    __buttonFont.setUnderline(true);

    QPixmap pixmap(16,16);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QString string;
    string = QString::fromUtf8("\U0001F589");
    painter.setFont(__buttonFont);
    painter.drawText(0,0,16,16,Qt::AlignHCenter | Qt::AlignVCenter, string);


    setFontButton_->setText("Set Font");
    setFontButton_->setIcon(pixmap/*QIcon(":/pic/penf.png")*/);


    mainLay->addWidget(setFontButton_);

    setFontButton_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    setFontButton_->setContentsMargins(0,0,0,0);


    mainLay->setContentsMargins(0,0,0,0);


    defaultFontCheckBox_->setText("set default");


    defaultFontCheckBox_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    defaultFontCheckBox_->setContentsMargins(0,0,0,0);

    mainLay->addWidget(defaultFontCheckBox_);

}


