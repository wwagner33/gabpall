#
# Makefile for the Gamma Compiler
#
YFLAGS	= -d

CFLAGS	= -g

LFLAGS	=

INC     = include/const.h include/types.h include/fnc.h include/rtbag.h

SRC	= grammar.y scan.l main.c parser.c misc.c print.c check.c vars.c code.c printcode.c pattern.c 

OBJ	= grammar.o scan.o main.o parser.o misc.o print.o check.o vars.o code.o printcode.o pattern.o 

gm	:	$(OBJ)
	mpicc $(CFLAGS) $(OBJ) -o gm

scan.o	: y.tab.h

parser.o : $(INC)

misc.c	: $(INC)

grammar.y : $(INC) include/vars.h

print.o : $(INC)

check.o : $(INC)

copy.o  : $(INC)

vars.o : $(INC) 

pattern.o : $(INC)

main.o : $(INC) include/vars.h

code.c : $(INC)

printcode.o: $(INC)

clean	:
	rm -f y.output *.o
