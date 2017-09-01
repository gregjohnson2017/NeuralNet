#include "utils.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
	srand(time(NULL));
	printf("%d\n", getRandom(0, 99));
	return 1;
}
