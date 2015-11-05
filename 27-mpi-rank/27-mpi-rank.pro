#-------------------------------------------------
#
# Project created by QtCreator 2015-11-04T22:47:43
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = 27-mpi-rank
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

#QMAKE_CXX = mpic++
#QMAKE_CC = mpicc
#QMAKE_LINK = mpic++

QMAKE_CC = mpecc
QMAKE_CXX = mpecc
QMAKE_LINK = mpecc
QMAKE_CXXFLAGS += -mpilog
QMAKE_CFLAGS += -mpilog
QMAKE_LFLAGS += -mpicc=mpic++ -mpilog

SOURCES += main.cpp
