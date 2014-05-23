#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T15:06:02
#
#-------------------------------------------------

QT       += core gui network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlogClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login2.cpp \
    cookiejar.cpp

HEADERS  += mainwindow.h \
    login2.h \
    cookiejar.h

FORMS    += mainwindow.ui \
    login2.ui

OTHER_FILES += \
    ReadMe.md
