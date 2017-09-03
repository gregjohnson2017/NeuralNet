CC=g++
CFLAGS=-g -Wall -Ilpng1632
CFLAGS=-Wall -Werror -g

all: utils neuron layer network main

utils: utils.cpp utils.h
	$(CC) $(CLFAGS) -c utils.cpp -o utils.o

neuron: neuron.cpp neuron.h utils.h
	$(CC) $(CLFAGS) -c neuron.cpp -o neuron.o
	
layer: layer.cpp layer.h utils.h
	$(CC) $(CLFAGS) -c layer.cpp -o layer.o
	
network: network.cpp network.h utils.h
	$(CC) $(CLFAGS) -c network.cpp -o network.o
	
#quantify: ./Quantify/quantify.c ./Quantify/quantify.h
#	gcc $(CFLAGS) -c ./Quantify/quantify.c -o ./Quantify/quantify.o
	
main: utils.o network.o layer.o neuron.o ./Quantifier/quantify.o ./Quantifier/gimage.o
	$(MAKE) -C ./Quantifier
	$(CC) $(CLFAGS) neuron.o layer.o network.o ./Quantifier/quantify.o ./Quantifier/gimage.o utils.o main.cpp -o net -lpng -lz

clean:
	rm *.o net && cd ./Quantify && make clean
