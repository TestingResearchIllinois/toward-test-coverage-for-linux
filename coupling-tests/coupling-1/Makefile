# SPDX-License-Identifier: MIT

CC=gcc
CFLAGS=-Wall

all: myprogram

myprogram: main.o ss1.o ss2.o
	$(CC) -o myprogram main.o ss1.o ss2.o

main.o: main.c ss1.h ss2.h
	$(CC) $(CFLAGS) -c main.c

ss1.o: ss1.c ss1.h
	$(CC) $(CFLAGS) -c ss1.c

ss2.o: ss2.c ss2.h
	$(CC) $(CFLAGS) -c ss2.c

clean:
	rm -f *.o myprogram
