#include "utils.h"
#include <vector>

using namespace std;

int getRandom(int min, int max){
	return rand() % (max - min + 1) + min;
}

/*
Returns the calculated sigmoid value 1/(1+e^-x)
Range is 0 to 1 non-inclusive
*/
double sigmoid(double x){
	return 1 / (1 + exp(-1 * x));
}

double inverse_sigmoid(double x){
  return -log((1-x)/x);
}

double sigmoid_prime(double x){
	return sigmoid(x) * (1 - sigmoid(x));
}

/*
Returns a new vector of a + b
*/
vector<double> add(const vector<double> &a, const vector<double> &b){
	vector<double> c;
	if(a.size() != b.size()){
		throw invalid_argument("cannot add vectors of different sizes");
	}
	for(int i = 0; i < (int)a.size(); i++){
		c.push_back(a[i] + b[i]);
	}
	return c;
}

/*
Returns a new vector of a - b
*/
vector<double> subtract(const vector<double> &a, const vector<double> &b){
	vector<double> c;
	if(a.size() != b.size()){
		throw invalid_argument("cannot subtract vectors of different sizes");
	}
	for(int i = 0; i < (int)a.size(); i++){
		c.push_back(a[i] - b[i]);
	}
	return c;
}
