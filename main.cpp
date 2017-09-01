#include "utils.h"
#include "neuron.h"
#include "layer.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
	srand(time(NULL));
	printf("%d\n", getRandom(0, 99));
	Neuron n(3);
	return 1;
}
