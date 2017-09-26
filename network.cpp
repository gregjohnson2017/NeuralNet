#include "network.h"
#include "utils.h"
#include <vector>

extern "C" {
#include "./Quantifier/quantify.h"
}

using namespace std;

/*
  Network constructor: Initialized to have a given number of  random layers, 
  with a specified input and output layer size.
*/
Network::Network(int nLayers, int nInputs, int nOutputs, errorFunc err){
  this->getError = err;
  this->nLayers = nLayers;
  this->nInputs = nInputs;
  this->nOutputs = nOutputs;
  for(int i = 0; i < nLayers; i++){
    int neuronsPerLayer; 
    if(i == 0) neuronsPerLayer = nInputs; // input layer
    else if(i == nLayers - 1) neuronsPerLayer = nOutputs; // output layer
    else neuronsPerLayer = (nInputs + nOutputs) / (2); // hidden layer (default size)
    
    // if the layer is not the input layer, then the number of neurons in the layer
    // will depend on the number of inputs received from the previous layer
    // (i.e. number of neurons in the prevous layer)
    layers.push_back(new Layer(neuronsPerLayer, i == 0 ? nInputs : layers[i - 1]->nNeurons, i==0));
  }
  for(int n = 0; n < layers[0]->nNeurons; n++){
    layers[0]->neurons[n]->bias = 0;
    for(int w = 0; w < (int)layers[0]->neurons[n]->weights.size(); w++){
      layers[0]->neurons[n]->weights[w] = 0;
    }
    layers[0]->neurons[n]->inPos = n;
    layers[0]->neurons[n]->weights[n] = 1;
  }
}

/*
  Network constructor used for loading pre-defined networks
*/
/*Network::Network(vector<Layer*> &layers, int nInputs, int nOutputs){
  this->nLayers = layers.size();
  this->nInputs = nInputs;
  this->nOutputs = nOutputs;
  this->layers = layers;
  for(int n = 0; n < layers[0]->nNeurons; n++){
    layers[0]->neurons[n]->inPos = n;
  }
}*/

Network::~Network(){
  layers.clear();
}

void Network::printNetwork(){
  printf("nLayers = %d, nInputs = %d, nOutputs = %d\n", nLayers, nInputs, nOutputs);
}

/*
  Completes one pass through the network, returning the outputs of the neurons
  in the final layer.
*/
void Network::feedForward(vector<double> &inputs){
  if(inputs.size() != layers[0]->neurons.size()){
    printf("inputs size = %d and input layer size = %d\n", (int)inputs.size(), (int)layers[0]->neurons.size());
    throw invalid_argument("inputs != input layer");
  }
  for(int i = 0; i < nLayers; i++){
    vector<double> layerInputs;
    if(i == 0){
        layerInputs = inputs;
    }else{
        layerInputs =  layers[i - 1]->getOutputs();
    }
    for(int j = 0; j < (int)layers[i]->neurons.size(); j++){
      layers[i]->neurons[j]->feed(layerInputs);
    }
//    printf("Layer %d average error = %e\n", i, totalError / (double)layers[i].neurons.size());
    layerInputs.clear();
  }
}

vector<double> Network::getOutputs(){
  vector<double> outputs;
  for(int i = 0; i < (int)layers[layers.size() - 1]->neurons.size(); i++){
    //printf("adding %f to vector\n", layers[layers.size() - 1].neurons[i].a);
    outputs.push_back(layers[layers.size() - 1]->neurons[i]->a);
  }
  return outputs;
}

/*
Train a network by feeding input forward through the network, calculating the final output error,
backpropagating that error back through the hidden layers, and then calculating weight deltas to
correct the weights of the network. Does not use batches.
*/
void Network::train(const char *fileName, double trainingConstant){
  sampleSet *s = getSamples(fileName);
  const double tc = -trainingConstant; // optimization, please pass positive

  int numSamples = (int)s->inputData->size();

  for(int i = 0; i < numSamples; i++){
    feedForward(s->inputData->at(i));
    computeOutputError(s->answers->at(i));
    backPropagate();
    if(i%1000==0){
      printf("%s %f%%\n", fileName, i * 100.0 / numSamples);
    }
    for(int L = 1; L < (int)layers.size() - 1; L++){
      for(int N = 0; N < (int)layers[L]->neurons.size(); N++){
        for(int W = 0; W < (int)layers[L]->neurons[N]->weights.size(); W++){
          double deltaW = tc * layers[L - 1]->neurons[W]->a * layers[L]->neurons[N]->error;
          layers[L]->neurons[N]->weights[W] += deltaW;
        }
	      layers[L]->neurons[N]->bias += tc * layers[L]->neurons[N]->error;
      }
    }
    
    
  }
}

/*
This function should be unique to an implementation
 -> should be specified by the user, not hard-coded here
 
Assumes feedForward was already called, so the final
neurons each have an output
*/
void Network::computeOutputError(double answer){
  for(int i = 0; i < layers[layers.size() - 1]->nNeurons; i++){ // 0 to 25
    double y = i == answer ? 1 : 0;
    Neuron *n = layers[layers.size() - 1]->neurons[i]; // final layer
    n->error = (n->a - y) * n->a * (1 - n->a);
  }
}

/*
  Calculates and updates the errors of the neurons in the hidden layers.
  Requires that the output layer errors were calculated first.
*/
void Network::backPropagate(){
  // loop backwards from last hidden layer to first hidden layer
  for(int l = nLayers - 2; l >= 1; l--){
    for(int j = 0; j < layers[l]->nNeurons; j++){
      double sum = 0;
      for(int k = 0; k < (int)layers[l + 1]->neurons.size(); k++){
	      double w = layers[l + 1]->neurons[k]->weights[j];
	      double e = layers[l + 1]->neurons[k]->error;
	      sum += w * e;
      }
  	  double a = layers[l]->neurons[j]->a;
      layers[l]->neurons[j]->error = sum * a * (1 - a);
    }
  }
}

void printSample(vector<double> sample){
  for(int i = 0; i < (int)sample.size(); i++){
    printf((double)sample[i] == 0 ? "-, " : "1, ");
    if((i%28==0 && i != 0) || i == (int)sample.size() - 1){
      printf("\n");
    }
  }
}

void Network::test(const char *testingData){
  sampleSet *testing = getSamples(testingData);
  double numCorrect = 0;
  for(int i = 0; i < (int)testing->inputData->size(); i++){
    this->feedForward(testing->inputData->at(i));
    vector<double> outputs = this->getOutputs();
    double highestOutput = outputs[0], guess = 0;
    for(int j = 0; j < (int)outputs.size(); j++){
      if(outputs[j] > highestOutput){
        guess = j;
        highestOutput = outputs[j];
      }
    }
    if(testing->answers->at(i) == guess){
      numCorrect++;
    }else{
      printf("WRONG: Guess: %f, Answer: %f\n", guess, testing->answers->at(i));
      printSample(testing->inputData->at(i));
    }
    outputs.clear();
  }
  printf("%s %f%% correct\n", testingData, numCorrect * 100 / (double) testing->inputData->size());
}

sampleSet* getSamples(const char *fileName){
  struct data_collection *d = read_data((char*)fileName);
  sampleSet *s = (sampleSet*)malloc(sizeof(sampleSet));
  vector<vector<double> > *data = new vector<vector<double> >();
  vector<double> *answers = new vector<double>();
  int total = 0;
  for(int i = 0; i < d->num_arrays; i++){
    vector<double> sample;
    for(int j = 0; j < d->size; j++){
      for(int k = 0; k < d->size; k++){
	      sample.push_back((double)d->data[i][j][k]/255.0);
      }
    }
    data->push_back(sample);
    answers->push_back((double)d->answers[i]);
    if((double)d->answers[i]==0) total++;
  }
  s->inputData = data;
  s->sampleSize = d->size * d->size;
  s->answers = answers;
  return s;
}

/*
  Network FILE FORMAT:
  nInputs (int)
  nOutputs (int)
  nLayers (int)
  nNeurons (int) LAYER #0-n
  nWeights (int) NEURON #0-n
  weight (double) WEIGHT #0-n
  ...
  bias (double)
  ...
  ...
  EOF
*/
void Network::save(const char *fileName){
  FILE *fp = fopen(fileName, "wb");
  if(!fp){
    printf("Error opening %s\n", fileName);
    throw invalid_argument("file IO error");
  }
  fwrite(&nInputs, sizeof(nInputs), 1, fp);
  fwrite(&nOutputs, sizeof(nOutputs), 1, fp);
  fwrite(&nLayers, sizeof(nLayers), 1, fp);
  for(int i = 0; i < nLayers; i++){
    fwrite(&layers[i]->nNeurons, sizeof(layers[i]->nNeurons), 1, fp);
    for(int j = 0; j < layers[i]->nNeurons; j++){
      int nWeights = layers[i]->neurons[j]->weights.size();
      fwrite(&nWeights, sizeof(nWeights), 1, fp);
      for(int k = 0; k < (int)layers[i]->neurons[j]->weights.size(); k++){
	fwrite(&layers[i]->neurons[j]->weights[k], sizeof(layers[i]->neurons[j]->weights[k]), 1, fp);
      }
      fwrite(&layers[i]->neurons[j]->bias, sizeof(layers[i]->neurons[j]->bias), 1, fp);
    }
  }
  fclose(fp);
}

/*
  Loads a network from specified file. (See saveNetwork for file format)
*/
Network::Network(const char *fileName, errorFunc err){
  this->getError = err;
  FILE *fp = fopen(fileName, "rb");
  if(!fp){
    printf("Error opening %s\n", fileName);
    throw invalid_argument("file IO error");
  }
  size_t gcc = 0;
  int nInputs, nOutputs, nLayers;
  gcc += fread(&nInputs, sizeof(int), 1, fp) * sizeof(int);
  gcc += fread(&nOutputs, sizeof(int), 1, fp) * sizeof(int);
  gcc += fread(&nLayers, sizeof(int), 1, fp) * sizeof(int);
  printf("x=%dy=%dz=%d\n", nLayers, nInputs, nOutputs);
  this->nLayers = nLayers;
  this->nInputs = nInputs;
  this->nOutputs = nOutputs;
  // load each layer from the file
  for(int i = 0; i < nLayers; i++){
    vector<Neuron*> *neurons = new vector<Neuron*>();
    int nNeurons;
    gcc += fread(&nNeurons, sizeof(int), 1, fp) * sizeof(int);
    for(int j = 0; j < nNeurons; j++){
      vector<double> weights;
      int nWeights;
      gcc += fread(&nWeights, sizeof(int), 1, fp) * sizeof(int);
      for(int k = 0; k < nWeights; k++){
	      double weight;
	      gcc += fread(&weight, sizeof(double), 1, fp) * sizeof(double);
	      weights.push_back(weight);
      }
      double bias;
      gcc += fread(&bias, sizeof(double), 1, fp) * sizeof(double);
      neurons->push_back(new Neuron(weights, bias, i == 0));
    }
    layers.push_back(new Layer(neurons));
  }
  // Fix Layer 0's inPos
  for(int n = 0; n < layers[0]->nNeurons; n++){
    layers[0]->neurons[n]->bias = 0;
    for(int w = 0; w < (int)layers[0]->neurons[n]->weights.size(); w++){
      layers[0]->neurons[n]->weights[w] = 0;
    }
    layers[0]->neurons[n]->inPos = n;
    layers[0]->neurons[n]->weights[n] = 1;
  } 
  printf("Loaded neural network from file %s with %d inputs %d outputs %d layers (read %lu bytes).\n", fileName, nInputs, nOutputs, nLayers, gcc);
}
