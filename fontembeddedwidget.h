#ifndef FONTEMBEDDEDWIDGET_H
#define FONTEMBEDDEDWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QFontDialog>
#include <QCheckBox>
#include <QPointer>
#include <QDebug>
#include <QPainter>

class fontEmbeddedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit fontEmbeddedWidget(QWidget *parent__ /*= nullptr*/);

    virtual ~fontEmbeddedWidget();

    inline QPushButton*& getButton(){ return setFontButton_;}

signals:

    void defaultFontChanged(QString const&);

private:
    QPointer<QWidget> parent_ = nullptr;
    void initSignals();
    void initForm();

    QPushButton* setFontButton_ = nullptr;

    QPointer<QCheckBox> defaultFontCheckBox_;
};

#endif // FONTEMBEDDEDWIDGET_H
