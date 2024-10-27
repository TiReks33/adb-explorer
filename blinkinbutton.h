#ifndef BLINKINBUTTON_H
#define BLINKINBUTTON_H

#include <QPushButton>
#include <QTimer>
#include <QDebug>
#include <QPointer>

class BlinkinButton : public QPushButton
{

    Q_OBJECT

public:

    explicit BlinkinButton(QWidget *parent = nullptr,const QString &text = NULL);

    virtual ~BlinkinButton();

    inline void setStyleSheet(QString const& newSS__){

        dfltStyleSheet_ = newSS__;

        QPushButton::setStyleSheet(newSS__);
    };


    inline void changeBlinkSS(QString const& newBlinkSS__){
        blinkStyleSheet_ = newBlinkSS__;
    };

    inline QString const & blinkSS(){
        return blinkStyleSheet_;
    };

private slots:
    virtual void update();

public slots:
    void start_blinkin(int freq_sec = 1000, int timelimit = 0);
    virtual void stop_blinkin();

    bool is_active();

signals:

    void blinkStopped();

protected:
    QPointer<QTimer> timer;

    bool buttonState=true;

private:

    QString dfltStyleSheet_ = "";

    QString blinkStyleSheet_ = "";

};

#endif // BLINKINBUTTON_H
