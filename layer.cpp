#include "neuron.h"
#include "layer.h"

/*
Layer constructor, builds a layer of nNeurons neurons.
Each neuron will be initialized with nInputs weights.
*/
Layer::Layer(int nNeurons, int nInputs){
	this->nNeurons = nNeurons;
	for(int i = 0; i < neurons.size(); i++){
		neurons.push_back(new Neuron(nInputs));
	}
}

Layer::~Layer(){
	neurons.clear();
}

