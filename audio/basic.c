/*
FGameEngine - Complete tool kit for 3D games development.
Copyright (C) 2012  Fabio J. Gonzalez <fabiojosue@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <config.h>

#include <audio/basic.h>
#include <stdio.h>

float wave_df( float* a, guint t, guint d ) {
	int j;
	float x=0;

	for( j = t; j < (t + (WAVE_WINDOW_SIZE/2)); j++ )
		x += ( ( a[j] - a[j+d] ) * ( a[j] - a[j+d] ) );

	return x / WAVE_WINDOW_SIZE;
}

void wave_normalize( float* buf, unsigned int samples ) {
	guint i;
	float med = 0;
	float max=-1;
	
	/*Set wave center to 0*/
	for( i = 0; i < samples; i++ )
		med += buf[i];
	
	med /= samples;
	
	for( i = 0; i < samples; i++ ) {
		buf[i] -= med;
		if( buf[i] > max )
			max = buf[i];
	}
	
	fprintf(stderr, "%f\n", max);
	for( i = 0; i < samples; i++ )
		buf[i] *= (1. / max);
}

float wave_frequency( float* buf ) 
{
	guint i;
	float t;
	
	for( i = 40; i < (WAVE_WINDOW_SIZE/2); i++ ) {
		t = wave_df( buf, 0, i );
		if( t < 0.01 )
			return 44100 / i;
	}
}

void wave_synthesize( float* buf, guint samples,
					  guint freq, fInstrument tr ) 
{
	
}

