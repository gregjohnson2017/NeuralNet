#include "network.h"
#include "utils.h"
#include <vector>

using namespace std;

/*
Network constructor: Initialized to have a given number of layers, with a specified
input and output layer size.
*/
Network::Network(int nLayers, int nInputs, int nOutputs){
	this->nLayers = nLayers;
	this->nInputs = nInputs;
	this->nOutputs = nOutputs;
	for(int i = 0; i < nLayers; i++){
		int neuronsPerLayer; 
		if(i == 0) neuronsPerLayer = nInputs; // input layer
		else if(i == nLayers - 1) neuronsPerLayer = nOutputs; // output layer
		else neuronsPerLayer = (nInputs + nOutputs) / 2; // hidden layer (default size)
		// if the layer is not the input layer, then the number of neurons in the layer
		// will depend on the number of inputs received from the previous layer
		// (i.e. number of neurons in the prevous layer)
		layers.push_back(new Layer(neuronsPerLayer, i == 0 ? nInputs : layers[i - 1]->nNeurons));
	}
}

/*
Network destructor
*/
Network::~Network(){
	layers.clear();
}

/*
Completes one pass through the network, returning the outputs of the neurons
in the final layer.
*/
vector<double> Network::getOutputs(vector<double> &inputs){
	vector<double> results = inputs;
	for(int i = 0; i < nLayers; i++){
		results = layers[i]->feedLayer(results);
	}
	return results;
}
	
/*
This will train all the neurons in the network by calculating their error
from a known answer, and modifying their weights respetively.
In every layer, each neuron gets all the inputs from the previous layer.
*/	
void Network::train(vector<double> &guesses, vector<double> &answers){
	if(guesses.size() != answers.size()) throw invalid_argument("mismatched guesses and answers");
	vector<double> error = subtract(answers, guesses);
	for(int i = 0; i < (int)layers.size(); i++){
		for(int j = 0; j < (int)layers[i]->neurons.size(); j++){
			for(int k = 0; k < (int)layers[i]->neurons[j]->weights.size(); k++){
				layers[i]->neurons[j]->weights[k] += trainingConstant() * 0 * layers[i]->outputs[k];
				// replace 0 with error, or some sort of back propagation approach
			}
		}
	}
}

void backPropagate(vector<double> &weights, vector<double> &error, vector<double> &z){
	return;
}
