#include "helping_stuff.h"

//QString pack_(const QStringList &list, QChar separator)
//{
//    return list.join(separator);
//}

//QString pack_(const QStringList &list, QString separator)
//{
//    return list.join(separator);
//}

QString pack_(const QStringList &list/*, QString separator*/)
{
    return list.join(/*separator*/ ", ");
}

//QStringList unpack_(const QString &string, QChar separator)
//{
//    return string.split(separator);
//}

//QStringList unpack_(const QString &string, QString separator)
//{
//    return string.split(separator);
//}

QStringList unpack_(const QString &string/*, QString separator*/)
{
    return string.split(/*separator*/ ", ");
}

void set_cursor_to_end_(QPlainTextEdit *plainTextEdit)
{
    QTextCursor cursor = plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    plainTextEdit->setTextCursor(cursor);
}

void window_center_from_another_(QWidget *old_window_,QWidget*new_window_)
{
    QPoint centerPoint = old_window_->geometry().center();

    //new_window_->adjustSize();
    new_window_->move(centerPoint.x() - new_window_->width()/2, centerPoint.y() - new_window_->height()/2);
}
