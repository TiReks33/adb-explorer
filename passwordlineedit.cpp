#include "passwordlineedit.h"

PasswordLineEdit::PasswordLineEdit(QWidget *parent):
    QLineEdit(parent)
  , m_on(":/pic/eyeOn1.png")
  , m_off(":/pic/eyeOff0.png")
{
    setEchoMode(QLineEdit::Password);
    QAction *action = addAction(m_off, QLineEdit::TrailingPosition);
    button = qobject_cast<QToolButton *>(action->associatedWidgets().last());
    button->hide();
    button->setCursor(QCursor(Qt::PointingHandCursor));
    connect(button, &QToolButton::pressed, this, &PasswordLineEdit::onPressed);
    connect(button, &QToolButton::released, this, &PasswordLineEdit::onReleased);
}

void PasswordLineEdit::onPressed(){
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    button->setIcon(m_on);
    setEchoMode(QLineEdit::Normal);
}

void PasswordLineEdit::onReleased(){
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    button->setIcon(m_off);
    setEchoMode(QLineEdit::Password);
}

void PasswordLineEdit::enterEvent(QEvent *event){
    button->show();
    QLineEdit::enterEvent(event);
}

void PasswordLineEdit::leaveEvent(QEvent *event){
    button->hide();
    QLineEdit::leaveEvent(event);
}

void PasswordLineEdit::focusInEvent(QFocusEvent *event){
    button->show();
    QLineEdit::focusInEvent(event);
}

void PasswordLineEdit::focusOutEvent(QFocusEvent *event){
    button->hide();
    QLineEdit::focusOutEvent(event);
}


