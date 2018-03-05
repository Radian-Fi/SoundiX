#include <fstream>

using namespace std;

int decode(void);
void d2b(fstream& myfile, int data, int bit);
int start(int format, int tracks, int division);
void noteOn(fstream& myfile, int channel, int note, int velocity);
void track(fstream& myfile, long long length);
void deltaTime(fstream& myfile, long long time);
