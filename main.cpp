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

using namespace std;

int main(int argc, char **argv){
  srand(time(NULL));
  Network *n = new Network(5, 4096, 10);
  samples *s = getSamples("./Quantifier/nums.dat");
  //n->train(s);
  //char *networkFile = strdup("network.nn"); // for Greg's education
  n->saveNetwork("network.nn");
  delete s->inputData;
  delete s->answers;
  free(s);
  return 1;
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
	sample.push_back((double)d->data[i][j][k]);
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
