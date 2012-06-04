/*
FDiamondEngine - Complete engine for 3D games development.
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

#if HAVE_ALSA

#include <audio/alsa.h>
#include <stdio.h>



audio_loop* audio_input_loop = NULL;
audio_loop* audio_output_loop = NULL;

GMutex* aud_mutex = NULL;
fThread* aud_fthread = NULL;

int get_audio( alsa_audio* audio ) {

	snd_pcm_hw_params_t *hw_params;
	int err;

	if( audio == NULL ) {
		fprintf(stderr, "It is necessary to specify any argument to the function: 'get_audio'.\n"
		" If you do not want that, then use 'get_audio_output' or 'get_audio_input'.\n");
		return -1;
	}
	
	if( audio->handle != NULL )
		return 0;

	if ((err = snd_pcm_open (&(audio->handle), audio->out_name, audio->stream, 0)) < 0) {
		fprintf (stderr, "cannot open audio device %s (%s)\n", 
 			 audio->out_name,
			 snd_strerror (err));
		return -1;
	}
	   
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
			 snd_strerror (err));
		return -1;
	}
			 
	if ((err = snd_pcm_hw_params_any (audio->handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_access( audio->handle, hw_params, audio->access)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_format ( audio->handle, hw_params, audio->format)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_rate_near ( audio->handle, hw_params, &(audio->rate), 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params_set_channels ( audio->handle, hw_params, audio->channels)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	if ((err = snd_pcm_hw_params ( audio->handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		return -1;
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (audio->handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		return -1;
	}
	
	return 0;
	
}

alsa_audio* get_audio_output() {
	static alsa_audio* audio = NULL;
	
	if( audio != NULL )
		return audio;
	
	audio = g_try_new0( alsa_audio, 1 );
	
	audio_output_loop = g_try_new0( audio_loop, 1 );
	audio_output_loop->alsa = audio;
	
	audio->stream = SND_PCM_STREAM_PLAYBACK;
	audio->access = SND_PCM_ACCESS_RW_INTERLEAVED;
	audio->channels = 2;
	audio->format = SND_PCM_FORMAT_FLOAT_LE;
	audio->out_name = "default";
	audio->rate = 44100;
	
	if( get_audio( audio ) < 0 ) {
		g_free(audio);
		return NULL;
	} else
		return audio;
	
}
alsa_audio* get_audio_input() {
	static alsa_audio* audio = NULL;
	
	if( audio != NULL )
		return audio;
	
	audio = g_try_new0( alsa_audio, 1 );
	
	audio_input_loop = g_try_new0( audio_loop, 1 );
	audio_input_loop->alsa = audio;
	
	audio->stream = SND_PCM_STREAM_CAPTURE;
	audio->access = SND_PCM_ACCESS_RW_INTERLEAVED;
	audio->channels = 1;
	audio->format = SND_PCM_FORMAT_FLOAT_LE;
	audio->out_name = "default";
	audio->rate = 44100;
	
	if( get_audio( audio ) < 0 ) {
		g_free(audio);
		return NULL;
	} else
		return audio;
}

void audio_output_mainloop() {
	alsa_audio* audio;
	GList* l;
	AudioBuffer* buf = NULL;

	audio = get_audio_output();

	audio_output_loop->l = &l; 
	
	while(1) {
		waits(1);
		if(  l != NULL ) {
			buf = l->data;
			l->data = NULL;
		}
		l = g_list_delete_link( g_list_first(l), l );
		wait(1);
		if( buf != NULL )
			audio_write( buf->data, buf->samples/2 );
		
		g_free( buf );
		buf = NULL;
		
		wait(1);
	}
}

void audio_mainloop() {

	aud_fthread = thsyshash_get();
	aud_mutex = g_mutex_new();

	thsys_addp( audio_output_mainloop, NULL );
}

void psamplei( float sample, float balance ) {
	float s[2];
	float b[2];
	GList** l;
	AudioBuffer* buf;
	
	buf = g_malloc( sizeof(buf) );
	
	if( balance >= 0 ) {
		b[1] = balance;
		b[0] = 1 - balance;
	} else {
		balance += 1.f;
		b[0] = balance;
		b[1] = 1 - balance;
	}
	
	s[0] = sample * b[0];
	s[1] = sample * b[1];
	
	waits(1);
	l = audio_output_loop->l;
	(*l)->data = buf;
	buf->data = &(s[0]);
	buf->samples = 2;
	
	wait(2);
}

void audio_read( float* buf, unsigned int samples  ) {
	alsa_audio* a;
	int err;
	float* end;
	
	a = audio_input_loop->alsa;
	
	end = &buf[samples];
	
	for( ; buf < end; buf+=128 ) {
		if( end - buf > 128 ) {
			if ( (err = snd_pcm_readi( a->handle, buf, 128 )) 
				!= 128 )
				fprintf (stderr, "read audio failed (%s)\n",
					snd_strerror (err));
		} else
			if ( (err = snd_pcm_readi( a->handle, buf, end - buf )) 
				!= end - buf )
				fprintf (stderr, "read audio failed (%s)\n",
					snd_strerror (err));
	}
}

void audio_write( float* buf, unsigned int frames ) {
	alsa_audio* a;
	int err;
	float* end;
	
	a = audio_output_loop->alsa;
	
	end = &buf[frames];
	
	for( ; buf < end; buf+=128 ) {
		if( end - buf > 128 ) {
			if ( (err = snd_pcm_writei( a->handle, buf, 128 )) 
				!= 128 )
				fprintf (stderr, "write audio failed (%s)\n",
					snd_strerror (err));
		} else
			if ( (err = snd_pcm_writei( a->handle, buf, end - buf )) 
				!= end - buf )
				fprintf (stderr, "write audio failed (%s)\n",
					snd_strerror (err));
	}
}

#endif