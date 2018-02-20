#include <iostream>
#include <complex>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define MAX 256
int *buf;
int num,c;
//int BUFFER_LEN; //number of samples (in window)


using namespace std;

int log2(int N)    /*function to calculate the log2(.) of int numbers*/
{
	int k = N, i = 0;
	while(k) {
		k >>= 1;
		i++;
	}
	return i - 1;
}

int check(int n)    //checking if the number of element is a power of 2
{
	return n > 0 && (n & (n - 1)) == 0;
}

int reverse(int N, int n)    //calculating revers number
{
	  int j, p = 0;
	  for(j = 1; j <= log2(N); j++) {
		if(n & (1 << (log2(N) - j)))
			p |= 1 << (j - 1);
	  }
	  return p;
}

void order(complex<double>* f1, int N) //using the reverse order in the array
{
	complex<double> f2[MAX];
	for(int i = 0; i < N; i++) {
		f2[i] = f1[reverse(N, i)];
	}
	for(int j = 0; j < N; j++) {
		f1[j] = f2[j];
	}
}

void transform(complex<double>* f, int N) //
{
	order(f, N);    //first: reverse order
	complex<double> *W;
	W = (complex<double> *)malloc(N / 2 * sizeof(complex<double>));
	W[1] = polar(1., -2. * M_PI / N);
	W[0] = 1;
	for(int i = 2; i < N / 2; i++)
	W[i] = pow(W[1], i);
	int n = 1;
	int a = N / 2;
	for(int j = 0; j < log2(N); j++) {
		for(int i = 0; i < N; i++) {
			if(!(i & n)) {
				complex<double> temp = f[i];
				complex<double> Temp = W[(i * a) % (n * a)] * f[i + n];
				f[i] = temp + Temp;
				f[i + n] = temp - Temp;
			}
		}
		n *= 2;
		a = a / 2;
	}
}

void FFT(complex<double>* f, int N, double d)
{
	transform(f, N);
	for(int i = 0; i < N; i++)
	f[i] *= d; //multiplying by step
}

void decode()
{
    SNDFILE *sf;
    SF_INFO info;
    //int num_channels;
    int num_items;
    int f,sr;
    char fname[260];

    
    /* Open the WAV file. */
    info.format = 0;
    cout << "Select the file path: ";
    cin.getline(fname, sizeof fname);
    sf = sf_open(fname,SFM_READ,&info);
    if (sf == NULL) {
        printf("Failed to open the file.\n");
        exit(-1);
        }
    /* Print some of the info, and figure out how much data to read. */
    f = info.frames;
    sr = info.samplerate;
    c = info.channels;
    printf("frames=%d\n",f);
    printf("samplerate=%d\n",sr);
    printf("channels=%d\n",c);
    num_items = f*c;
    printf("num_items=%d\n",num_items);
    /* Allocate space for the data to be read, then read it. */
    buf = (int *) malloc(num_items*sizeof(int));
    num = sf_read_int(sf,buf,num_items);
    sf_close(sf);
    printf("Read %d items\n",num);
}

int main()
{
	double d = 1; //sampeling step
	complex<double> vec[MAX];
	int i,j;
	decode();
	for (int i = 0; i < num; i += c) {
		for (int j = 0; j < c; ++j) {
			vec[i+j] = buf[i+j];
		}
        }
	int n = 256;
	FFT(vec, n, d);
	cout << "...printing the FFT" << endl;
	for(int j = 0; j < n; j++)
	cout << vec[j] << endl;
	return 0;
}
