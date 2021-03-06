#include "neuron.h"
#include "layer.h"
#include <stdio.h>
/*
  Random Layer constructor, builds a layer of nNeurons neurons.
  Each neuron will be initialized randomly with nInputs weights and a bias.
*/
Layer::Layer(int nNeurons, int nInputs, bool input){
  this->nNeurons = nNeurons;
  for(int i = 0; i < nNeurons; i++){
    neurons.push_back(new Neuron(nInputs, input));
  }
}

/*
  Layer constructor for pre-defined layers. Used for loading networks.
*/
Layer::Layer(vector<Neuron*> *neurons){
  this->nNeurons = neurons->size();
  this->neurons = *neurons;
}

/*
  Adds each neuron's output to the layer's result vector, and returns this result vector.
*/
/*void Layer::feedLayer(vector<double> &inputs){
  for(int i = 0; i < (int)neurons.size(); i++){
    neurons[i]->feed(inputs);
  }
}*/

vector<double> Layer::getOutputs(){
  vector<double> outputs;
  for(int i = 0; i < nNeurons; i++){
    outputs.push_back(neurons[i]->a);
  }
  return outputs;
}

Layer::~Layer(){
  neurons.clear();
}

