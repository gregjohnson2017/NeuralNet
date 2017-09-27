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

using namespace std;

int main(int argc, char **argv){

  Network *n = new Network("network.nn", NULL);
  const char *emnist_testing = "./Quantifier/NetworkData/emnist-digits-testing.dat";
  //const char *mnist_testing = "./Quantifier/NetworkData/mnist-digits-testing.dat";
  //const char *mynums = "./Quantifier/NetworkData/dadnums.dat";
  //n->test(mnist_testing);
  n->test(emnist_testing);
  //n->test(mynums);
  return 1;
}

