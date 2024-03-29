QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common.cpp \
    execsqlthread.cpp \
    execsqlwidget.cpp \
    getrealtimedata.cpp \
    highlighter.cpp \
    logfile.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    setalertrulewidget.cpp \
    settingsdialog.cpp \
    showalertruledialog.cpp \
    showreadonlytextdialog.cpp \
    showrealtimedatawidget.cpp \
    tdenginedb.cpp

HEADERS += \
    common.h \
    execsqlthread.h \
    execsqlwidget.h \
    getrealtimedata.h \
    highlighter.h \
    logfile.h \
    logindialog.h \
    mainwindow.h \
    setalertrulewidget.h \
    settingsdialog.h \
    showalertruledialog.h \
    showreadonlytextdialog.h \
    showrealtimedatawidget.h \
    tdenginedb.h

FORMS += \
    execsqlwidget.ui \
    logindialog.ui \
    mainwindow.ui \
    setalertrulewidget.ui \
    settingsdialog.ui \
    showalertruledialog.ui \
    showreadonlytextdialog.ui \
    showrealtimedatawidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

LIBS += -LC:/TDengine/driver/ -ltaos

INCLUDEPATH += C:/TDengine/include
DEPENDPATH += C:/TDengine/include

RC_ICONS = crane.ico
