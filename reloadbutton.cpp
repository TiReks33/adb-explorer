#include "reloadbutton.h"

reloadButton::reloadButton(QWidget* parent__, QColor bgColor__, QColor fontColor__, bool fontBold__, bool fontItalic__, QString const& alterText__)
    : /*QPushButton*/BlinkinButton(parent__)
    , reload_icon_{QApplication::style()->standardIcon(QStyle::SP_BrowserReload)}
    , button_icon_lbl_{new QLabel{}}
    , button_rich_text_lbl_{new QLabel{}}
{
    QVBoxLayout* button_rich_text_layout = new QVBoxLayout{};

    setObjectName("reloadButtonObj");

    // [button_rich_text_layout->setSpacing(int) for setting spacing between rich text and pixmap]
    ////button_rich_text_layout->setSpacing(1);

    button_rich_text_layout->setSizeConstraint(QLayout::SetMinimumSize);

    this->setLayout(button_rich_text_layout);

    QHBoxLayout *button_sublayout = new QHBoxLayout;

    button_rich_text_layout->addLayout(button_sublayout);

    button_rich_text_lbl_->setParent(this);
    button_rich_text_lbl_->setObjectName("reloadButtonText");

    button_icon_lbl_->setParent(this);
    button_icon_lbl_->setObjectName("reloadButtonIcon");


    if(alterText__.isNull())
        setText("<u>R</u>eload");
    else
        setText(/*"<p>"+*/alterText__/*+"</p>"*/);

    setBold(fontBold__);
    setItalic(fontItalic__);

    button_rich_text_lbl_->setAlignment(Qt::AlignVCenter | Qt::AlignVCenter); // or center
    button_rich_text_lbl_->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    button_rich_text_lbl_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);


    button_sublayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
    button_sublayout->addWidget(button_icon_lbl_);
    button_sublayout->addWidget(button_rich_text_lbl_);
    button_sublayout->addItem(new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));

    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);


    if(fontColor__.isValid()){
        setFontColour(fontColor__);
    } else{
        setFontColour(QWidget::palette().color(QWidget::foregroundRole()));
    }

    if(bgColor__.isValid()){
        setBackgroundColour(bgColor__);
    } else{
        setBackgroundColour(QWidget::palette().color(QWidget::backgroundRole()));
    }

    hoverFontColour_ = defaultFontColour();
    focusFontColour_ = defaultFontColour();

    installEventFilter(this);

}

reloadButton::~reloadButton()
{

}

void reloadButton::setText(const QString &newText__)
{
    button_rich_text_lbl_->setText(newText__);
}


void reloadButton::setBold(bool value__)
{
    QFont lblFont = button_rich_text_lbl_->font();
    lblFont.setBold(value__);
    button_rich_text_lbl_->setFont(lblFont);
}

void reloadButton::setItalic(bool value__)
{
    QFont lblFont = button_rich_text_lbl_->font();
    lblFont.setItalic(value__);
    button_rich_text_lbl_->setFont(lblFont);
}

void reloadButton::setFont(const QFont & newFont__)
{

    button_rich_text_lbl_->setFont(newFont__);

    reloadIconChange();
}

void reloadButton::setFontPointSize(int newPointSize__)
{

    QFont font = button_rich_text_lbl_->font();
    font.setPointSize(newPointSize__);
    setFont(font);
}


void reloadButton::setKhakiHighlight()
{

    hoverFontColour_ = adb_style::buttonKhakiHighlightTextHoverColor_;
    focusFontColour_ = adb_style::buttonKhakiHighlightTextFocusColor_;


    setStyleSheet(adb_style::getbuttonKhakiHiglightSS(hoverFontColour_.name(),focusFontColour_.name()));

}

void reloadButton::removeKhakiHighlight()
{

    hoverFontColour_ = defaultFontColour();
    focusFontColour_ = defaultFontColour();

    // backup font properties
    auto curFont = font();
//    auto curIsBold = isBold();
//    auto curIsItalic = isItalic();
//    auto curFontPointS = fontPointSize();

    setStyleSheet("");

    setBackgroundColour(defaultBackgroundColour());
    setFontColour(defaultFontColour());

    setFont(curFont);
//    setBold(curIsBold);
//    setItalic(curIsItalic);
//    setFontPointSize(curFontPointS);
}


void reloadButton::update()
{
    if((buttonState=!buttonState)){
        repaintBackground(defaultBackgroundColour());
        repaintFont(defaultFontColour());
    }else {
        repaintBackground(blinkinBackgroundColour());
        repaintFont(blinkinFontColour());
    }
}



void reloadButton::stop_blinkin()
{
    if(is_active()){
        timer->stop();
        repaintBackground(defaultBackgroundColour());
        repaintFont(defaultFontColour());
//        currentState_ = ReloadButtonState::Default;
        emit blinkStopped();
    }
}

void reloadButton::enterEvent(QEvent *e)
{
    emit hovered();

    QWidget::enterEvent(e);
}

void reloadButton::leaveEvent(QEvent *e)
{
    emit leave();

    QWidget::leaveEvent(e);
}

void reloadButton::focusInEvent(QFocusEvent *e)
{
    emit focusIn();

    QWidget::focusInEvent(e);
}

void reloadButton::focusOutEvent(QFocusEvent *e)
{
    emit focusOut();

    QWidget::focusOutEvent(e);
}



void reloadButton::reloadIconChange(QIcon::Mode mode__, QColor colour__/*, ReloadButtonState state__*/)
{
    auto __newPointSize = font().pointSize()*1.6;
    QPixmap pixm = reload_icon_.pixmap(QSize(__newPointSize,__newPointSize),mode__);


    QColor __newColor;
    if(!colour__.isValid())
        __newColor = defaultFontColour();
    else
        __newColor = colour__;


    // Convert the pixmap to QImage
    QImage tmp = pixm.toImage();


    // Loop all the pixels
    for(int y = 0; y < tmp.height(); y++)
    {
        for(int x= 0; x < tmp.width(); x++)
        {
            // Read the alpha value each pixel, keeping the RGB values of your color
            __newColor.setAlpha(tmp.pixelColor(x,y).alpha());

            // Apply the pixel color
            tmp.setPixelColor(x,y,__newColor);

         }
     }
    // Get the coloured pixmap
    pixm = QPixmap::fromImage(tmp);

    // Set coloured pixmap image to the label
    button_icon_lbl_->setPixmap(pixm);
}



void reloadButton::changeEvent(QEvent *event__)
{
    if(event__->type() == QEvent::EnabledChange){
        if(!isEnabled()){
            //reloadIconChange(QIcon::Disabled);
            repaintFont(this->palette().buttonText().color()/*defaultFontColour()*/, QIcon::Disabled);
        } else{
            //reloadIconChange(QIcon::Normal);
            repaintFont(/*this->palette().buttonText().color()*/defaultFontColour(), QIcon::Normal);
        }
    }

    QPushButton::changeEvent(event__);

}


bool reloadButton::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::HoverEnter && isEnabled())
    {
        repaintFont( hoverFontColour_);

    // Whatever you want to do when mouse goes over targetPushButton
    } else if(event->type() == QEvent::HoverLeave && !hasFocus() && isEnabled()){

        repaintFont( defaultFontColour_);

    } else if(event->type() == QEvent::HoverLeave && hasFocus()){

        repaintFont(focusFontColour_);

    } else if(event->type() == QEvent::FocusIn ){

        repaintFont( hoverFontColour_);

    } else if(event->type() == QEvent::FocusOut){

        repaintFont( defaultFontColour_);
    }


    // pass the event on to the parent class
    return BlinkinButton::eventFilter(obj, event);

}


