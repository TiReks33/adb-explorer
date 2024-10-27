#include "blinkinbutton.h"



BlinkinButton::BlinkinButton(QWidget *parent__, const QString &text__) :
    QPushButton(text__,parent__)
  , timer{new QTimer{this}}
  , dfltStyleSheet_(this->styleSheet())
  , blinkStyleSheet_("background: lightgreen")
{
    //init_signals();
    connect(timer, &QTimer::timeout, this, &BlinkinButton::update);
}

BlinkinButton::~BlinkinButton()
{
    delete timer;
}

void BlinkinButton::update()
{
    if((buttonState=!buttonState)){
        this->QPushButton::setStyleSheet(dfltStyleSheet_/*"background: palette(button)"*/);

    } else{
        this->QPushButton::setStyleSheet(blinkStyleSheet_);
    }

}


void BlinkinButton::start_blinkin(int freq_sec, int timelimit)
{
    if(timelimit){
        QPointer<QTimer> tempTimer = new QTimer;
        tempTimer->setInterval(timelimit);
        tempTimer->setSingleShot(true);
        auto con1 = QObject::connect(tempTimer,&QTimer::timeout,this,&BlinkinButton::stop_blinkin);

        auto con2 = QObject::connect(this,&BlinkinButton::blinkStopped,this,[tempTimer]{
//            qDebug() << "tempTimer interrupted";
            if(tempTimer){
                if(tempTimer->isActive()){
                    tempTimer->stop();
                }
            }
            delete tempTimer;
        }, Qt::AutoConnection);

        auto con3 = QObject::connect(this,&BlinkinButton::destroyed,this,[tempTimer]{
//            qDebug() << "~tempTimer lambda call";
            if(tempTimer){
                delete tempTimer;
            }
        }, Qt::AutoConnection);

        connect(tempTimer,&QTimer::destroyed,[con1,con2,con3]{
            disconnect(con1);
            disconnect(con2);
            disconnect(con3);
        });

        tempTimer->start();
        //QTimer::singleShot(timelimit, this, &BlinkinButton:: stop_blinkin);
    }

    timer->start(freq_sec);
}



void BlinkinButton::stop_blinkin()
{
    if(is_active()){
        timer->stop();
        this->QPushButton::setStyleSheet(/*"background:palette(button)"*/dfltStyleSheet_);
        emit blinkStopped();
    }
}

bool BlinkinButton::is_active()
{
    return timer->isActive();
}


