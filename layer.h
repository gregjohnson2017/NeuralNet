#ifndef LAYER_H
#define LAYER_H
#include "neuron.h"
#include <vector>
using namespace std;
class Layer{
	public:
		vector<Neuron*> neurons;
		int nNeurons;
		void feedLayer(vector<double> &inputs);
		vector<double> getOutputs();
		vector<double> outputs;
		Layer(int nNeurons, int nInputs, bool n);
		Layer(vector<Neuron*> *neurons);
		~Layer();
};
#endif
