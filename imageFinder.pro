#-------------------------------------------------
#
# Project created by QtCreator 2021-02-11T11:58:21
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imageFinder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    downloader.cpp

HEADERS  += mainwindow.h \
    downloader.h

FORMS    += mainwindow.ui
