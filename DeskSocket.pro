QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++11

VERSION = 22.01.07.03

lessThan(QT_MAJOR_VERSION, 6): win32: QMAKE_CXXFLAGS += -execution-charset:utf-8

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/Libs/spdlog/include
INCLUDEPATH += $$PWD/Libs/hpsocket/include

LIBS += -l$$PWD/Libs/hpsocket/lib/HPSocket_U

DESTDIR = ..\output

QMAKE_CXXFLAGS_WARN_OFF += -Wunused-parameter

SOURCES += \
    Dialog/dialogabout.cpp \
    Dialog/dialognote.cpp \
    Dialog/dialogserverargs.cpp \
    Log/logger.cpp \
    Manager/clientmanager.cpp \
    Manager/servermanager.cpp \
    Net/tcpserver.cpp \
    Net/tcpsocket.cpp \
    Public/appsignal.cpp \
    Public/softconstants.cpp \
    Widget/widgetsocketview.cpp \
    Widget/widgettabcontent.cpp \
    Widget/widgettabpages.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Dialog/dialogabout.h \
    Dialog/dialognote.h \
    Dialog/dialogserverargs.h \
    Log/logger.h \
    Manager/clientmanager.h \
    Manager/servermanager.h \
    Net/tcpserver.h \
    Net/tcpsocket.h \
    Public/appsignal.h \
    Public/defines.h \
    Public/softconstants.h \
    Public/threadpool.h \
    Public/treeitemdelegate.h \
    Widget/widgetsocketview.h \
    Widget/widgettabcontent.h \
    Widget/widgettabpages.h \
    mainwindow.h

FORMS += \
    Dialog/dialogabout.ui \
    Dialog/dialognote.ui \
    Dialog/dialogserverargs.ui \
    Widget/widgetsocketview.ui \
    Widget/widgettabcontent.ui \
    Widget/widgettabpages.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

RC_ICONS = icon.ico

#0x0800代表和系统当前语言一致
RC_LANG = 0x0800
