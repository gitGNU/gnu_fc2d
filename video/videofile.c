/*
FGameEngine - Complete tool kit for 3D games development.
Copyright (C) 2012  Fabio J. Gonzalez

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

#if HAVE_VIDEO

#include <video/videofile.h>

gboolean videofilesystem_INITED = FALSE;

inline void vf_init() {
	if( videofilesystem_INITED )
		return;
	
	av_register_all();
	
	videofilesystem_INITED = TRUE;
	
}
fVideoFile* vf_open(const char* name) {
	fVideoFile* video;
	int err;
	char buf[256];
	int i;
	AVCodecContext* c;
	
	video = g_malloc( sizeof(fVideoFile) );
	
	err = avformat_open_input( &(video->format_ctx), name, NULL, NULL );
	if( err < 0 )
		goto error;
	
	err = avformat_find_stream_info( video->format_ctx, NULL);
	if( err < 0 )
		goto error;
	
	for( i = 0; i < (video->format_ctx)->nb_streams; i++ ) {
		c = (video->format_ctx)->streams[i];
		if( c->codec_type == AVMEDIA_TYPE_AUDIO ) {
			video->audio_stream = c;
			video->audio_codec = avcodec_find_decoder(c->codec_id);
		}
		else if( c->codec_type == AVMEDIA_TYPE_VIDEO ) {
			video->video_stream = c;
			video->video_codec = avcodec_find_decoder(c->codec_id);
		}
	}
	
	avcodec_open2( video->video_stream, video->video_codec, NULL);
	avcodec_open2( video->audio_stream, video->audio_codec, NULL);
	
	return video;
	
	error:
	g_free(video);
	return NULL;
}

#endif