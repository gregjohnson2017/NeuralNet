#ifndef NETWORK_H
#define NETWORK_H
#include "layer.h"
#include <vector>
using namespace std;
class Network{
	public:
		int nLayers, nInput, nOutput;
		vector<Layer*> layers;
		Network::vector<double> getOutputs(vector<double> inputs){
		void train(vector<double> inputs, vector<double> answers);
		Network(int nLayers, int nInput, int nOutput);
		~Network();
};
#endif

