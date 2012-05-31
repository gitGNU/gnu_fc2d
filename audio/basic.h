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

#ifndef __AUDIO_BASIC_H__
#define __AUDIO_BASIC_H__ 1

#include <config.h>

#include <glib.h>

#define SFT(x) \
	((x) * 44100)
	
#define SR \
	((float)(44100))
	
#define FSIN( freq, amplitude ) \
	(sin(freq) * amplitude)

#define FCOS( freq, amplitude ) \
	(cos(freq) * (amplitude))

#define WAVE_WINDOW_SIZE (SR/60)

#define FREQ_A  (264/2)
#define FREQ_B  (297/2)
#define FREQ_C  (330/2)
#define FREQ_D  (352/2)
#define FREQ_E  (396/2)
#define FREQ_F  (440/2)
#define FREQ_G  (495/2)

/*! \brief Musical Instrument*/
typedef enum {
	PIANO1,
	CONSTANT1,
	PERCUSSION1,
	BASS1,
	BASS1_PERCUSSION
} fInstrument;

typedef enum { 
	A, B, C, D, E, F,
	G, DO=C, RE=D,
	MI=E, FA=F, SOL=G,
	LA=A, SI=B
} fMusicalNote;

float wave_df( float* a, guint t, guint d );

void wave_normalize( float* buf, unsigned int samples );

/*! \brief Get the approximate frequency in Hz */
float wave_frequency( float* buf );

/*! \brief Try to synthesizes the sound of a
           musical instrument
    \param buf For the result
    \param sample Number of samplesof this sound
    \param freq Frequence of sound(Hz)
    \param tr The musical instrument to synthesize
    \param a Wave amplitude
    \
*/
void wave_synthesize( float* buf, guint samples, 
					  float freq, fInstrument tr,
					  float a);

/*! \brief Synthesizes the sound of an instrument
 *         from the input buffer
 *  \details Approximates the values ​​for
 *           musical notes between(inclusive)
 *           263Hz and 493Hz(without sharps)
 * 
 * \param voice The input buffer( may be your own voice )
 * \param dest The buffer to plot the samples
 * \param tr The musical instrument to synthesize
 * \param samples The total number of samples of your melody
 */
void wave_resynthesize(float* voice, float** dest, 
					   fInstrument tr, guint samples );

float* wave_harmonics( float* buf );

float freq_refine( guint f );

float freq_fromnote( fMusicalNote note, guint n );
float freq_fromnote2( guint n );
#endif