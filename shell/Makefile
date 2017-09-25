all: shell

CFLAGS = -g -O2

shell: shell.o mytoken.o
	cc -o shell shell.o mytoken.o

mytoc.o: mytoken.c
	cc -c mytoken.c

shell.o: shell.c mytoken.h
	cc -c shell.c
clean:
	rm -f  *.o mytoken mytoken.exe shell shell.exe
