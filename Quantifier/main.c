#include "quantify.h"
#include "mnist.h"

int main(int argc, char **argv){
	struct data_collection *training = read_mnist_data("train-images.idx3-ubyte", "train-labels.idx1-ubyte");
<<<<<<< HEAD
	//struct data_collection *training = read_mnist_data("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte");
	printf("read training data\nenter to continue\n");
	getchar();
	write_mnist_data(training, "nums.dat");
=======
	struct data_collection *testing = read_mnist_data("t10k-images.idx3-ubyte", "t10k-labels.idx1-ubyte");
	write_mnist_data(training, "training.dat");
	write_mnist_data(testing, "testing.dat");
>>>>>>> 5356a150a832205bdc39c43dd39e883bb9a21657
	//generate_mnist_images(training);
	return 1;
}
