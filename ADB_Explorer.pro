QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    create_db_name.cpp \
    custom_query.cpp \
    customqueryresult.cpp \
    customquerysettings.cpp \
    databases.cpp \
    db_connection.cpp \
    delete_db.cpp \
    delete_table.cpp \
    main.cpp \
    loginwindow.cpp \
    newtable.cpp \
    select_cells.cpp \
    tables.cpp

HEADERS += \
    auth.h \
    create_db_name.h \
    custom_query.h \
    customqueryresult.h \
    customquerysettings.h \
    databases.h \
    db_connection.h \
    delete_db.h \
    delete_table.h \
    loginwindow.h \
    newtable.h \
    select_cells.hpp \
    tables.h

FORMS += \
    create_db_name.ui \
    custom_query.ui \
    customqueryresult.ui \
    customquerysettings.ui \
    databases.ui \
    delete_db.ui \
    loginwindow.ui \
    newtable.ui \
    tables.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
