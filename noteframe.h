#ifndef NOTEFRAME_H
#define NOTEFRAME_H

#include <QFrame>
#include <QObject>
#include <QLayout>
#include <QPushButton>
#include <QDebug>

#include "clickablelabel.h"

class noteFrame : public QFrame
{
    Q_OBJECT
public:
    explicit noteFrame(const QString &, QWidget* = nullptr);
    virtual ~noteFrame(){ /*qDebug() << "~noteFrame()";*/};

signals:
    void dontShowNoteAgainSig();

private:
    ClickableLabel* dyn_note_lbl_ = nullptr;
    QPushButton* dyn_note_hide_button_ = nullptr;
    QPushButton* dyn_note_disable_button_ = nullptr;

    void initForm();
    void initSig();
};

#endif // NOTEFRAME_H
