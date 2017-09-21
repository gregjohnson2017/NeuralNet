#include "quantify.h"
#include "mnist.h"

int main(int argc, char **argv){
#ifdef TESTING
  write_data("./Samples28x28", 28, "mynums.dat");
  struct data_collection *mynums = read_data("mynums.dat");
  print_data(mynums);
  destroy_data(mynums);
#else
	struct data_collection *training = read_mnist_data("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
	struct data_collection *testing = read_mnist_data("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte");
	write_mnist_data(training, "training.dat");
	write_mnist_data(testing, "testing.dat");
	//generate_mnist_images(training);
#endif
	return 1;
}
