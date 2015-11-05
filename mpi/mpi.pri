
# default mpi compiler
#QMAKE_CXX = mpic++
#QMAKE_CC = mpicc
#QMAKE_LINK = mpic++

# overload mpi library using mpe for tracing
QMAKE_CC = mpecc
QMAKE_CXX = mpecc
QMAKE_LINK = mpecc
QMAKE_CXXFLAGS += -mpilog
QMAKE_CFLAGS += -mpilog
QMAKE_LFLAGS += -mpicc=mpic++ -mpilog

INCLUDEPATH += /usr/lib/openmpi/include

#CONFIG += c++11
