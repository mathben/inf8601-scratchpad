#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T08:21:24
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 01-mpi-rank
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

include(../mpi.pri)

SOURCES += main.cpp
