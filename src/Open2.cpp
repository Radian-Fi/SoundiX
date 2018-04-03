#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <sndfile.h>
#include <complex>

using namespace std;


void info(char fname[260], int* f, int* sr, int* c, int* num_items)
{
	SNDFILE *sf;
    SF_INFO info;
    
    sf = sf_open(fname,SFM_READ,&info);
    if (sf == NULL)
    {
        cout << "Failed to open the file." << endl;
        cout << "The file was " << fname << endl;
        exit(-1);
	}
    *f = info.frames;
    *sr = info.samplerate;
    *c = info.channels;
    *num_items = (*f)*(*c);
    sf_close(sf);
}

void decode(char fname[260], complex<double> a[])
{
    SNDFILE *sf;
    SF_INFO info;
    int sr, num;
    int num_items;
    int f,c;
    int *buf;

    
    /* Open the WAV file. */
    info.format = 0;
    sf = sf_open(fname,SFM_READ,&info);
    /* Print some of the info, and figure out how much data to read. */
    f = info.frames;
    sr = info.samplerate;
    c = info.channels;
    cout << "Frames=				" << f << endl;
    cout << "Samplerate=			" << sr << endl;
    cout << "Channels=				" << c << endl;
    num_items = f*c;
    cout << "Total number of samples=	" << num_items << endl;
    /* Allocate space for the data to be read, then read it. */
    buf = (int *) malloc(num_items*sizeof(int));
    num = sf_read_int(sf,buf,num_items);
    sf_close(sf);
    cout << "Read " << num << " samples" << endl;
    /* Write the data to filedata.out. */
	for (int i = 0; i < num; i += 1)
    {
		for (int j = 0; j < 1; ++j)
		{
			a[i+j] = buf[i+j]; //using just first channel
		}
    }
}
