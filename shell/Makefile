all: test

CFLAGS = -g -O2

test: test.o mytoken.o
	cc -o test test.o mytoken.o

mytoc.o: mytoken.c
	cc -c mytoken.c

test.o: test.c mytoken.h
	cc -c test.c
clean:
	rm -f  *.o mytoken mytoken.exe test test.exe
