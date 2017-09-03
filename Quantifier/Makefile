CC=gcc
CFLAGS=-g -Wall -Ilpng1632
all: quan
quan: quantify.o gimage.o main.o
	gcc -o quantify.exe main.o quantify.o gimage.o -lpng -lz
clean:
	rm *.o
