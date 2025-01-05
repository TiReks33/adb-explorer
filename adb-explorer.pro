QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "/home/alexander/qtcreator_proj/adb-crypt" #for headers; not neccessary if dynamic lib used

LIBS += -L"/home/alexander/qtcreator_proj/build-adb-crypt-Desktop-Release" -ladb-crypt

SOURCES += \
    adb-exp-utility.cpp \
    auth.cpp \
    blinkinbutton.cpp \
    clickablelabel.cpp \
    create_db_name.cpp \
    create_table_constructor.cpp \
    createtupleconstructor.cpp \
    custom_query.cpp \
    customqueryresult.cpp \
    customquerysettings.cpp \
    databases.cpp \
    db_connection.cpp \
    delete_db.cpp \
    delete_form.cpp \
    delete_sqldb_user.cpp \
    delete_table.cpp \
    fontembeddedwidget.cpp \
    hidemenu.cpp \
    main.cpp \
    loginwindow.cpp \
    notifycombobox.cpp \
    reloadbutton.cpp \
    scrolledstatusbar.cpp \
    select_cells.cpp \
    signaltableview.cpp \
    sqldump.cpp \
    sqlusermanagement.cpp \
    tables.cpp \
    twolistselection.cpp

HEADERS += \
    adb-exp-utility.h \
    auth.h \
    blinkinbutton.h \
    clickablelabel.h \
    create_db_name.h \
    create_table_constructor.h \
    createtupleconstructor.h \
    custom_query.h \
    customqueryresult.h \
    customquerysettings.h \
    databases.h \
    db_connection.h \
    delete_db.h \
    delete_form.h \
    delete_sqldb_user.h \
    delete_table.h \
    fontembeddedwidget.h \
    hidemenu.h \
    loginwindow.h \
    notifycombobox.h \
    reloadbutton.h \
    scrolledstatusbar.h \
    select_cells.hpp \
    signaltableview.h \
    sqldump.h \
    sqlusermanagement.h \
    tables.h \
    twolistselection.h

FORMS += \
    create_db_name.ui \
    create_table_constructor.ui \
    createtupleconstructor.ui \
    custom_query.ui \
    customqueryresult.ui \
    customquerysettings.ui \
    databases.ui \
    delete_form.ui \
    loginwindow.ui \
    tables.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc

DISTFILES += \
    CopyrightNoticeOfUsedSources
