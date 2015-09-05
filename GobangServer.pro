#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T11:24:13
#
#-------------------------------------------------

QT       += core gui
QT       += network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GobangServer
TEMPLATE = app


SOURCES += main.cpp\
        gobang.cpp \
    pieces.cpp \
    step.cpp \
    mainwindow.cpp \
    startmenu.cpp \
    networkserver.cpp \
    waitwidget.cpp \
    enterdialog.cpp \
    winwidget.cpp \
    gobangplat.cpp

HEADERS  += gobang.h \
    pieces.h \
    step.h \
    mainwindow.h \
    startmenu.h \
    networkserver.h \
    waitwidget.h \
    enterdialog.h \
    winwidget.h \
    gobangplat.h

FORMS    += \
    mainwindow.ui \
    gobang.ui \
    startmenu.ui \
    waitwidget.ui \
    enterdialog.ui \
    winwidget.ui

RESOURCES += \
    resource.qrc

RC_ICONS = Icon.ico
