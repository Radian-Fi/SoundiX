#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/stat.h>

using namespace std;

void d2b(fstream& myfile, int data, int bit)
	{
		//fstream myfile(fname, ios_base::out);
		int k = 0;
		for(int i = 8; i < bit+8; i+=8)
		{
			for(int j = 1; bit-j-i+8 >= bit-i; ++j)
			{
				if (pow(2,(bit-j-i+8)) < data)
				{
					k = k + pow(2,(8-j));
					data = data - pow(2,(bit-j-i+8));
				}
			}
			myfile << (char)k;
		}
		//myfile.close();
	}

int start(fstream& myfile, int format, int tracks, int division)
{
	myfile << "MThd";
	d2b(myfile,6,32);
	d2b(myfile,format,16);
	d2b(myfile,tracks,16);
	d2b(myfile,division,16);
	return 0;
}

void track(fstream& myfile, long length)
{
	myfile << "MTrk";
	d2b(myfile,length,32);
}

void deltaTime(fstream& myfile, long long time)
{
	d2b(myfile,time,floor(log2(time+2)));
}

void noteOn(fstream& myfile, int channel, int note, int velocity)
{
	d2b(myfile,144+channel+1,8);
	d2b(myfile,note+1,8);
	d2b(myfile,velocity+1,8);
}

long getFileSize(string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

void __attribute__ ((constructor)) initLibrary(void) {
 //
 // Function that is called when the library is loaded
 //
    printf("Library is initialized\n");
}
void __attribute__ ((destructor)) cleanUpLibrary(void) {
 //
 // Function that is called when the library is »closed«.
 //
    printf("Library is exited\n"); 
}
