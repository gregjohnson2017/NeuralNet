#include "utils.h"
#include "neuron.h"
#include "layer.h"
#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

extern "C" {
    #include "./Quantifier/quantify.h"
}

samples* getSamples(char* fileName);

using namespace std;

int main(int argc, char **argv){
	srand(time(NULL));
	Network *n = new Network(5, 4096, 4);
	char *dataFile = (char*)malloc(sizeof(char) * 22);
	memcpy(dataFile, "./Quantifier/nums.dat\0", 22);
	samples *s = getSamples(dataFile);
	n->train(s);
	char *networkFile = (char*)malloc(sizeof(char) * 11);
	memcpy(networkFile, "network.nn\0", 11);
	n->saveNetwork(networkFile);
	free(dataFile);
	free(networkFile);
	return 1;
}

samples* getSamples(char *fileName){
	data_collection *d = read_data(fileName);
	samples *s = (samples*)malloc(sizeof(samples));
	vector<double> data, answers;
	for(int i = 0; i < d->num_arrays; i++){
		for(int j = 0; j < d->size; j++){
			for(int k = 0; k < d->size; k++){
				data.push_back((double)d->data[i][j][k]);
			}
		}
		answers.push_back((double)d->answers[i]);
	}
	s->inputData = data;
	s->sampleSize = d->size * d->size;
	s->answers = answers;
	return s;
}
