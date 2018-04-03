#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <complex>
#include <stdlib.h>
#include <sndfile.h>
#include <sstream>
#include <fstream>
#include <lsndx.h>
#include <limits>
#include <iterator>

using namespace std;

double volumax = numeric_limits<double>::min();
double volumin = numeric_limits<double>::max();


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

void decomplex(complex<double>* f, int N, double result[][2], int sr)
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
	return 0;
}

void vminmax(double res[][2], int N)
{
	if (res[0][0] < volumin)
		volumin = res[0][0];
	if (res[N-1][0] > volumax and res[N-1][1] != 0)
		volumax = res[N-1][0];
}

double volume(double x, int steps, int limit)
{
	x = x-volumin;
	float multiplier = 127/steps;
	if (x*127./(volumax-volumin) < limit){ //filtering by volume, volume under 'limit' ignored
		x = 0;
	}
	else {
		//x = 127;
		x = x/(volumax-volumin)*steps*multiplier; //volume values divided in steps (127 volume steps) 8)
	}
	return x;
}

int freq(double y)
{
	y = round(69.0 + 12*log2(y/440));
	if (y < 1 or y > 127)	{y = 0;}
	return y;
}

void compress(double res[][2], int N, double notes[][128], int m, int steps, int limit,
              int maxSimultaneousNotes)
{
	int f = 0;
	int n = 0;
	for (int i = 0; N-i > 0; ++i) //set to N-i > N-1
	{
		f = freq(res[N-i][1]);
		if (f == 0) { //ignore the frequency 0 Hz
			continue;
		}
		if (notes[m][f] == 0)
		{
			notes[m][f] = volume(res[N-i][0], steps, limit);
			//cout << "res[N-i][0] " << res[N-i][0] << endl;
			//cout << "notes[" << m << "]["<<f<<" set to " <<notes[m][f]<<endl; 
			n+=1;
		}
		if (n >= maxSimultaneousNotes) {break;} //also could be if (n == 127)
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

int main(int, char *argv[])
{
	char fname[260];
	cout << "Select the file path: ";
	cin.getline(fname, sizeof fname);
	int f, sr, c, num_items, num;
    
	info(fname, &f, &sr, &c, &num_items);
	cout << num_items << endl;
	complex<double> a[num_items];
	for  (int i = 0; i < num_items; i++){
		a[i] = (double)0.;
	}
	
	decode(fname, a);
	
	char option;
	cout << "Would you like to continue? [Y/N]? ";
	cin >> option;
	if (toupper(option) == 'N')
	{
		return 0;
	}
	
	double d = 1; //sampling step
	int MAX = pow(2,floor(log2(sr)));
	complex<double> vec[MAX] = {0};
	double result[(int)ceil(MAX/2)][2] = {{0}};
	
	double notes[(int)ceil(num_items/(MAX/60))][128] = {{0}};
	int i,j;
	int m = 0;
	
	int steps = 1;
	int limit = 20;
	int maxSimultaneousNotes = 1;
	int maximumNotes = sizeof(int);
	
	cout << endl;
	cout << "Preferences" << endl;
	cout << endl;
	
	cout << "Volume steps [0-127]: " << endl;
	cin >> steps;
	
	cout << "Noise gate limit [0-127]: " << endl;
	cin >> limit;
	
	cout << "Maximum number of simultaneous notes [0-" << maximumNotes << "]: " << endl;
	cin >> maxSimultaneousNotes;
	
	for (int i = 0; floor(i*MAX/60) < num_items - MAX; ++i)
        {
		fill(vec, vec+MAX, 0);
		for (int j = 0; j < MAX; ++j)
		{
			vec[j] = a[(int)floor(i*MAX/60)+j];
		}
		cout << "Computing FFT: " << (int)(i/(num_items/(MAX/0.6))) << "%\r";
		cout.flush();
		hann(vec, MAX);
		FFT(vec, MAX, d);
		decomplex(vec, ceil(MAX/2+1), result, sr);
		filter(result, ceil(MAX/2+1));
		vminmax(result, ceil(MAX/2+1));
		compress(result, ceil(MAX/2+1), notes, i, steps, limit, maxSimultaneousNotes);
		m = i;
		}
	cout << "Computing FFT: 100%" << endl;
	char filename[260];
	cout << "Select the file path (to save MIDI file): ";
	cin.getline(filename, sizeof filename);
	fstream outFile(filename, ios_base::out);
	fstream tmpFile("notes.out", ios_base::out);
	start(outFile,1,1,96); //32768+256*30+2 32 delta-t in one quarternote
	int duration, lastChange = 0;
	int headersize = 50;
	int hexCode;
	string header[headersize] = {
						"00", "ff", "58", "04", "04", "02", "18", "08",		//time signature
						"00", "ff", "59", "02", "00", "00",				//key signature
						"00", "b0", "79", "00",						//reset all (controller)
						"00", "c0", "4f",							//program change (instrument)
						"00", "b0", "07", "64",						//set controller volume
						"00", "b0", "40", "40",						//set sustain (pedal)
						"00", "0a", "40",							//set expression / pan, 64 = center
						"00", "5b", "00",							//reverb, set to 0
						"00", "5d", "00",							//chorus, set to 0
						"00", "ff", "21", "01", "00",				//midi port, set to 0
						"00", "ff", "51", "03", "18", "6a", "00"};						//plus 1/MAX*32, as time per quater note
	for (int i = 0; i < headersize; ++i)
	{
		stringstream(header[i]) >> hex >> hexCode;
		tmpFile << (char)hexCode;
	}
	double amplitude, frequency;
	double lastAmplitude[128] = {0};
	deltaTime(tmpFile,0); //first delta time index before note
	d2b(tmpFile,144,8); //defining row of noteOn messages on channel 1 (written as 0)
	for (int j = 0; j < m; ++j)
	{
		for (int frequency = 0; frequency < 128; ++frequency)
		{
			amplitude = (int)notes[j][frequency];
			if (j == 0)
			{
				lastAmplitude[frequency] = amplitude;
				if (amplitude > 0)
				{
					noteOn(tmpFile,frequency,amplitude);
				}
			}
			if (amplitude != lastAmplitude[frequency])
			{
				duration = j - lastChange;
				lastChange = j;
				//cout << "change at " << lastChange << endl; 
				//cout << "writing delta " << duration << endl;
				deltaTime(tmpFile,duration);
				lastAmplitude[frequency] = amplitude;
				noteOn(tmpFile,frequency,amplitude);
			}
			if (amplitude){
				//cout << "note " << frequency << " at " << j << endl;
			}
		}
		cout << "Writing out to " << fname << " :" << (int)(j*100/m) << "%\r";
		cout.flush();
		
	}
	duration = m  - 1 - lastChange;
	deltaTime(tmpFile,duration);
	int eofsize = 3;
	string eof[eofsize] = {"ff", "2f", "00"};
	for (int i = 0; i < eofsize; ++i)
	{
		stringstream(eof[i]) >> hex >> hexCode;
		tmpFile << 	(char)hexCode;
	}
	tmpFile.close();
	long length = getFileSize("notes.out");
	track(outFile,length);
	fstream tmpFileIn("notes.out", ios_base::in);
	outFile << tmpFileIn.rdbuf();
	outFile.close();
	remove("notes.out");
	cout << "Writing out to " << filename << " :" << "100%" << endl;
	//copy(begin(notes), end(notes), ostream_iterator<double>(cout, " "));
	cout << "Done." << endl;
	return 0;
}
