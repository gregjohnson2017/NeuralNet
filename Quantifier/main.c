#include "quantify.h"
#include "mnist.h"
int main(int argc, char **argv){
#ifdef TESTING
  write_data("./Samples28x28", 28, "mynums.dat");
  struct data_collection *mynums = read_data("mynums.dat");
  //print_data(mynums);
  //destroy_data(mynums);
#else
	struct data_collection *training = read_emnist_data("emnist-digits-train-images-idx3-ubyte", "emnist-digits-train-labels-idx1-ubyte");
	struct data_collection *testing = read_emnist_data("emnist-digits-test-images-idx3-ubyte", "emnist-digits-test-labels-idx1-ubyte");
	//struct data_collection *training = read_emnist_data("emnist-letters-train-images-idx3-ubyte", "emnist-letters-train-labels-idx1-ubyte");
	//struct data_collection *testing = read_emnist_data("emnist-letters-test-images-idx3-ubyte", "emnist-letters-test-labels-idx1-ubyte");
	//struct data_collection *training = read_emnist_data("emnist-bymerge-train-images-idx3-ubyte", "emnist-bymerge-train-labels-idx1-ubyte");
	//struct data_collection *testing = read_emnist_data("emnist-bymerge-test-images-idx3-ubyte", "emnist-bymerge-test-labels-idx1-ubyte");
	write_mnist_data(training, "emnist-digits-training.dat");
	write_mnist_data(testing, "emnist-digits-testing.dat");
	//print_data(testing);
	//generate_emnist_images(training);
#endif
	return 1;
}
