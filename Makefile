CXX=g++
CXXFLAGS=-Wall -Werror -g -O4

all: net Quantifier/quantify pr

Quantifier/quantify:
	make -C Quantifier all

utils.o: utils.cpp utils.h
	$(CXX) $(CXXFLAGS) -c utils.cpp -o utils.o

neuron.o: neuron.cpp neuron.h utils.h
	$(CXX) $(CXXFLAGS) -c neuron.cpp -o neuron.o

layer.o: layer.cpp layer.h utils.h
	$(CXX) $(CXXFLAGS) -c layer.cpp -o layer.o

network.o: network.cpp network.h utils.h
	$(CXX) $(CXXFLAGS) -c network.cpp -o network.o

./Quantifier/quantify.o: ./Quantifier/quantify.c ./Quantifier/quantify.h
	gcc $(CXXFLAGS) -c ./Quantifier/quantify.c -o ./Quantifier/quantify.o

./Quantifier/gimage.o: ./Quantifier/gimage.c ./Quantifier/gimage.h
	gcc $(CXXFLAGS) -c ./Quantifier/gimage.c -o ./Quantifier/gimage.o

net: utils.o network.o layer.o neuron.o ./Quantifier/quantify.o ./Quantifier/gimage.o main.cpp
	$(CXX) $(CXXFLAGS) neuron.o layer.o network.o ./Quantifier/quantify.o ./Quantifier/gimage.o utils.o main.cpp -o net -lpng -lz

pr: utils.o network.o layer.o neuron.o ./Quantifier/quantify.o ./Quantifier/gimage.o main.cpp main2.cpp
	$(CXX) $(CXXFLAGS) neuron.o layer.o network.o ./Quantifier/quantify.o ./Quantifier/gimage.o utils.o main2.cpp -o pr -lpng -lz

clean:
	rm -f *.o net && make -C Quantifier clean
