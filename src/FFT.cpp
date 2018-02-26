#define _USE_MATH_DEFINES
#include <math.h>

void hann (complex<double>* dataIn, int N)
{
	for (int i = 0; i < 2*N; i++)
	{
	double multiplier = 0.5 * (1 - cos((2*M_PIl*i)/(2*N-1)));
	dataIn[i] = multiplier * dataIn[i];
	}
}
