#include <iostream>
#include <complex>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <fstream>
#include <lsndx.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits>
#include <vector>
#include <iterator>

using namespace std;

//int BUFFER_LEN; //number of samples (in window)
int sr, num;
int MAX;
double volumax = numeric_limits<double>::min();
double volumin = numeric_limits<double>::max();

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
	complex<double> f2[N];
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

void decomplex(complex<double>* f, int N, double result[][2])
{
	double df = (double) sr / (N*2);
	for(int i = 0; i < N; i++)
	{
		result[i][0] = abs(f[i]);
		result[i][1] = df*i;
	}
}

int compare(const void* a, const void* b) {
	double* arr1 = (double*)a;
	double* arr2 = (double*)b;
	if (arr1[0] > arr2[0]) return 1;
	else if (arr1[0] < arr2[0]) return -1; //else not needed here, just for better structure
	else return 0;
}

int filter(double res[][2], int N)
{
	qsort(res, N, 2*sizeof(double), compare);
	double last = numeric_limits<double>::max();
	int index = 0;
	for (int i = 0; i < N; ++i)
	{
		if (res[i][1] < last) //also could be if (res[i][1] = 0)
		{
			last = res[i][1];
			index = i;
		}
	}
	return index;
}

void vminmax(double res[][2], int N)
{
	if (res[0][0] < volumin)
		volumin = res[0][0];
	if (res[N-1][0] > volumax and res[N-1][1] != 0)
		volumax = res[N-1][0];
}

double volume(double x)
{
	x = x-volumin;
	x = x/(volumax-volumin)*127;
	return x;
}

int freq(double y)
{
	y = round(69.0 + 12*log2(y/440));
	return y;
}

void hann (complex<double>* dataIn, int N)
{
	for (int i = 0; i < N; i++)
	{
	double multiplier = 0.5 * (1 - cos((2*M_PIl*i)/(N-1)));
	dataIn[i] = multiplier * dataIn[i];
	}
}

void FFT(complex<double>* f, int N, double d)
{
	transform(f, N);
	for(int i = 0; i < N; i++)
	f[i] *= d; //multiplying by step
}

int main()
{
	double d = 1; //sampeling step
	decode();
	int MAX = pow(2,floor(log2(sr)));
	complex<double> vec[MAX];
	double result[MAX/2][2];
	int i,j;
	int m = 0;
	int o = 0;
	complex<double> a[num];
	//vector<vector<vector<double>>> notes;
	fstream myfile("filedata.out", ios_base::in);
	for (int j = 1; j < num+1; ++j)
	{
		myfile >> a[j];
		cout << "Reading audio data: " << (int)(j*100/num) << "%\r";
		cout.flush();
	}
	myfile.close();
	cout << "Reading audio data: 100%" << endl;
	fstream myfile2("filedata.out", ios_base::out);
	for (int i = 0; floor(i*MAX/50) < num; ++i)
        {
		fill(vec, vec+MAX, 0);
		for (int j = 0; j < MAX; ++j)
		{
			vec[j] = a[(int)floor(i*MAX/50)+j];
		}
		cout << "Computing FFT: " << (int)(m/(num/floor(MAX/0.5))) << "%\r";
		cout.flush();
		hann(vec, MAX);
		FFT(vec, MAX, d);
		decomplex(vec, MAX/2, result);
		int n = filter(result, MAX/2);
		vminmax(result, MAX/2);
		//cout << "...printing the FFT for the ";
		//cout << i*MAX << " to " << (i+1)*MAX-1 << "samples." << endl;
		/*for(int j = n; j < MAX/2; ++j)
			cout << result[j][1] << endl;*/
		for (int j = n; j < MAX/2; ++j)
		{
			/*notes.push_back(vector<vector<double>>());
			notes[i].push_back(vector<double>());
			notes[i][j].push_back(result[j][0]);
			notes[i][j].push_back(result[j][1]);*/
			myfile2 << m << endl;
			myfile2 << result[j][0] << endl;
			myfile2 << result[j][1] << endl;
			o+=1;
		}
		m+=1;
        }
	myfile2.close();
	cout << "Computing FFT: 100%" << endl;
	fstream myfile3("filedata.out", ios_base::in);
	fstream myfile4("notes.out", ios_base::out);
	int t;
	double x, y;
	for (int j = 0; j < o; ++j)
	{
		myfile3 >> t >> x >> y;
		myfile4 << t << endl;
		myfile4 << volume(x) << endl;
		myfile4 << freq(y) << endl;
		cout << "Writing out (to notes.out): " << (int)(j*100/o) << "%\r";
		cout.flush();
	}
	myfile3.close();
	myfile4.close();
	cout << "Writing out (to notes.out): 100%" << endl;
	//copy(begin(notes), end(notes), ostream_iterator<double>(cout, " "));
	cout << "Done." << endl;
	return 0;
}
