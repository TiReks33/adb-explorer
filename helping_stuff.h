#ifndef HELPING_STUFF_H
#define HELPING_STUFF_H
#include <QString>
#include <QStringList>

//QStringList-->QString
//QString pack_(QStringList const& list, QChar separator=',');
//QString pack_(QStringList const& list, QString separator=", ");
QString pack_(QStringList const& list/*, QString separator=", "*/);

//QString-->QStringList
//QStringList unpack_(QString const& string, QChar separator=',');
//QStringList unpack_(QString const& string, QString separator=", ");
QStringList unpack_(QString const& string/*, QString separator=", "*/);

#endif // HELPING_STUFF_H
