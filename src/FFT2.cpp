#include <iostream>
#include <complex>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <sstream>
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

int log2(int N)    //function to calculate the log2(.) of int numbers
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

void transform(complex<double>* f, int N)
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
	x = round(x/(volumax-volumin)*8)*15.875; //volume values divided in steps (9 volume steps)
	if (x < 63 or x > 127)	{x = 0;} //filtering by volume, volume under 63 ignored
	return x;
}

int freq(double y)
{
	y = round(69.0 + 12*log2(y/440));
	if (y < 1 or y > 127)	{y = 0;}
	return y;
}

void compress(double res[][2], int N, double notes[][128], int m)
{
	int f = 0;
	for (int i = 0; N-i > N-2; ++i) //set to return just 1 note with highest amplitude
	{
		f = freq(res[N-i][1]);
		if (notes[m][f] == 0)
		{
			notes[m][f] = volume(res[N-i][0]);
		}
	}
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
	double d = 1; //sampling step
	decode();
	int MAX = pow(2,floor(log2(sr)));
	complex<double> vec[MAX] = {0};
	double result[(int)ceil(MAX/2)][2] = {{0}};
	//double notebuf[128];
	double notes[(int)ceil(num/(MAX/60))][128] = {{0}};
	int i,j;
	int m = 0;
	//int o = 0;
	complex<double> a[num] = {0};
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
	//fstream myfile2("filedata.out", ios_base::out);
	for (int i = 0; floor(i*MAX/60) < num; ++i)
        {
		fill(vec, vec+MAX, 0);
		for (int j = 0; j < MAX; ++j)
		{
			vec[j] = a[(int)floor(i*MAX/60)+j];
		}
		cout << "Computing FFT: " << (int)(i/(num/(MAX/0.6))) << "%\r";
		cout.flush();
		hann(vec, MAX);
		FFT(vec, MAX, d);
		decomplex(vec, ceil(MAX/2+1), result);
		int n = filter(result, ceil(MAX/2+1));
		vminmax(result, ceil(MAX/2+1));
		compress(result, ceil(MAX/2+1), notes, i);
		//cout << "...printing the FFT for the ";
		//cout << i*MAX << " to " << (i+1)*MAX-1 << "samples." << endl;
		/*for(int j = n; j < ceil(MAX/2+1); ++j)
			cout << result[j][1] << endl;*/
		/*for (int j = 0; j < 128; ++j)
		{
			notes.push_back(vector<vector<double>>());
			notes[i].push_back(vector<double>());
			notes[i][j].push_back(result[j][0]);
			notes[i][j].push_back(result[j][1]);
			myfile2 << i << endl;
			myfile2 << notes[j] << endl;
			o+=1;
		}*/
		m = i;
        }
	//myfile2.close();
	cout << "Computing FFT: 100%" << endl;
	//fstream myfile3("filedata.out", ios_base::in);
	char fname[260];
	cout << "Select the file path (to save MIDI file): ";
	cin.getline(fname, sizeof fname);
	fstream myfile4(fname, ios_base::out);
	fstream myfile5("notes.out", ios_base::out);
	fstream test("test.out", ios_base::out);
	start(myfile4,1,1,96); //32768+256*30+2 32 delta-t in one quarternote
	int t, time, n = 0;
	int headersize = 46;
	string header[headersize] = {
						"00", "ff", "58", "04", "04", "02", "18", "08",		//time signature
						"00", "ff", "59", "02", "00", "00",				//key signature
						"00", "b0", "79", "00",						//reset all (controller)
						"00", "c0", "00",							//program change
						"00", "b0", "07", "64",						//set controller volume
						"00", "0a", "40",							//set expression / pan, 64 = center
						"00", "5b", "00",							//reverb, set to 0
						"00", "5d", "00",							//chorus, set to 0
						"00", "ff", "21", "01", "00",				//midi port, set to 0
						"00", "ff", "51", "03", "18", "6a", "00"};						//plus 1/MAX*32, as time per quater note
	for (int i = 0; i < headersize; ++i)
	{
		stringstream(header[i]) >> hex >> n;
		myfile5 << (char)n;
	}
	//d2b(myfile5,1600000,24); //time per quater note
	double x, y;
	double change[128] = {0};
	deltaTime(myfile5,0); //first delta time index before note
	d2b(myfile5,144,8); //defining row of noteOn messages on channel 1 (written as 0)
	for (int j = 0; j < m; ++j)
	{
		for (int y = 0; y < 128; ++y)
		{
			x = (int)notes[j][y];
			//myfile3 >> t >> x;
			if (j == 0)
			{
				change[y] = x;
				if (x > 0)
				{
					if (j != 0 and y !=0) {deltaTime(myfile5,t);}
					//myfile5 << (char)144;
					noteOn(myfile5,y,x);
					//test << j << " : " << y << " " << x << endl;
				}
			}
			if (x != change[y])
			{
				t = j - time;
				time = j;
				deltaTime(myfile5,t);
				//noteOff(myfile5,0,y,0);
				change[y] = x;
				//deltaTime(myfile5,0);
				noteOn(myfile5,y,x);
				//test << j << " : " << y << " " << x << endl;
			}
		}
		cout << "Writing out to " << fname << " :" << (int)(j*100/m) << "%\r";
		cout.flush();
	}
	int eofsize = 4;
	string eof[eofsize] = {"01", "ff", "2f", "00"};
	for (int i = 0; i < eofsize; ++i)
	{
		stringstream(eof[i]) >> hex >> n;
		myfile5 << 	(char)n;
	}
	myfile5.close();
	long length = getFileSize("notes.out");
	track(myfile4,length);
	fstream myfile6("notes.out", ios_base::in);
	myfile4 << myfile6.rdbuf();
	//myfile3.close();
	myfile4.close();
	cout << "Writing out to " << fname << " :" << "100%" << endl;
	//copy(begin(notes), end(notes), ostream_iterator<double>(cout, " "));
	cout << "Done." << endl;
	return 0;
}
