/*
 * SoundiX.cpp
 * 
 * Copyright 2017 Adrian Filcik <adrian.filcik@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 /*
 * Copyright (C) 2007-2011 Erik de Castro Lopo <erikd@mega-nerd.com>
 */


#include <iostream>

#include <cstdio>
#include <cstring>

#include <sndfile.hh>

#define BUFFER_LEN 1024

using namespace std;

static void create_file (const char * fname, int format)
{	static short buffer [BUFFER_LEN] ;

	SndfileHandle file ;
	int channels = 2 ;
	int srate = 48000 ;

	printf ("Creating file named '%s'\n", fname) ;

	file = SndfileHandle (fname, SFM_WRITE, format, channels, srate) ;

	memset (buffer, 0, sizeof (buffer)) ;

	file.write (buffer, BUFFER_LEN) ;

	cout << ".\n" ;
} /* create_file */

static void read_file (const char * fname)
{	static short buffer [BUFFER_LEN] ;

	SndfileHandle file ;

	file = SndfileHandle (fname) ;

	printf ("Opened file '%s'\n", fname) ;
	printf ("    Sample rate : %d\n", file.samplerate ()) ;
	printf ("    Channels    : %d\n", file.channels ()) ;

	file.read (buffer, BUFFER_LEN) ;

	cout << ".\n" ;
} /* read_file */

int main (void)
{	const char * fname = "test.wav" ;

	create_file (fname, SF_FORMAT_WAV | SF_FORMAT_PCM_16) ;

	read_file (fname) ;

	cout << "Done.\n" ;
	return 0 ;
} /* main */
