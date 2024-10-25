#include "blinkinbutton.h"

BlinkinButton::BlinkinButton(QWidget *parent) :
    QPushButton(parent)
  , timer{new QTimer{this}}
{
//    init_signals();
    connect(timer, &QTimer::timeout, this, &BlinkinButton::update);
}

BlinkinButton::BlinkinButton(const QString &text, QWidget *parent) :
    QPushButton(text,parent)
  , timer{new QTimer{this}}
{
    //init_signals();
    connect(timer, &QTimer::timeout, this, &BlinkinButton::update);
}

BlinkinButton::~BlinkinButton()
{
    delete timer;
    ////qDebug() << "~BlinkinButton destroyed";
}

void BlinkinButton::update()
{
    if((buttonState=!buttonState))
        this->setStyleSheet("background: palette(window)");
    else
        this->setStyleSheet("background: lightgreen");
    ////qDebug()<<"update...";
}

void BlinkinButton::start_blinkin(int freq_sec)
{
    timer->start(freq_sec);
}

void BlinkinButton::stop_blinkin()
{
    if(is_active()){
        timer->stop();
        this->setStyleSheet("background:palette(window)");
    }
}

bool BlinkinButton::is_active()
{
    return timer->isActive();
}

//void BlinkinButton::init_signals()
//{
//    connect(timer, &QTimer::timeout, this, &BlinkinButton::update);
//}
