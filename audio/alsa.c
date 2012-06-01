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
	audio->out_name = "pulse";
	audio->rate = 44100;
	
	if( get_audio( audio ) < 0 ) {
		g_free(audio);
		return NULL;
	} else
		return audio;
}

void audio_output_mainloop() {
	alsa_audio* audio;
	int rest;
	guint16* buf;
	int err;
	
	audio = get_audio_output();
	
	buf = audio_input_loop->samples;
	
	while(1) {
		
		rest = ((int)(audio_output_loop->write) - audio_output_loop->read);
		if( rest < 0 )
			rest = (65536 - audio_output_loop->read) +
			audio_output_loop->write;
		
		if( rest >= 1024 ) {
			if ( (err = snd_pcm_writei(audio->handle, &(buf[audio_output_loop->read]), 512)) != 512) {
				fprintf (stderr, "write to audio interface failed (%s)\n",
					snd_strerror (err));
				break;
			}

			audio_output_loop->read += 1024;
			
		} else if( rest > 0 ) {
			if ( (err = snd_pcm_writei(audio->handle, &(buf[audio_output_loop->read]), rest/2)) != rest/2) {
				fprintf (stderr, "write to audio interface failed (%s)\n",
					snd_strerror (err));
				break;
			}

			audio_output_loop->read += rest;
			audio_output_loop->readed = TRUE;
		}
		wait(1);
	}
}

void audio_input_mainloop() {
	alsa_audio* audio;
	
	audio = get_audio_input();
	
	while(1) {
		wait(1);
	}
}

void audio_mainloop() {
	
	aud_fthread = thsyshash_get();
	
	aud_mutex = g_mutex_new();
	
	/* The function does not return, to avoid the
	 * automatic termination of the 'fThread' */
	wait(0);
}

void psamplei( guint16 sample, float balance ) {
	
	CFTHREAD = aud_fthread;
	
	if( aud_fthread == NULL )
		return;
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