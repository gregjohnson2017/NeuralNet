
class Neuron {
	public:
		double *weights, bias;
		int nWeights;
		const double trainingConstant;

		Neuron (int);
		~Neuron();
		double feed(double[], int);
		void train(double[], int, int);
};
