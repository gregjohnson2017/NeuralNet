#include <stdlib.h>
#include "neuron.h"
#include "utils.h"

/*
Authors/Contributors: Greg Johnson, Trevor Kropp, Tyler Kropp
Date: 8/31/17
Acknowledgements: http://natureofcode.com/book/chapter-10-neural-networks/
*/

/*
Random Neuron constructor: assigns random weights (-1 to 1 non-inclusive),
that should correspond the number of inputs from the previous layer.
Each neuron also gets a random bias (-1 to 1 non-inclusive).
*/
Neuron::Neuron(int nWeights){
	bias = rand() * 2 - 1; // -1 to 1 non-inclusive
	for(int i = 0; i < nWeights; i++){
		weights.push_back(rand() * 2 - 1); // -1 to 1 non-inclusive
	}
	error = 0;
	z = 0;
	a = 0;
}

/*
Neuron constructor for pre-defined weights and bias.
Used for loading networks
*/
Neuron::Neuron(vector<double> &weights, double bias){
	this->weights = weights;
	this->bias = bias;
}

/*
Neuron destructor method.
*/	
Neuron::~Neuron(){
	weights.clear();
}
/*
Assigns the neuron's activation value
Determines the neuron's output (-1 to 1 non-inclusive).
Sums the products of all inputs and their respective weights plus bias.
*/
void Neuron::feed(vector<double> &inputs){
	if(inputs.size() != weights.size()) throw std::invalid_argument("mismatched inputs and weights");
	double sum = 0;
	for(int i = 0; i < (int)weights.size(); i++){
		sum += inputs[i] * weights[i];
	}
	z = sum + bias;
	a = sigmoid(sum + bias);
}



