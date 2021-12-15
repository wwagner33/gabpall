#
# Makefile for the Gamma Compiler
#

ALL: default

##### User configurable options #####

ARCH        = /usr/bin
COMM        = 
BOPT        = 
P4_DIR      = 
TOOLS_DIR   = 
MPIR_HOME   = ~/ProjetosGit/Gamma/Gamma_Par #Aqui Rui vc tem que alterar o caminho
CC          = /usr/bin/mpicc
CLINKER     = /usr/bin/mpicc
CCC         = 
CCLINKER    = $(CCC)
F77         = true
FLINKER     = true
AR          = ar crl
RANLIB      = ranlib
PROFILING   = $(PMPILIB)
OPTFLAGS    = -g
LIB_PATH    = -L /usr/lib #~/gabriel/MPI/lib 
LIB_LIST    = -lmpi /usr/lib/libmpi.so
#
INCLUDE_DIR =  -I ../include #$(MPIR_HOME)/include 
DEVICE      = ch_p4

### End User configurable options ###

CFLAGS	  =  -DMPID_NO_FORTRAN  -DHAS_XDR=1  -DHAVE_STDLIB_H=1 -DHAVE_SYSTEM=1 -DHAVE_NICE=1 $(OPTFLAGS) $(INCLUDE_DIR) -DMPI_$(ARCH) 
CCFLAGS	  = $(CFLAGS)
#FFLAGS	  = '-qdpc=e' 
FFLAGS	  =  $(OPTFLAGS)
LIBS	  = $(LIB_PATH) $(LIB_LIST)
LIBSPP	  = $(MPIPPLIB) $(LIBS)
EXECS	  = run
MPI = mpicc

SRC	=  run.c bagcomm.c bag_cell.c controller_cell.c rtbag.c copy.c runtimevars.c
OBJ	=  run.o bagcomm.o bag_cell.o controller_cell.o rtbag.o copy.o ../misc.o runtimevars.o

default: $(EXECS)

all: default

run: $(OBJ) 
	$(CLINKER) $(OPTFLAGS) -o run $(OBJ) \
	$(LIB_PATH) $(LIB_LIST) -lm 

../misc.o	: ../include/const.h ../include/types.h
	$(CC) $(CFLAGS) -c ../misc.c

rtbag.o	: ../include/types.h ../include/rtbag.h ../include/fnc.h
	$(CC) $(CFLAGS) -c rtbag.c

vars.o	: ../include/const.h ../include/types.h 
	$(CC) $(CFLAGS) -c vars.c

runtimevars.o	: ../include/rtbag.h
	$(CC) $(CFLAGS) -c runtimevars.c

bag_cell.o	: ../include/const.h ../include/types.h ../include/vars.h ../include/msg.h
	$(CC) $(CFLAGS) -c bag_cell.c

controller_cell.o	: ../include/const.h ../include/types.h ../include/vars.h ../include/msg.h
	$(CC) $(CFLAGS) -c controller_cell.c

bagcomm.o	: ../include/const.h ../include/types.h ../include/vars.h ../include/msg.h ../include/rtbag.h ../include/fnc.h
	$(CC) $(CFLAGS) -c bagcomm.c

run.o	: ../include/const.h ../include/types.h ../include/rtbag.h 
	$(CC) $(CFLAGS) -c run.c

copy.o	: ../include/const.h ../include/types.h ../include/rtbag.h ../include/vars.h
	$(CC) $(CFLAGS) -c copy.c




