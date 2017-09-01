#ifndef NETWORK_H
#define NETWORK_H
#include "layer.h"
#include <vector>
using namespace std;
class Network{
	public:
		static double trainingConstant(){
			return 1;
		}
		int nLayers, nInputs, nOutputs;
		vector<Layer*> layers;
		vector<double> getOutputs(vector<double> inputs);
		void train(vector<double> inputs, vector<double> answers);
		Network(int nLayers, int nInputs, int nOutputs);
		~Network();
};
#endif

