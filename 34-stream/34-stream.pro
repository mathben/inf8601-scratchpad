#-------------------------------------------------
#
# Project created by QtCreator 2015-11-23T20:28:28
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 34-stream
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -fopenmp
QMAKE_CFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

SOURCES += main.cpp
