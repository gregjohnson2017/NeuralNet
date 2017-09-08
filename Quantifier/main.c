#include "quantify.h"
#include "mnist.h"

int main(int argc, char **argv){
	/*int size = 64;
	write_data("./Samples", size, "nums.dat");
	struct data_collection *d = read_data("nums.dat");
	//print_data(d);
	destroy_data(d);*/
	
	// new MNIST section
	
	struct data_collection *training = read_mnist_data("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
	printf("read training data\nenter to continue\n");
	getchar();
	write_mnist_data(training, "nums.dat");
	generate_mnist_images(training);
	return 1;
}
