#ifndef NEURON_H
#define NEURON_H
#include <vector>
using namespace std;
class Neuron {
	public:
		vector<double> weights;
		Neuron(int);
		Neuron(vector<double> &weights, double bias);
		~Neuron();
		double bias;
		double error;
		double z;
		double feed(vector<double> &);
};
#endif
