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
vector<double> add(vector<double> a, vector<double> b);
vector<double> subtract(vector<double> a, vector<double> b);
#endif
