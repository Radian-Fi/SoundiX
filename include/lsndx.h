#include <fstream>

using namespace std;

int decode(void);
void d2b(fstream& myfile, int data, int bit);
int start(fstream& myfile, int format, int tracks, int division);
void noteOn(fstream& myfile, int note, int velocity);
void noteOff(fstream& myfile, int channel, int note, int velocity);
long getFileSize(string filename);
void track(fstream& myfile, long length);
void deltaTime(fstream& myfile, long long time);
