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

#define WAVE_WINDOW_SIZE 60

/*! \brief Musical Instrument*/
typedef enum {
	PIANO,
	VIOLIN,
	FLUTE
} fInstrument;

float wave_df( float* a, guint t, guint d );

void wave_normalize( float* buf, unsigned int samples );

/*! \brief Get frequency in hertz */
float wave_frequency( float* buf );

/*! \brief Synthesizes the sound of a
           musical instrument */
void wave_synthesize( float* buf, guint samples, 
					  guint freq, fInstrument tr );


#endif