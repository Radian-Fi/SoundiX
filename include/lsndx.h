#include <fstream>

using namespace std;

void info(char fname[260], int* f, int* sr, int* c, int* num_items);
void decode(char fname[260], complex<double> a[]);
//void decodeMp3(char fname[260], complex<double> a[]);

void d2b(fstream& myfile, int data, int bit);
int start(fstream& myfile, int format, int tracks, int division);
void noteOn(fstream& myfile, int note, int velocity);
void noteOff(fstream& myfile, int channel, int note, int velocity);
long getFileSize(string filename);
void track(fstream& myfile, long length);
void deltaTime(fstream& myfile, long long time);
