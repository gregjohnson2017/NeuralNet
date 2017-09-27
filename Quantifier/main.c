#include "quantify.h"
int main(int argc, char **argv){
	create_data_file("./Blurry", "./Crisp", 28, "blur_training.dat");
	data_collection *training_blurry = read_data("blur_training.dat");
	printf("count = %d, size = %d\n", training_blurry->num_arrays, training_blurry->size);
	destroy_data(training_blurry); 
	return 1;
}
