#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <math.h>
#include <tuple>

using namespace std;

extern int sr, num;

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

int decode()
{
    SNDFILE *sf;
    SF_INFO info;
    //int num_channels;
    //int sr, num;
    int num_items;
    int f,c;
    int *buf;
    FILE *out;
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
    out = fopen("filedata.out","w");
    for (int i = 0; i < num; i += 1)
        {
        for (int j = 0; j < 1; ++j)
            fprintf(out,"%d ",buf[i+j]);
        fprintf(out,"\n");
        }
    fclose(out);
    return num;
}

/*int main() {
    decode();
    return 0;
}*/
