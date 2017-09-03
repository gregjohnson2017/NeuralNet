#include "neuron.h"
#include "layer.h"

/*
Random Layer constructor, builds a layer of nNeurons neurons.
Each neuron will be initialized randomly with nInputs weights and a bias.
*/
Layer::Layer(int nNeurons, int nInputs){
	this->nNeurons = nNeurons;
	for(int i = 0; i < (int)neurons.size(); i++){
		neurons.push_back(Neuron(nInputs));
	}
}

/*
Layer constructor for pre-defined layers. Used for loading networks.
*/
Layer::Layer(vector<Neuron> &neurons){
	this->nNeurons = neurons.size();
	this->neurons = neurons;
}

/*
Adds each neuron's output to the layer's result vector, and returns this result vector.
*/
vector<double> Layer::feedLayer(vector<double> &inputs){
	for(int i = 0; i < (int)neurons.size(); i++){
		outputs.push_back(neurons[i].feed(inputs));
	}
	return outputs;
}

/*
Layer destructor
*/
Layer::~Layer(){
	neurons.clear();
}

