#include "helping_stuff.h"

//QString pack_(const QStringList &list, QChar separator)
//{
//    return list.join(separator);
//}

QString pack_(const QStringList &list, QString const& separator)
{
    return list.join(separator);
}

//QString pack_(const QStringList &list/*, QString separator*/)
//{
//    return list.join(/*separator*/ ", ");
//}

//QStringList unpack_(const QString &string, QChar separator)
//{
//    return string.split(separator);
//}

QStringList unpack_(const QString &string, QString const& separator)
{
    return string.split(separator);
}

//QStringList unpack_(const QString &string/*, QString separator*/)
//{
//    return string.split(/*separator*/ ", ");
//}

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


void replace_all(
    std::string& s,
    std::string const& toReplace,
    std::string const& replaceWith
) {
    std::string buf;
    std::size_t pos = 0;
    std::size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.find(toReplace, pos);
        if (pos == std::string::npos)
            break;
        buf.append(s, prevPos, pos - prevPos);
        buf += replaceWith;
        pos += toReplace.size();
    }

    buf.append(s, prevPos, s.size() - prevPos);
    s.swap(buf);
}

void replace_all(QString& s,QString const& toReplace,QString const& replaceWith) {
    QString buf;
    size_t pos = 0;
    size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(s.size());

    while (true) {
        prevPos = pos;
        pos = s.indexOf(toReplace, pos);
        if (pos == size_t(-1))
            break;
//        QString _SUBSTR_ = s.mid(prevPos, pos - prevPos);
//        buf.append(_SUBSTR_);
        buf.append(s.mid(prevPos, pos - prevPos));
        buf += replaceWith;
        pos += toReplace.size();
    }
//    QString _SUBSTR_2 = s.mid(prevPos, s.size() - prevPos);
//    buf.append(_SUBSTR_2);
    buf.append(s.mid(prevPos, s.size() - prevPos));
    s.swap(buf);
}


QString replace_all(const QString &str, const QString &toReplace, const QString &replaceWith)
{
    QString final_string = str;
    QString buf;
    size_t pos = 0;
    size_t prevPos;

    // Reserves rough estimate of final size of string.
    buf.reserve(final_string.size());

    while (true) {
        prevPos = pos;
        pos = final_string.indexOf(toReplace, pos);
        if (pos == size_t(-1))
            break;
//        QString _SUBSTR_ = s.mid(prevPos, pos - prevPos);
//        buf.append(_SUBSTR_);
        buf.append(final_string.mid(prevPos, pos - prevPos));
        buf += replaceWith;
        pos += toReplace.size();
    }
//    QString _SUBSTR_2 = s.mid(prevPos, s.size() - prevPos);
//    buf.append(_SUBSTR_2);
    buf.append(final_string.mid(prevPos, final_string.size() - prevPos));
    final_string.swap(buf);
    return final_string;
}

QString escape_sql_backticks(const QString &str__)
{
    QString copy_str{str__};
    replace_all(copy_str,"`","``");
    return copy_str;
}

QStringList escape_sql_backticks(const QStringList list__)
{
    QStringList new_list_with_esc;

    for( const auto& i : list__)
    {
        new_list_with_esc.append(escape_sql_backticks(i));
    }

    return new_list_with_esc;
}
