#ifndef NEURON_H
#define NEURON_H
#include <vector>
using namespace std;
class Neuron {
	public:
		double bias;
		vector<double> weights; // assume weights[k] == weight from this neuron to neuron k in the previous layer
		double error = 0;
		double z = 0;
		Neuron (int);
		~Neuron();
		double feed(vector<double> &);
};
#endif
