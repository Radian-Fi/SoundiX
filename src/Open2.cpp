#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <math.h>
#include <complex>

using namespace std;

void info(char fname[260], int f, int sr, int c, int num_items)
{
	SNDFILE *sf;
    SF_INFO info;
    
    sf = sf_open(fname,SFM_READ,&info);
    if (sf == NULL)
    {
        printf("Failed to open the file.\n");
        exit(-1);
	}
    f = info.frames;
    sr = info.samplerate;
    c = info.channels;
    num_items = f*c;
}

/*
void decodeMp3(char fname[260], complex<double> a[])
{
    mpg123_init();

    int err;
    mpg123_handle *mh = mpg123_new(NULL, &err);
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;

    int channels, encoding;
    long rate;
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));

    mpg123_open(mh, fname);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    unsigned int counter = 0;

    for (int totalBytes = 0; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; )
    {
        short* tst = reinterpret_cast<short*>(buffer);
        for (auto i = 0; i < buffer_size / 2; i++)
        {
            a[totalBytes+i] = tst[i];
        }
        counter += buffer_size/2;
        totalBytes += done;
    }
    free(buffer);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
}
*/

void decode(char fname[260], complex<double> a[])
{
    SNDFILE *sf;
    SF_INFO info;
    //int num_channels;
    //int sr, num;
    int num_items;
    int f,c;
    int *buf;
    FILE *out;

    
    /* Open the WAV file. */
    info.format = 0;
    sf = sf_open(fname,SFM_READ,&info);
    /* Print some of the info, and figure out how much data to read. */
    f = info.frames;
    sr = info.samplerate;
    c = info.channels;
    printf("Frames=				%d\n",f);
    printf("Samplerate=			%dHz\n",sr);
    printf("Channels=			%d\n",c);
    num_items = f*c;
    printf("Total number of samples=	%d\n",num_items);
    /* Allocate space for the data to be read, then read it. */
    buf = (int *) malloc(num_items*sizeof(int));
    num = sf_read_int(sf,buf,num_items);
    sf_close(sf);
    printf("Read %d samples\n",num);
    /* Write the data to filedata.out. */
	for (int i = 0; i < num; i += 1)
    {
		for (int j = 0; j < 1; ++j) {a[i+j] = buf[i+j]);} //using just first channel
    }
}

/*int main() {
    decode();
    return 0;
}*/
