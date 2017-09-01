#include "neuron.h"

class Layer{
	public:
		Neuron neurons[];
		int nNeurons;
		Layer(int nNeurons, int nInputs){
			this->nNeurons = nNeurons;
			neurons = new Neuron[nNeurons];
			for(int i = 0; i < nNeurons; i++){
				neurons[i] = new Neuron(nInputs);
			}
		}
}
