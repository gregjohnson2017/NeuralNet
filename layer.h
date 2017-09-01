#ifndef LAYER_H
#define LAYER_H
#include "neuron.h"
#include <vector>
using namespace std;
class Layer{
	public:
		vector<Neuron*> neurons;
		int nNeurons;
		Layer(int nNeurons, int nInputs);
		~Layer();
};
#endif
