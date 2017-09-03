#ifndef NETWORK_H
#define NETWORK_H
#include <stdio.h>
#include <vector>
#include "layer.h"
using namespace std;

typedef struct samples{
	vector<double> inputData, answers;
	int sampleSize; // length of 1d array stretch before next sample
	// ex 64x64 image: sampleSize = 4096
} samples;

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
		void train(samples *s);
		void saveNetwork(char *fileName);
		Network* loadNetwork(char *fileName);
		void backPropagate();
};
#endif

