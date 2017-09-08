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

samples* getSamples(const char *fileName);
void printSample(vector<double> sample);
void printHighestLayerZ(Network*);
void printAverageLayerZ(Network*);
void printAverageLayerActivation(Network*);
void printAverageLayerWeights(Network *n);
void printAverageLayerError(Network *n);
using namespace std;

int main(int argc, char **argv){
  srand(time(NULL));
  if(atoi(argv[1])){
    Network *n = new Network("network.nn");
    //printf("Network Loaded: information\n");
    //n->printNetwork();
    
    samples *s = getSamples("./Quantifier/nums.dat");
    double numCorrect = 0;
    for(int i = 0; i < (int)s->inputData->size(); i++){
      //printf("feeding network sample %d\n", i);
      n->feedNetwork(s->inputData->at(i));
      //printSample(s->inputData->at(i));
      //printf("answer = %f\n", s->answers->at(i));
      vector<double> outputs = n->getOutputs();
      double highestOutput = outputs[0], guess = 0;
      for(int j = 0; j < (int)outputs.size(); j++){
        if(outputs[j] > highestOutput){
          guess = j;
          highestOutput = outputs[j];
        }
        //printf("N%d=%f\n", j, outputs[j]);
      }
      if(s->answers->at(i) == guess){
        //printf("Correct! Answer: %f, Guess: %f\n", (double)s->answers->at(i), (double)guess);
        //getchar();
        numCorrect++;
      }
      //printHighestLayerZ(n);
      //printAverageLayerZ(n);
      //printAverageLayerWeights(n);
      //printAverageLayerActivation(n);
      outputs.clear();
      //getchar();
    }
    printf("%f%% correct\n", numCorrect * 100 / (double) s->inputData->size());
  }else{
    Network *n = new Network(5, 28*28, 10);
    samples *s = getSamples("./Quantifier/nums.dat");
    n->train(s);
    n->saveNetwork("network.nn");
  }
  return 1;
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
    printf("%d, ", (int)sample[i]);
    if((i%64==0 && i != 0) || i == (int)sample.size() - 1){
      printf("\n");
    }
  }
}

samples* getSamples(const char *fileName){
  data_collection *d = read_data((char*)fileName);
  samples *s = (samples*)malloc(sizeof(samples));
  vector<vector<double> > *data = new vector<vector<double> >();
  vector<double> *answers = new vector<double>();
  for(int i = 0; i < d->num_arrays; i++){
    vector<double> sample;
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
