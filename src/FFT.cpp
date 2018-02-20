#define _USE_MATH_DEFINES
#include <math.h>

for (int i = 0; i < 2048; i++) {
	/* Twice the size of the buffer lenghth */
    double multiplier = 0.5 * (1 - cos(2*M_PIl*i/2047));
    dataOut[i] = multiplier * dataIn[i];
}
