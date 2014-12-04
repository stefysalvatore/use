#-------------------------------------------------
#
# Project created by QtCreator 2014-12-03T12:40:26
#
#-------------------------------------------------

QT       += core
QT       += network
QT       += sql

QT       -= gui


TARGET = ServerGeomarketing
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    myserver.cpp \
    mythread.cpp

HEADERS += \
    myserver.h \
    mythread.h
