CC=gcc
CFLAGS=-g -Wall -Ilpng1632
all: quan pixelate soften
quan: quantify.o gimage.o main.o
	$(CC) $(CFLAGS) -o quantify main.o quantify.o gimage.o -lpng -lz
pixelate: pixelate.o gimage.o 
	$(CC) $(CFLAGS) -o pixelate pixelate.o gimage.o -lpng -lz -lm
soften: soften.o gimage.o matrix.o
	$(CC) $(CFLAGS) -o soften soften.o matrix.o gimage.o -lpng -lz -lm
clean:
	rm -f *.o
