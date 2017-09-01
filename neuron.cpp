#include <stdlib.h>
#include "utils.h"

/*
Authors/Contributors: Greg Johnson, Trevor Kropp, Tyler Kropp
Date: 8/31/17
Acknowledgements: http://natureofcode.com/book/chapter-10-neural-networks/
*/

class Neuron{
	public:
		double weights[], bias;
		int nWeights;
		const double trainingConstant; // Affects speed/accuracy of training.
		
		/*
		Neuron constructor: assigns random weights (-1 to 1 non-inclusive),
		that should correspond the number of inputs from the previous layer.
		Each neuron also gets a random bias (-1 to 1 non-inclusive).
		*/
		Neuron(int nWeights) : trainingConstant(1){
			this->nWeights = nWeights;
			weights = new double[nWeights];
			bias = rand() * 2 - 1; // -1 to 1 non-inclusive
			for(int i = 0; i < nWeights; i++){
				weights[i] = rand() * 2 - 1; // -1 to 1 non-inclusive
			}
		}
		
		/*
		Neuron destructor method.
		*/
		~Neuron(){
			if (weights)
				delete[] weights;
		}
		
		/*
		Determines the neuron's output (-1 to 1 non-inclusive).
		Sums the products of all inputs and their respective weights minus bias.
		Returns the sigmoid function of this sum.
		*/
		double feed(double inputs[], int nInputs){
			if(nInputs != nWeights) throw std::invalid_argument("mismatched inputs and weights");
			double sum = 0;
			for(int i = 0; i < nWeights; i++){
				sum += inputs[i] * weights[i] - bias;
			}
			return sigmoid(sum);
		}
		
		// should be moved to a different class
		void train(double inputs[], int nInputs, int answer){
			int guess = feed(inputs, nInputs);
			double error = answer - guess;
			for(int i = 0; i < nWeights; i++){
				weights[i] += trainingConstant * error * inputs[i];
			}	
		}
};
