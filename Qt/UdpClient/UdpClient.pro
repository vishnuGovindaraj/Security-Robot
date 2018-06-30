#-------------------------------------------------
#
# Project created by QtCreator 2014-03-18T16:41:31
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UdpClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    udpclient.cpp

HEADERS  += mainwindow.h \
    udpclient.h

FORMS    += mainwindow.ui
