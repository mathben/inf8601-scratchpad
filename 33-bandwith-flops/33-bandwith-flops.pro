#-------------------------------------------------
#
# Project created by QtCreator 2015-11-18T19:25:31
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 33-bandwith-flops
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += C++11
LIBS += -ltbb

SOURCES += main.cpp \
    benchmark.cpp

HEADERS += \
    benchmark.h
