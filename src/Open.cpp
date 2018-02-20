#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

using namespace std;

int decode()
{
    SNDFILE *sf;
    SF_INFO info;
    //int num_channels;
    int num, num_items;
    int f,sr,c;
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
    /* Write the data to filedata.out. */
    cout << buf[1] << endl;
    return 0;
}

int main()
{
    int buffer = decode();
    for(size_t i = sizeof(buffer) - 1; i>= 0; i--) {
        cout << buffer << endl;
    }

    /*FILE *out;
    int num;
    out = fopen("filedata.out","w");
    for (int i = 0; i < num; i += 1)
        {
        for (int j = 0; j < 1; ++j)
            fprintf(out,"%d ",buffer[i+j]);
        fprintf(out,"\n");
        }
    fclose(out);*/
}
