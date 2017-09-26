#ifndef NETWORK_H
#define NETWORK_H
#include <stdio.h>
#include <vector>
#include "layer.h"
using namespace std;

typedef double (*errorFunc)(double activation, void* answer);

typedef struct sampleSet{
	vector<vector<double> > *inputData;
	
	vector<double> *answers;
	int sampleSize; // length of 1d array stretch before next sample
	// ex 64x64 image: sampleSize = 4096
} sampleSet;

sampleSet* getSamples(const char *fileName);
void printSample(vector<double> sample);

class Network{
	public:
		int nLayers, nInputs, nOutputs;
		
		errorFunc getError;
		
		Network(int nLayers, int nInputs, int nOutputs, errorFunc err);
		//Network(vector<Layer*> &layers, int nInputs, int nOutputs);
		Network(const char *fileName, errorFunc err);
		~Network();
		static double trainingConstant(){
			return 0.3;
		}
		static int batchSize(){
			return 25;
		}
		vector<Layer*> layers;
		vector<double> getOutputs();
		void test(const char *testingData);
		void train(const char *fileName, double trainingConstant);
		void feedForward(vector<double> &inputs);
		void backPropagate();
		void printNetwork();
		void computeOutputError(double answer);
		void save(const char *fileName);
};
#endif

