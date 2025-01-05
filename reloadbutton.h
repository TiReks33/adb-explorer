#ifndef RELOADBUTTON_H
#define RELOADBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QApplication>
#include <QLayout>
#include <QLabel>
#include <QStyle>
#include <QDebug>

#include "blinkinbutton.h"
#include "adb-exp-utility.h"


enum ReloadButtonState {
    Default,
    Blinkin
};

class reloadButton : public BlinkinButton/*QPushButton*/
{
    Q_OBJECT
public:
    explicit reloadButton(QWidget* parent__ = nullptr, QColor bgColor__ = QColor(), QColor fontColor__ = QColor(),bool fontBold = false, bool fontItalic = false, QString const& alterText__ = NULL);
    virtual ~reloadButton();


    void setText(const QString &newText__);

    inline void setStyleSheet(QString const& newSS__) { QPushButton::setStyleSheet(newSS__); };

    inline void repaintBackground(QColor newColour__){

        QPalette pal = this->palette();

        pal.setColor(QPalette::Button, newColour__);

        setPalette(pal);
    }


    inline void repaintFont(QColor newColour__, QIcon::Mode iconPixmapMode__ = QIcon::Normal){


        QPalette textpal = button_rich_text_lbl_->palette();

        textpal.setColor(QPalette::ButtonText, newColour__);

        button_rich_text_lbl_->setPalette(textpal);

        reloadIconChange(iconPixmapMode__,newColour__);
    }


    inline void setBackgroundColour(QColor newColour__,bool repaintNow = true){

        defaultBgColour_ = newColour__;

        if(!alternateBlinkinColour_)
            blinkinBgColour_ = adb_style::rgb_reverse(newColour__);

        if(repaintNow)
            repaintBackground(newColour__);
    }

    inline void setFontColour(QColor newColour__,bool repaintNow = true){

        defaultFontColour_ = newColour__;

        if(!alternateBlinkinColour_)
            blinkinFontColour_ = adb_style::rgb_reverse(newColour__);

        if(repaintNow)
            repaintFont(newColour__);
    }

    inline void setAlternateBlinkinFontColour(QColor newColour__){

        blinkinFontColour_ = newColour__;

        alternateBlinkinColour_ = true;
    }

    inline void setAlternateBlinkinBackgroundColour(QColor newColour__){

        blinkinBgColour_ = newColour__;

        alternateBlinkinColour_ = true;
    }

    inline void setBlinkinReverseRGB(){ alternateBlinkinColour_ = false;}

    void setBold(bool);

    void setItalic(bool);

    void setFont(QFont const &);

    void setFontPointSize(int newPointSize__);

    inline QColor defaultBackgroundColour() const{ return defaultBgColour_;}

    inline QColor defaultFontColour() const{ return defaultFontColour_;}

    inline QColor blinkinBackgroundColour() const{ return blinkinBgColour_;}

    inline QColor blinkinFontColour() const{ return blinkinFontColour_;}


    //
    inline QFont const& font() const{ return button_rich_text_lbl_->font(); }
    //

    inline bool isBold() const{ return this->font().bold();}

    inline bool isItalic() const{ return this->font().italic();}

    inline int fontPointSize() const{ return this->font().pointSize();}

    inline QString currentText() const{ return button_rich_text_lbl_->text();}

    inline void backupStyleSheet(){ styleSheetBackup_ = styleSheet();}

    inline void restoreStyleSheet(){ setStyleSheet(styleSheetBackup_);}

    void setKhakiHighlight();

    void removeKhakiHighlight();


    void changeBlinkSS(QString const&) = delete;

    QString const& blinkSS() = delete;

private slots:
    virtual void update() override;

public slots:

    virtual void stop_blinkin() override;

signals:

    void hovered();
    void leave();
    void focusIn();
    void focusOut();

protected:
    virtual void enterEvent( QEvent* e ) override;
    virtual void leaveEvent( QEvent* e) override;
    virtual void focusInEvent (QFocusEvent* e) override;
    virtual void focusOutEvent(QFocusEvent*e) override;

private:
    QIcon const reload_icon_;

    QLabel* button_icon_lbl_ = nullptr;
    QLabel* button_rich_text_lbl_ = nullptr;

    void reloadIconChange(QIcon::Mode = QIcon::Normal, QColor = QColor()/*, ReloadButtonState state = ReloadButtonState::Default*/);

    void changeEvent(QEvent *) override;

    QColor defaultBgColour_;
    QColor defaultFontColour_;

    QColor blinkinBgColour_;
    QColor blinkinFontColour_;

    QColor hoverFontColour_;
    QColor focusFontColour_;



    bool alternateBlinkinColour_=false;

    QString styleSheetBackup_ = "";



    bool eventFilter(QObject *obj, QEvent *event) override;

//    using BlinkinButton::changeBlinkSS;

};

#endif // RELOADBUTTON_H
