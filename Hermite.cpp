#include "Hermite.h"

Hermite::Hermite()
{

}

Hermite::Hermite(double* x, double* y, int64_t n)
	:x(x), y(y), n(n)
{
	if (n < 2) {
		return;
	}

	this->delta = new double[n];
	this->caculate();
}

Hermite::~Hermite()
{
	if (this->delta != nullptr) {
		delete[] this->delta;
		this->delta = nullptr;
	}
}

void Hermite::caculate()
{
	this->delta[0] = 0;
	this->delta[n - 1] = 0;
	for (uint64_t i = 1; i < this->n - 1; i++) {
		this->delta[i] = (this->y[i + 1] - this->y[i - 1]) / (this->x[i + 1] - this->x[i - 1]);
	}
}

double Hermite::result(double t)
{
	if (this->n < 2) {
		return 0;
	}

	auto f1 = [](double& T1, double& T2, double& D1, double& D2)->double {return (1.0 + 2.0 * T1) * pow(T2, 2.0); };
	auto f2 = [](double& T1, double& T2, double& D1, double& D2)->double {return (1.0 + 2.0 * T2) * pow(T1, 2.0); };
	auto f3 = [](double& T1, double& T2, double& D1, double& D2)->double {return D1 * pow(T2, 2.0); };
	auto f4 = [](double& T1, double& T2, double& D1, double& D2)->double {return D2 * pow(T1, 2.0); };

	for (uint64_t i = 0; i < this->n - 1; i++) {
		if (t >= this->x[i] && t < this->x[i + 1]) {
			double delta1 = (t - this->x[i]);
			double delta2 = (t - this->x[i + 1]);
			double T1 = delta1 / (this->x[i + 1] - this->x[i]);
			double T2 = delta2 / (this->x[i] - this->x[i + 1]);
			return f1(T1, T2, delta1, delta2) * this->y[i] + f2(T1, T2, delta1, delta2) * this->y[i + 1] + f3(T1, T2, delta1, delta2) * this->delta[i] + f4(T1, T2, delta1, delta2) * this->delta[i + 1];
		}
	}

	return 0;
}