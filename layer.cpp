#include "neuron.h"
#include "layer.h"

/*
Layer constructor, builds a layer of nNeurons neurons.
Each neuron will be initialized with nInputs weights.
*/
Layer::Layer(int nNeurons, int nInputs){
	this->nNeurons = nNeurons;
	this->nInputs = nInputs;
	for(int i = 0; i < (int)neurons.size(); i++){
		neurons.push_back(new Neuron(nInputs));
	}
}

/*
Adds each neuron's output to the layer's result vector, and returns this result vector.
*/
vector<double> Layer::feedLayer(vector<double> &inputs){
	for(int i = 0; i < (int)neurons.size(); i++){
		outputs.push_back(neurons[i]->feed(inputs));
	}
	return outputs;
}

/*
Layer destructor
*/
Layer::~Layer(){
	neurons.clear();
}

