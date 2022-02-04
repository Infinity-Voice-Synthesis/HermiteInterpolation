#pragma once

#include <algorithm>
#include <cmath>

class Hermite final
{
public:
	Hermite(double* x, double* y, int64_t n);
	~Hermite();

	double result(double t);
	void caculate();
private:
	Hermite();
	double* x = nullptr;
	double* y = nullptr;
	int64_t n = 0;

	double* delta = nullptr;
};

