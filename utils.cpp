#include "utils.h"

int getRandom(int min, int max){
	return rand() % (max - min + 1) + min;
}
