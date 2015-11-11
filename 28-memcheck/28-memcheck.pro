QT += core
QT -= gui

TARGET = 28-memcheck
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

# utilisation de MemorySanitizer
#QMAKE_CXX = clang++
#QMAKE_LINK = clang++
#QMAKE_CXXFLAGS = -fsanitize=memory -fPIE
#QMAKE_LIBS = -fsanitize=memory -pie

SOURCES += main.cpp

