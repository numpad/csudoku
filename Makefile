CC=gcc
CSTD=c99
CWARN=-Wall -pedantic -ggdb
CFLAGS=-Ilib/ -Isrc/
CCOMP=lib/lodepng.c

all:
	${CC} -std=${CSTD} ${CWARN} ${CFLAGS} src/solver.c ${CCOMP} -osolver

