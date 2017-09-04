CC=g++
CFLAGS=-Wall -Werror -g

all: net

utils.o: utils.cpp utils.h
	$(CC) $(CFLAGS) -c utils.cpp -o utils.o

neuron.o: neuron.cpp neuron.h utils.h
	$(CC) $(CFLAGS) -c neuron.cpp -o neuron.o
	
layer.o: layer.cpp layer.h utils.h
	$(CC) $(CFLAGS) -c layer.cpp -o layer.o
	
network.o: network.cpp network.h utils.h
	$(CC) $(CFLAGS) -c network.cpp -o network.o

./Quantifier/quantify.o: ./Quantifier/quantify.c ./Quantifier/quantify.h
	gcc $(CFLAGS) -c ./Quantifier/quantify.c -o ./Quantifier/quantify.o

./Quantifier/gimage.o: ./Quantifier/gimage.c ./Quantifier/gimage.h
	gcc $(CFLAGS) -c ./Quantifier/gimage.c -o ./Quantifier/gimage.o
	
net: utils.o network.o layer.o neuron.o ./Quantifier/quantify.o ./Quantifier/gimage.o main.cpp
	$(CC) $(CFLAGS) neuron.o layer.o network.o ./Quantifier/quantify.o ./Quantifier/gimage.o utils.o main.cpp -o net -lpng -lz

clean:
	rm -f *.o net && cd ./Quantifier && make clean
