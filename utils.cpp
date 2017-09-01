#include "utils.h"

int getRandom(int min, int max){
	return rand() % (max - min + 1) + min;
}

double sigmoid(double sum){
	return 1 / (1 + exp(-1 * sum));
}
