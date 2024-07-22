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
