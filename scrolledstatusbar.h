#ifndef SCROLLEDSTATUSBAR_H
#define SCROLLEDSTATUSBAR_H

#include <QMainWindow>
#include <QLayout>
#include <QScrollBar>
#include <QLineEdit>
#include <QDebug>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <cmath>
#include <QMouseEvent>

#include "adb-exp-utility.h"

//QT_BEGIN_NAMESPACE
//namespace Ui { class scrolledStatusBar; }
//QT_END_NAMESPACE

class statusLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit statusLineEdit(QWidget *parent = 0);

    void setText(QString const&);

private slots:
    void mousePressEvent(QMouseEvent *e);

    void keyPressEvent(QKeyEvent *e);

signals:
    void rightClicked();

};


class scrolledStatusBar : public QWidget
{
    Q_OBJECT

public:
    scrolledStatusBar(QWidget *parent = nullptr);
    ~scrolledStatusBar();

protected:
    void resizeEvent(QResizeEvent *event);

public slots:

    inline statusLineEdit* get_line(){ return statusLine;}
    inline QScrollBar* get_scroll(){ return hor_scrollbar;}

private:
    statusLineEdit* statusLine;
    QScrollBar* hor_scrollbar;

    int maximum=0;
    int width=0;
};
#endif // SCROLLEDSTATUSBAR_H
