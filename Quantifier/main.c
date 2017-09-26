#include "quantify.h"
#include "mnist.h"
int main(int argc, char **argv){
#ifdef TESTING
  write_data("./DadSamples28x28", 28, "./NetworkData/dadnums.dat");
  struct data_collection *dadnums = read_data("./NetworkData/dadnums.dat");
 	generate_mnist_images(dadnums);
  //print_data(mynums);
  //destroy_data(mynums);
#else
	//struct data_collection *training = read_mnist_data("./RawData/train-images.idx3-ubyte", "./RawData/train-labels.idx1-ubyte");
	//struct data_collection *testing = read_mnist_data("./RawData/t10k-images.idx3-ubyte", "./RawData/t10k-labels.idx1-ubyte");
	struct data_collection *training = read_emnist_data("./RawData/emnist-digits-train-images-idx3-ubyte", "./RawData/emnist-digits-train-labels-idx1-ubyte");
	struct data_collection *testing = read_emnist_data("./RawData/emnist-digits-test-images-idx3-ubyte", "./RawData/emnist-digits-test-labels-idx1-ubyte");
	//struct data_collection *training = read_emnist_data("./RawData/emnist-letters-train-images-idx3-ubyte", "./RawData/emnist-letters-train-labels-idx1-ubyte");
	//struct data_collection *testing = read_emnist_data("./RawData/emnist-letters-test-images-idx3-ubyte", "./RawData/emnist-letters-test-labels-idx1-ubyte");
	//struct data_collection *training = read_emnist_data("./RawData/emnist-bymerge-train-images-idx3-ubyte", "./RawData/emnist-bymerge-train-labels-idx1-ubyte");
	//struct data_collection *testing = read_emnist_data("./RawData/emnist-bymerge-test-images-idx3-ubyte", "./RawData/emnist-bymerge-test-labels-idx1-ubyte");
	write_mnist_data(training, "./NetworkData/emnist-digits-training.dat");
	write_mnist_data(testing, "./NetworkData/emnist-digits-testing.dat");
	//print_data(testing);
	generate_emnist_images(training);
#endif
	return 1;
}
