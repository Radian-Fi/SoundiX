#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/stat.h>

using namespace std;

void d2b(fstream& myfile, int data, int bit)
	{
		//fstream myfile(fname, ios_base::out);
		for(int i = 8; i < bit+8; i+=8)
		{
			int j = (data-pow(2,(bit-i))+1);
			if(j < 0) {myfile << (char)0;}
			else      {myfile << (char)j;}
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

void track(fstream& myfile, long long length)
{
	myfile << "MTrk";
	d2b(myfile,length,32);
}

void deltaTime(fstream& myfile, long long time)
{
	d2b(myfile,time,ceil(log2(time+1)));
}

void noteOn(fstream& myfile, int channel, int note, int velocity)
{
	d2b(myfile,144+channel,8);
	d2b(myfile,note,8);
	d2b(myfile,velocity,8);
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
