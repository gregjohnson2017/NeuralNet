#ifndef LAYER_H
#define LAYER_H
#include "neuron.h"
#include <vector>
using namespace std;
class Layer{
	public:
		vector<Neuron> neurons;
		int nNeurons;
		vector<double> feedLayer(vector<double> &inputs), outputs;
		Layer(int nNeurons, int nInputs);
		Layer(vector<Neuron> &neurons);
		~Layer();
};
#endif
