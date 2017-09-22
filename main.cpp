#include "utils.h"
#include "neuron.h"
#include "layer.h"
#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

extern "C" {
#include "./Quantifier/quantify.h"
}

sampleSet* getSamples(const char *fileName);
void printSample(vector<double> sample);
void printHighestLayerZ(Network*);
void printAverageLayerZ(Network*);
void printAverageLayerActivation(Network*);
void printAverageLayerWeights(Network *n);
void printAverageLayerError(Network *n);
void trainNetwork(Network *n, const char *trainingData, double trainingConstant);
double testNetwork(Network *n, const char *testingData);
using namespace std;

int main(int argc, char **argv){
  srand(time(NULL));
  double bestPC = 0, bestTraining = 0, bestLayers = 3;
  for(double training = 0.90; 0; training += 0.05){
    for(double layers = 3; 0; layers++){
      Network *n = new Network(layers, 28*28, 10);
      trainNetwork(n, "./Quantifier/training.dat", training);
      double PC = testNetwork(n, "./Quantifier/testing.dat");
      if(PC > bestPC){
        bestPC = PC;
        bestTraining = training;
        bestLayers = layers;
        printf("New best PC = %f with training = %f and %f layers\n", bestPC, bestTraining, bestLayers);
        n->saveNetwork("networkBest.nn");
      }
    }
  }
  printf("Best PC = %f, best training = %f\n", bestPC, bestTraining);
  return 1;
}

void trainNetwork(Network *n, const char *trainingData, double trainingConstant){
  sampleSet *training = getSamples(trainingData);
  n->train(training, trainingConstant);
}

double testNetwork(Network *n, const char *testingData){
  sampleSet *testing = getSamples(testingData);
  double numCorrect = 0;
  for(int i = 0; i < (int)testing->inputData->size(); i++){
    n->feedNetwork(testing->inputData->at(i));
    vector<double> outputs = n->getOutputs();
    double highestOutput = outputs[0], guess = 0;
    for(int j = 0; j < (int)outputs.size(); j++){
      if(outputs[j] > highestOutput){
        guess = j;
        highestOutput = outputs[j];
      }
    }
    if(testing->answers->at(i) == guess){
      numCorrect++;
    }
    outputs.clear();
  }
  double PC = numCorrect * 100 / (double) testing->inputData->size();
  printf("%f%% correct\n", PC);
  return PC;
}

void printAverageLayerWeights(Network *n){
  for(int L = 0; L < (int)n->layers.size(); L++){
    double totalW = 0, countW = 0;
    for(int N = 0; N < (int)n->layers[L]->neurons.size(); N++){
      for(int W = 0; W < (int)n->layers[L]->neurons[N]->weights.size(); W++){
        totalW += n->layers[L]->neurons[N]->weights[W];
        countW++;
      }
    }
    printf("Layer %d average weights = %f\n", L, totalW / countW);
  }
}

void printAverageLayerActivation(Network *n){
  for(int L = 0; L < (int)n->layers.size(); L++){
    double totalA = 0;
    for(int N = 0; N < (int)n->layers[L]->neurons.size(); N++){
      totalA += n->layers[L]->neurons[N]->a;
    }
    printf("Layer %d average a = %f\n", L, totalA / (double)n->layers[L]->neurons.size());
  }
}

void printAverageLayerZ(Network *n){
  for(int L = 0; L < (int)n->layers.size(); L++){
    double totalZ = 0;
    for(int N = 0; N < (int)n->layers[L]->neurons.size(); N++){
      totalZ += n->layers[L]->neurons[N]->z;
    }
    printf("Layer %d average z = %f\n", L, totalZ / (double)n->layers[L]->neurons.size());
  }
}

void printAverageLayerError(Network *n){
  for(int L = 0; L < (int)n->layers.size(); L++){
    double totalError = 0;
    for(int N = 0; N < (int)n->layers[L]->neurons.size(); N++){
      totalError += n->layers[L]->neurons[N]->error;
    }
    printf("Layer %d average error = %f\n", L, totalError / (double)n->layers[L]->neurons.size());
  }
}

void printHighestLayerZ(Network *n){
  for(int L = 0; L < (int)n->layers.size(); L++){
    double highestZ = n->layers[L]->neurons[0]->z;
    for(int N = 0; N < (int)n->layers[L]->neurons.size(); N++){
      if(n->layers[L]->neurons[N]->z > highestZ){
        highestZ = n->layers[L]->neurons[N]->z;
      }
    }
    printf("Layer %d highest z = %f\n", L, highestZ);
  }
}

void printSample(vector<double> sample){
  for(int i = 0; i < (int)sample.size(); i++){
    printf("%f, ", (double)sample[i]);
    if((i % 28 == 0 && i != 0) || i == (int)sample.size() - 1){
      printf("\n");
    }
  }
}

sampleSet* getSamples(const char *fileName){
  data_collection *d = read_data((char*)fileName);
  sampleSet *s = (sampleSet*)malloc(sizeof(sampleSet));
  vector<vector<double> > *data = new vector<vector<double> >();
  vector<double> *answers = new vector<double>();
  for(int i = 0; i < d->num_arrays; i++){
    vector<double> sample;
    if(d->size != 28){
      printf("AHHH\n");
    }
    for(int j = 0; j < d->size; j++){
      for(int k = 0; k < d->size; k++){
	      sample.push_back((double)d->data[i][j][k]/255.0);
      }
    }
    data->push_back(sample);
    answers->push_back((double)d->answers[i]);
  }
  s->inputData = data;
  s->sampleSize = d->size * d->size;
  s->answers = answers;
  return s;
}
