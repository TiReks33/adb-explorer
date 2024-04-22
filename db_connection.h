#ifndef DB_CONNECTION_H
#define DB_CONNECTION_H
#include <QSqlDatabase>
#include <QDebug>

#include "auth.h"

bool db_connect(auth&);

void db_close();

#endif // DB_CONNECTION_H
