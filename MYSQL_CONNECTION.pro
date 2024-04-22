QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databases.cpp \
    db_connection.cpp \
    main.cpp \
    loginwindow.cpp \
    select_cells.cpp \
    tables.cpp

HEADERS += \
    auth.h \
    databases.h \
    db_connection.h \
    loginwindow.h \
    select_cells.hpp \
    tables.h

FORMS += \
    databases.ui \
    loginwindow.ui \
    tables.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
