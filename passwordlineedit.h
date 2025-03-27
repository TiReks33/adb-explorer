#ifndef PASSWORDLINEEDIT_H
#define PASSWORDLINEEDIT_H

#include <QAction>
#include <QLineEdit>
#include <QToolButton>

class PasswordLineEdit: public QLineEdit
{
public:
    explicit PasswordLineEdit(QWidget *parent=nullptr);
    virtual ~PasswordLineEdit(){};
private slots:
    void onPressed();
    void onReleased();
protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
private:
    QToolButton *button;
    QIcon   m_on;
    QIcon   m_off;
};


#endif // PASSWORDLINEEDIT_H
