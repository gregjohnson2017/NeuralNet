#ifndef NEURON_H
#define NEURON_H
#include <vector>
using namespace std;
class Neuron {
	public:
		double bias;
		const double trainingConstant;
		vector<double> weights;
		Neuron (int);
		~Neuron();
		double feed(vector<double>);
};
#endif
