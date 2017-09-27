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

  srand(time(NULL));
  Network *n = new Network(4, 28*28, 26, NULL);
  n->train("./Quantifier/NetworkData/emnist-letters -training.dat", 1.0);
  n->save("networkL.nn");
  return 1;
}


