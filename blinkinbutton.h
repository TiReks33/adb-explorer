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
    explicit BlinkinButton(QWidget *parent = nullptr);
    explicit BlinkinButton(const QString &text, QWidget *parent = nullptr);

    virtual ~BlinkinButton();

private slots:
    void update();

public slots:
    void start_blinkin(int freq_sec = 1000);
    void stop_blinkin();

    bool is_active();

private:
    QPointer<QTimer> timer;
    bool buttonState=true;

//    void init_signals();
};

#endif // BLINKINBUTTON_H
