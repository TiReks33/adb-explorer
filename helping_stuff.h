#ifndef HELPING_STUFF_H
#define HELPING_STUFF_H
#include <QString>
#include <QStringList>
#include <QPlainTextEdit>

//TextCursor things
void set_cursor_to_end_(QPlainTextEdit*);

//move window to the center of a previuous window
void window_center_from_another_(QWidget*,QWidget*);

//QStringList-->QString
//QString pack_(QStringList const& list, QChar separator=',');
QString pack_(QStringList const& list, QString const& separator=", ");
//QString pack_(QStringList const& list/*, QString separator=", "*/);

//QString-->QStringList
//QStringList unpack_(QString const& string, QChar separator=',');
QStringList unpack_(QString const& string, QString const& separator=", ");
//QStringList unpack_(QString const& string/*, QString separator=", "*/);

//std::string
void replace_all( std::string& s, std::string const& toReplace, std::string const& replaceWith);

//QString
void replace_all(QString& s,QString const& toReplace,QString const& replaceWith);
QString replace_all(QString const& s,QString const& toReplace /*= "`"*/,QString const& replaceWith /*= "``"*/);
QString escape_sql_backticks(QString const&);

#endif // HELPING_STUFF_H
