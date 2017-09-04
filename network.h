#ifndef NETWORK_H
#define NETWORK_H
#include <stdio.h>
#include <vector>
#include "layer.h"
using namespace std;

typedef struct samples{
	vector<vector<double> > *inputData;
	vector<double> *answers;
	int sampleSize; // length of 1d array stretch before next sample
	// ex 64x64 image: sampleSize = 4096
} samples;

class Network{
	public:
		int nLayers, nInputs, nOutputs;
		Network(int nLayers, int nInputs, int nOutputs);
		Network(vector<Layer> &layers, int nInputs, int nOutputs);
		Network(char *fileName);
		~Network();
		static double trainingConstant(){
			return 1;
		}
		vector<Layer> layers;
		vector<double> getOutputs();
		void train(samples *s);
		void feedNetwork(vector<double> &inputs);
		void backPropagate();
		void computeOutputError(double answer);
		void saveNetwork(char *fileName);
};
#endif

