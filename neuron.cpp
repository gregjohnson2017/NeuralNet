#include <stdlib.h>
#include "utils.h"

/*
Using lessons from http://natureofcode.com/book/chapter-10-neural-networks/
*/

class Neuron{
	public:
		double *weights, bias;
		int nWeights;
		const double trainingConstant = 1;
		
		Neuron(int nWeights){
			this->nWeights = nWeights;
			weights = malloc(sizeof(double) * nWeights);
			bias = rand() * 2 - 1; // -1 to 1 non-inclusive
			for(int i = 0; i < nWeights; i++){
				weights[i] = rand() * 2 - 1; // -1 to 1 non-inclusive
			}
		}
		
		~Neuron(){
			free(weights);
		}
		
		int feedforward(double *inputs, nInputs){
			if(nInputs != nWeights) throw std::invalid_argument("mismatched inputs and weights");
			double sum = 0;
			for(int i = 0; i < nWeights; i++){
				sum += inputs[i] * weights[i] - bias;
			}
			return sigmoid(sum);
		}
		
		void train(double inputs[], int nInputs, int answer){
			int guess = feedforward(inputs, nInputs);
			double error = answer - guess;
			for(int i = 0; i < nWeights; i++){
				weights[i] += trainingConstant * error * inputs[i];
			}	
		}
}
