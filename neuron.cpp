#include <stdlib.h>
#include "neuron.h"
#include <stdio.h>
#include "utils.h"

/*
  Authors/Contributors: Greg Johnson, Trevor Kropp, Tyler Kropp
  Date: 8/31/17
  Acknowledgements: http://natureofcode.com/book/chapter-10-neural-networks/
*/

/*
  Random Neuron constructor: assigns random weights (-1 to 1 non-inclusive),
  that should correspond the number of inputs from the previous layer.
  Each neuron also gets a random bias (-1 to 1 non-inclusive).
*/
Neuron::Neuron(int nWeights, bool in){
  input = in;
  bias = rand()/(double)RAND_MAX * 2 - 1; // -1 to 1 non-inclusive
  bias *= 1.0 / sqrt(nWeights);
  for(int i = 0; i < nWeights; i++){
    double w = rand()/(double)RAND_MAX * 2 - 1; // -1 to 1 non-inclusive
    w *= 1.0 / sqrt(nWeights);
    weights.push_back(w);
  }
  error = 0;
  z = 0;
  a = 0;
}

/*
  Neuron constructor for pre-defined weights and bias.
  Used for loading networks
*/
Neuron::Neuron(vector<double> &weights, double bias){
  this->weights = weights;
  this->bias = bias;
  error = 0;
  z = 0;
  a = 0;
}

/*
  Neuron destructor method.
*/	
Neuron::~Neuron(){
  printf("neuron destroyed\n");
  weights.clear();
}
/*
  Assigns the neuron's activation value
  Determines the neuron's output (-1 to 1 non-inclusive).
  Sums the products of all inputs and their respective weights plus bias.
*/
void Neuron::feed(vector<double> &inputs){
  //printf("feed called on neuron\n");
  if(inputs.size() != weights.size()){
    fprintf(stderr, "Mismatched inputs and weights expected %lu inputs but got %lu!\n", weights.size(), inputs.size());
    throw std::invalid_argument("mismatched inputs and weights");
  }
  if(input){
    z = inputs[inPos];
    a = z;//sigmoid(z);
    return;
  }
  double sum = 0;
  for(int i = 0; i < (int)weights.size(); i++){
    sum += inputs[i] * weights[i];
    //printf("sum=%e, inputs[%d]=%e, weights[%d]=%e\n", sum, i, inputs[i], i, weights[i]);
  }
  z = sum + bias;
  a = sigmoid(sum + bias);
}
