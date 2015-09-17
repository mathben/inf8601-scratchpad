#-------------------------------------------------
#
# Project created by QtCreator 2015-09-16T22:16:06
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 14-speedup
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app
LIBS += -ltbb

SOURCES += main.cpp
