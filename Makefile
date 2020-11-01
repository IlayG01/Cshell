SHELL = /bin/sh

OBJS =  CShell.o RetirectionsHandler.o CmdHandler.o VectorParser.o
CFLAG = -Wall -Wextra -g
CC = gcc
INCLUDE =
LIBS = -lm

cshell:${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f *.o core *.core

.c.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<