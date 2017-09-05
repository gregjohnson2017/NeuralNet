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
		double bias, error, z, a;
		void feed(vector<double> &);
};
#endif
