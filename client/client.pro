QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttongroup.cpp \
    common/clientinfoinstance.cpp \
    common/downloadlayout.cpp \
    common/downloadqueue.cpp \
    common/encrypttool.cpp \
    common/jsontool.cpp \
    common/logger.cpp \
    common/networktool.cpp \
    common/uploadlayout.cpp \
    common/uploadqueue.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    myfilewidget.cpp \
    transportbar.cpp \
    transportwidget.cpp \
    widgetlogintitle.cpp \
    windowmanager.cpp

HEADERS += \
    buttongroup.h \
    common/clientinfoinstance.h \
    common/config.h \
    common/downloadlayout.h \
    common/downloadqueue.h \
    common/encrypttool.h \
    common/jsontool.h \
    common/logger.h \
    common/networktool.h \
    common/uploadlayout.h \
    common/uploadqueue.h \
    login.h \
    mainwindow.h \
    myfilewidget.h \
    structs/fileinfo.h \
    structs/foxcloudclientinfo.h \
    structs/httpreplaycode.h \
    structs/userinfo.h \
    structs/webserverinfo.h \
    transportbar.h \
    transportwidget.h \
    widgetlogintitle.h \
    windowmanager.h

FORMS += \
    buttongroup.ui \
    login.ui \
    mainwindow.ui \
    myfilewidget.ui \
    transportbar.ui \
    transportwidget.ui \
    widgetlogintitle.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# TARGET = foxcloud

RESOURCES += \
    resource.qrc

RC_ICONS = ./img/foxcloud-logo-name.ico
