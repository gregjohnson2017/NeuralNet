#ifndef NETWORK_H
#define NETWORK_H
#include "layer.h"
#include <vector>
using namespace std;
class Network{
	public:
		int nLayers, nInputs, nOutputs;
		Network(int nLayers, int nInputs, int nOutputs);
		Network(vector<Layer> &layers, int nInputs, int nOutputs);
		~Network();
		static double trainingConstant(){
			return 1;
		}
		vector<Layer> layers;
		vector<double> getOutputs(vector<double> &inputs);
		void train(vector<double> &inputs, vector<double> &answers);
		void backPropagate(vector<double> &weights, vector<double> &error, vector<double> &z);
		void saveNetwork(char *fileName);
		Network* loadNetwork(char *fileName);
};
#endif

