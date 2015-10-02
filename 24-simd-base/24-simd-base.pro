#-------------------------------------------------
#
# Project created by QtCreator 2015-10-02T15:23:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 24-simd-base
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    mmx.S

OTHER_FILES += \
    mmx.S

HEADERS += \
    mmx.h
