CC=gcc
CFLAGS=-g -Wall -Ilpng1632
all: quan
quan: quantify.o gimage.o main.o mnist.o
	$(CC) $(CFLAGS) -o quantify main.o mnist.o quantify.o gimage.o -lpng -lz
clean:
	rm -f *.o
