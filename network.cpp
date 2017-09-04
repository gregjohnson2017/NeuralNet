#include "network.h"
#include "utils.h"
#include <vector>

using namespace std;

/*
Network constructor: Initialized to have a given number of  random layers, 
with a specified input and output layer size.
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
		layers.push_back(Layer(neuronsPerLayer, i == 0 ? nInputs : layers[i - 1].nNeurons));
	}
}

/*
Network constructor used for loading pre-defined networks
*/
Network::Network(vector<Layer> &layers, int nInputs, int nOutputs){
	this->nLayers = layers.size();
	this->nInputs = nInputs;
	this->nOutputs = nOutputs;
	this->layers = layers;
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
void Network::feedNetwork(vector<double> &inputs){
	for(int i = 0; i < nLayers; i++){
		vector<double> layerInputs = i == 0 ? inputs : layers[i - 1].getOutputs();
		for(int j = 0; j < (int)layers[i].neurons.size(); j++){
			layers[i].neurons[j].feed(inputs);
		}
		layerInputs.clear();
	}
}

vector<double> Network::getOutputs(){
	vector<double> outputs;
	for(int i = 0; i < layers[layers.size() - 1].nNeurons; i++){
		outputs.push_back(layers[layers.size() - 1].neurons[i].a);
	}
	return outputs;
}

/*
FIX ME !!!!!!!!!!! cause I am borked
*/	
void Network::train(samples *s){
	// feed network
	// compute output errors
	// back propagate
	// gradient decent (to modify biases and weights)
}

void Network::computeOutputError(double answer){
	for(int i = 0; i < layers[layers.size() - 1].nNeurons; i++){
		double y = i - 1 == answer ? 1 : 0;
		Neuron n = layers[layers.size() - 1].neurons[i];
		n.error = (n.a - y) * sigmoid_prime(n.z);
	}
}

/*
  Calculates and updates the errors of the neurons
 */
void Network::backPropagate(){
	// loop through layers l = L-1, L-2, ..., 2
	for(int l = nLayers - 2; l >= 1; l--){ // L-1 <=> nLayers - 2; 2 <=> 1
		// loop through neurons j in layer l
		for(int j = 0; j < layers[l].nNeurons; j++){
			double sum = 0;
			// loop through neurons k in layer l+1
			for(int k = 0; k < layers[l + 1].nNeurons; k++){
				double w = layers[l + 1].neurons[k].weights[j];
				double e = layers[l + 1].neurons[k].error;
				double z = layers[l].neurons[j].z;
				sum += w * e * sigmoid_prime(z);
				// set error for the neuron to sum
				layers[l].neurons[j].error = sum;
			}
		}
	}
}

/*
Network FILE FORMAT:
nInputs (int)
nOutputs (int)
nLayers (int)
	nNeurons (int) LAYER #0-n
		nWeights (int) NEURON #0-n
			weight (double) WEIGHT #0-n
			...
			bias (double)
		...
	...
EOF
*/
void Network::saveNetwork(char *fileName){
	FILE *fp = fopen(fileName, "wb");
	if(!fp){
		printf("Error opening %s\n", fileName);
		throw invalid_argument("file IO error");
	}
	fwrite(&nInputs, sizeof(nInputs), 1, fp);
	fwrite(&nOutputs, sizeof(nOutputs), 1, fp);
	fwrite(&nLayers, sizeof(nLayers), 1, fp);
	for(int i = 0; i < nLayers; i++){
		fwrite(&layers[i].nNeurons, sizeof(layers[i].nNeurons), 1, fp);
		for(int j = 0; j < layers[i].nNeurons; j++){
			int nWeights = layers[i].neurons[j].weights.size();
			fwrite(&nWeights, sizeof(nWeights), 1, fp);
			for(int k = 0; k < (int)layers[i].neurons[j].weights.size(); k++){
				fwrite(&layers[i].neurons[j].weights[k], sizeof(layers[i].neurons[j].weights[k]), 1, fp);
			}
			fwrite(&layers[i].neurons[j].bias, sizeof(layers[i].neurons[j].bias), 1, fp);
		}
	}
	fclose(fp);
}

/*
 Loads a network from specified file. (See saveNetwork for file format)
 */
Network::Network(char *fileName){
	FILE *fp = fopen(fileName, "rb");
	if(!fp){
		printf("Error opening %s\n", fileName);
		throw invalid_argument("file IO error");
	}
	size_t gcc = 0;
	int nInputs, nOutputs, nLayers;
	gcc += fread(&nInputs, sizeof(int), 1, fp) * sizeof(int);
	gcc += fread(&nOutputs, sizeof(int), 1, fp) * sizeof(int);
	gcc += fread(&nLayers, sizeof(int), 1, fp) * sizeof(int);
	//vector<Layer> layers;
	for(int i = 0; i < nLayers; i++){
		vector<Neuron> neurons;
		int nNeurons;
		gcc += fread(&nNeurons, sizeof(int), 1, fp) * sizeof(int);
		for(int j = 0; j < nNeurons; j++){
			vector<double> weights;
			int nWeights;
			gcc += fread(&nWeights, sizeof(int), 1, fp) * sizeof(int);
			for(int k = 0; k < nWeights; k++){
				double weight;
				gcc += fread(&weight, sizeof(double), 1, fp) * sizeof(double);
				weights.push_back(weight);
			}
			double bias;
			gcc += fread(&bias, sizeof(double), 1, fp) * sizeof(double);
			neurons.push_back(Neuron(weights, bias));
		}
		layers.push_back(Layer(neurons));
	}
	printf("Loaded neural network from file %s with %d inputs %d outputs %d layers (read %lu bytes).\n", fileName, nInputs, nOutputs, nLayers, gcc);
}
