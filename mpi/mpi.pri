
# default mpi compiler
QMAKE_CXX = mpic++
QMAKE_CC = mpicc
QMAKE_LINK = mpic++

# overload mpi library using mpe for tracing
#QMAKE_CC = mpecc
#QMAKE_CXX = mpecc
#QMAKE_LINK = mpecc
#QMAKE_CXXFLAGS += -mpilog
#QMAKE_CFLAGS += -mpilog
#QMAKE_LFLAGS += -mpicc=mpic++ -mpilog

INCLUDEPATH += /usr/lib/openmpi/include

# OpenMPI causes some minor warnings when compiled with c++11:
# /usr/lib/openmpi/include/mpi_portable_platform.h:374: warning:
# invalid suffix on literal; C++11 requires a space between literal
# and string macro [-Wliteral-suffix]
# _STRINGIFY(__GNUC__)"."_STRINGIFY(__GNUC_MINOR__)"."_STRINGIFY(__GNUC_PATCHLEVEL__)
#                      ^

# CONFIG += c++11
