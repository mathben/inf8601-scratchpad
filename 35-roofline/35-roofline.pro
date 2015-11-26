QT += core
QT -= gui

TARGET = 35-roofline
CONFIG += console
CONFIG -= app_bundle

CONFIG += C++11
LIBS += -ltbb

TEMPLATE = app

SOURCES += main.cpp

