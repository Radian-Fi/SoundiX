#include <iostream>
#include <fstream>

using namespace std;

int main() //start()
{
    char fname[260];
    cout << "Select the file path (to save MIDI file): ";
	cin.getline(fname, sizeof fname);
	fstream myfile(fname, ios_base::out);
	myfile << "MThd " << 0x06 << " ";
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
