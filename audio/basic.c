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
#include <math.h>

float wave_df( float* a, guint t, guint d ) {
	int j;
	float x=0;

	for( j = t; j < (t + WAVE_WINDOW_SIZE); j++ )
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
	
	for( i = 0; i < samples; i++ )
		buf[i] *= (1. / max);
}

float wave_frequency( float* buf ) 
{
	guint i, j, j2;
	float t=0, t2=0;
	gboolean stop = FALSE;
	
	for( i = 40; i < WAVE_WINDOW_SIZE; i++ ) {
		t = wave_df( buf, 0, i );

		if( t < 0.05 ) {
			if( t < t2 )
				stop = TRUE;
	
			if( t > t2 && stop == TRUE )
				return SR / i;
		}
		t2 = t;
		
	}
	
	return 0;
}

void wave_synthesize( float* buf, guint samples,
					  float freq, fInstrument tr,
					  float a ) 
{
	float s=0, e=0, m=1;
	guint i, j;
	float f;
	
	f = ((M_PI*2) / SR) * freq;

	if( tr == PIANO1 ) {
		for( i = 0; i < samples; i++, e+=f ) {
			s = FSIN(e, 1) + FSIN(e*2, 0.2) +
			FSIN(e*3, 0.15) + FCOS(e*6, 0.12)
			+ FCOS(e*7, 0.13) + FSIN(e*8, 0.05);
			buf[i] = (float)((s/1.65) * (1./m) * a);
			m += (3. /SR);
		}
	} else if( tr == CONSTANT1 ) {
		for( i = 0; i < samples; i++, e+=f ) {
			s = FSIN(e, 1) + FCOS(e*6, 0.02) +
			FCOS(e*3, 0.5) + FSIN(e*4, 0.3) +
			FSIN(e*9, 0.2);
			buf[i] += (float)(s/2.02) * a;
		}
	} else if( tr == PERCUSSION1 ) {
		for( i = 0; i < samples; i++, e+=f ) {
			s = FSIN(e, 1) + FCOS(e*6, 0.02) +
			FCOS(e*3, 0.5) + FSIN(e*4, 0.3) +
			FSIN(e*9, 0.2);
			buf[i] += (float)(s/2.02) * a * (1./m);
			m += (10. /SR);
		}
	} else if( tr == BASS1 ) {
		for( i = 0; i < samples; i++, e+=f ) {
			s = FSIN(e, 1) + FCOS(e*2, 0.01) +
			FCOS(e*3, 0.07) + FSIN(e*4, 0.03) +
			FSIN(e*5, 0.02);
			buf[i] += (float)(s/2.02) * a;
		}
	} else if( tr == BASS1_PERCUSSION ) {
		for( i = 0; i < samples; i++, e+=f ) {
			s = FSIN(e, 1) + FCOS(e*2, 0.01) +
			FCOS(e*3, 0.07) + FSIN(e*4, 0.03) +
			FSIN(e*5, 0.02);
			buf[i] += (float)(s/2.02) * a * (1./m);
			m += (10. /SR);
		}
	}
	
}

void wave_resynthesize(float* voice, float** dest, 
					   fInstrument tr, guint samples) 
{
	guint i=0, h=0;
	guint j=0, m=0;
	float old=0;
	float eold=0;
	float f=0, g, l, f2;
	const guint w = WAVE_WINDOW_SIZE;

	if( *dest == NULL )
		*dest = g_malloc0( samples * sizeof(float) );

	for( i = 0; i < samples; i+=j ) {
		for( j = w; j+i < samples; j+= w) {
			wave_normalize(&(voice[i+j]), w);
			f2 = wave_frequency(&(voice[i+j]));
			if( (f2-g)*(f2-g) < 100 )
				f = freq_refine( g );
			else {
				f = freq_refine( f2 );
				g = f2;
			}
			//f = f2;
			if( f >= 1000 || f2 == 0 )
				f = old;
			
			if( old != f )
				break;
			
		}
		
		if( old != 0)  {
			wave_synthesize( &((*dest)[i]), j, old, tr, 1 );
			fprintf(stderr, "%f\n", old);
		}
		
		old = f;
	}
	
}

float* wave_harmonics( float* buf ) {
	guint i, j;
	float* ret;
	float freq, f, e=0;
	
	ret = g_malloc( sizeof(float) * 40 );
	freq = wave_frequency( buf );
	
	f = ((M_PI*2) / SR) * freq;
	
	for( j = 0; j < 20; j++ ) {
		ret[j] = 0;
		ret[j+20] = 0; 
		for( i = 0; i < WAVE_WINDOW_SIZE; i++, e+=f ) {
			ret[j] += ( buf[i] * sin(e*(j-19)) );
			ret[j+20] += ( buf[i] * cos(e*(j-19)) );
		}
		
		ret[j] /= WAVE_WINDOW_SIZE;
	}

	return ret;
}

float freq_refine( guint f ) {
	guint n;
		
	for( n = 0; freq_fromnote2(n) < f; n++ );
	
	return freq_fromnote2(n);
}

float freq_fromnote( fMusicalNote note, guint n ) {
	if( note == A )
		return FREQ_A*n;
	else if( note == B )
		return FREQ_B*n;
	else if( note == C )
		return FREQ_C*n;
	else if( note == D )
		return FREQ_D*n;
	else if( note == E )
		return FREQ_E*n;
	else if( note == F )
		return FREQ_F*n;
	else if( note == G )
		return FREQ_G*n;

	return 0;
}

float freq_fromnote2( guint n ) {
	guint i;
	fMusicalNote nt;
	
	nt = n % 7;
	
	return freq_fromnote( nt, (n / 7)+1 );
}