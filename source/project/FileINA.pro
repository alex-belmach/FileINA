#-------------------------------------------------
#
# Project created by QtCreator 2015-03-15T23:55:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileINA
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    pane.cpp \
    settings.cpp \
    properties.cpp \
    pastethread.cpp \
    deletethread.cpp \
    copythread.cpp \
    copyprogress.cpp \
    sizethread.cpp \
    findwindow.cpp

HEADERS  += mainwindow.h \
    pane.h \
    settings.h \
    properties.h \
    pastethread.h \
    deletethread.h \
    copythread.h \
    copyprogress.h \
    sizethread.h \
    findwindow.h

FORMS    += \
    copyprogress.ui \
    findwindow.ui

RESOURCES += \
    Resources.qrc
