#include <stdlib.h>
#include "neuron.h"
#include "utils.h"

/*
Using lessons from http://natureofcode.com/book/chapter-10-neural-networks/
*/

Neuron::Neuron(int nWeights) : trainingConstant(1){
	this->nWeights = nWeights;
	weights = new double[nWeights];
	bias = rand() * 2 - 1; // -1 to 1 non-inclusive
	for(int i = 0; i < nWeights; i++){
		weights[i] = rand() * 2 - 1; // -1 to 1 non-inclusive
	}
}

Neuron::~Neuron(){
	if (weights)
		delete[] weights;
}

double Neuron::feed(double inputs[], int nInputs){
	if(nInputs != nWeights) throw std::invalid_argument("mismatched inputs and weights");
	double sum = 0;
	for(int i = 0; i < nWeights; i++){
		sum += inputs[i] * weights[i] - bias;
	}
	return sigmoid(sum);
}

void Neuron::train(double inputs[], int nInputs, int answer){
	int guess = feed(inputs, nInputs);
	double error = answer - guess;
	for(int i = 0; i < nWeights; i++){
		weights[i] += trainingConstant * error * inputs[i];
	}	
}
