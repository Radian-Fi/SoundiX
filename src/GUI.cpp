#include "cpptk.h"
#include <iostream>

using namespace Tk;
using namespace std;

void drawSpectrogram(double notes[][128], int N)
{
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < 128; ++j)
		{
			int r = notes[i][j]*2;
			int g = notes[i][j]*2;
			int b = notes[i][j]*2;
			
			int x1 = winfo(width, ".c")/N*i;
			int x2 = x1+winfo(width, ".c")/N; //cloud be 2*x1, preventing 0,0 rectangle (1st iteration)
			".c" << create(rectangle, x1, j, x2, j+1) -fill(rgb(r, g, b))
		}
	}
}

/*
void change()
{
	lower();
	raise();
}
*/

int main(int, char *argv[])
{
	try
	{
		init(argv[0]);

		pack(canvas(".c") -background("grey"))
			-expand(true) -fill(both);
			
		drawSpectrogram(notes, i); //i = num_items(-1)
		
		//button(".b") -text("Spectrogram") -command(change);

		runEventLoop();
	}
	catch (exception const &e)
	{
		cerr << "Error: " << e.what() << '\n';
	}
}
