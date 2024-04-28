QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    create_db_name.cpp \
    custom_query.cpp \
    databases.cpp \
    db_connection.cpp \
    delete_db.cpp \
    main.cpp \
    loginwindow.cpp \
    select_cells.cpp \
    tables.cpp

HEADERS += \
    auth.h \
    create_db_name.h \
    custom_query.h \
    databases.h \
    db_connection.h \
    delete_db.h \
    loginwindow.h \
    select_cells.hpp \
    tables.h

FORMS += \
    create_db_name.ui \
    custom_query.ui \
    databases.ui \
    delete_db.ui \
    loginwindow.ui \
    tables.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
