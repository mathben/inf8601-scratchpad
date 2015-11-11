#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T09:23:16
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 28-sanitize-race
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXX = clang++
QMAKE_LINK = clang++
QMAKE_CXXFLAGS = -fsanitize=thread
QMAKE_LIBDIR += /usr/local/lib/linux/
LIBS += -lclang_rt.tsan-x86_64 -ldl -lpthread -lrt


SOURCES += main.cpp
