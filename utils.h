#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdexcept>
#include <math.h>
#include <vector>
using namespace std;
// Returns an int between min and max, inclusive.
int getRandom(int min, int max);
double sigmoid(double x);
double sigmoid_prime(double x);
double inverse_sigmoid(double x);
vector<double> add(const vector<double> &a, const vector<double> &b);
vector<double> subtract(const vector<double> &a, const vector<double> &b);
#endif
