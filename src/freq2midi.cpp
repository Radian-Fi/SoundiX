#include <iostream>
#include <fstream>

using namespace std;

char fname[260];

void d2b(int data, int bit, char*fname)
	{
		fstream myfile(fname, sizeof fname);
		for(int i = 0, i < bit, i+=8)
		{
			myfile << (char)(data-pow(2,(bit-i))+1)
		}
		myfile.close();
	}

int main() //start()
{
    cout << "Select the file path (to save MIDI file): ";
	cin.getline(fname, sizeof fname);
	fstream myfile(fname, ios_base::out);
	myfile << "MThd";
	d2b(6,32,fname);
	myfile.close();
	return 0;
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
