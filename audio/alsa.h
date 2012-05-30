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

#ifndef __AUDIO_ALSA_H__
#define __AUDIO_ALSA_H__ 1

#include <alsa/asoundlib.h>
#include <high-level/threads.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define psamplef( sample, balance ) \
	psamplei( ((float)(sample)) * 0xffff, balance )

typedef struct {
	snd_pcm_t* handle;
	snd_pcm_stream_t stream;
	snd_pcm_access_t access;
	snd_pcm_format_t format;
	unsigned int rate;
	unsigned int channels;
	char* out_name;
} alsa_audio;

typedef struct {
	alsa_audio* alsa;
	guint16 samples[65535];
	guint16 read;
	guint16 write;
	gboolean readed;
} audio_loop;

extern audio_loop* audio_input_loop;
extern audio_loop* audio_output_loop;
extern GMutex* aud_mutex;
extern fThread* aud_fthread;

int get_audio( alsa_audio* audio );

alsa_audio* get_audio_output();
alsa_audio* get_audio_input();

void audio_output_mainloop();
void audio_input_mainloop();
void audio_mainloop();

void psamplei( guint16 sample, float balance );

void audio_read( float* buf, unsigned int samples );
void audio_write( float* buf, unsigned int frames );

#ifdef __cplusplus
}
#endif

#endif