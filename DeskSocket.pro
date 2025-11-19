QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++11

VERSION = 24.07.05.1100

lessThan(QT_MAJOR_VERSION, 6): win32: QMAKE_CXXFLAGS += -execution-charset:utf-8

# 禁用 QDebug 输出调试i信息
#DEFINES += QT_NO_DEBUG_OUTPUT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/Sdk/spdlog/include
INCLUDEPATH += $$PWD/Sdk/hpsocket/include

QMAKE_CXXFLAGS_WARN_OFF += -Wunused-parameter

SOURCES += \
    Control/Message/messagewidget.cpp \
    Dialog/dialognetparameter.cpp \
    Log/logger.cpp \
    Network/networkmanager.cpp \
    Network/tcpserver.cpp \
    Network/tcpsocket.cpp \
    Network/networkobject.cpp \
    Network/udpbroadcast.cpp \
    Network/udpserver.cpp \
    Network/udpsocket.cpp \
    Public/appconfig.cpp \
    Public/appsignal.cpp \
    Public/appconstants.cpp \
    Widget/Tools/widgetformatjson.cpp \
    Widget/widgetappsetting.cpp \
    Widget/Tools/widgetformatpos.cpp \
    Widget/widgetnetwrokobjectview.cpp \
    Widget/widgetpagecontainer.cpp \
    Widget/widgetservercontent.cpp \
    Widget/widgetsocketcontent.cpp \
    Widget/widgetversioncontent.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Control/Message/messagewidget.h \
    Dialog/dialognetparameter.h \
    Log/logger.h \
    Network/networkmanager.h \
    Network/tcpserver.h \
    Network/tcpsocket.h \
    Network/networkobject.h \
    Network/udpbroadcast.h \
    Network/udpserver.h \
    Network/udpsocket.h \
    Public/appconfig.h \
    Public/appsignal.h \
    Public/defines.h \
    Public/appconstants.h \
    Public/treeitemdelegate.h \
    Widget/Tools/widgetformatjson.h \
    Widget/widgetappsetting.h \
    Widget/Tools/widgetformatpos.h \
    Widget/widgetnetwrokobjectview.h \
    Widget/widgetpagecontainer.h \
    Widget/widgetservercontent.h \
    Widget/widgetsocketcontent.h \
    Widget/widgetversioncontent.h \
    mainwindow.h

FORMS += \
    Dialog/dialognetparameter.ui \
    Widget/Tools/widgetformatjson.ui \
    Widget/widgetappsetting.ui \
    Widget/Tools/widgetformatpos.ui \
    Widget/widgetnetwrokobjectview.ui \
    Widget/widgetpagecontainer.ui \
    Widget/widgetservercontent.ui \
    Widget/widgetsocketcontent.ui \
    Widget/widgetversioncontent.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

RC_ICONS = icon.ico

#0x0800代表和系统当前语言一致
RC_LANG = 0x0800

LIBS += -L$$PWD/Sdk/hpsocket/lib \
-lHPSocket
