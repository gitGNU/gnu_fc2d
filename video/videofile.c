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
    AVStream* s;
	AVCodecContext* c;
    
	video = g_malloc0( sizeof(fVideoFile) );
	
	video->name = name;
	
    vf_init();
    
	err = avformat_open_input( &(video->format_ctx), name,
                               NULL, NULL );
	if( err < 0 )
		goto error;
	
	err = avformat_find_stream_info( video->format_ctx, NULL);
	if( err < 0 )
		goto error;
	
	for( i = 0; i < (video->format_ctx)->nb_streams; i++ ) {
		s = (video->format_ctx)->streams[i];
        c = s->codec;
		if( c->codec_type == AVMEDIA_TYPE_AUDIO ) {
            video->audio_index = i;
			video->audio_stream = s;
			video->audio_codec = 
			avcodec_find_decoder(c->codec_id);
            video->audio_ctx = c;
		}
		else if( c->codec_type == AVMEDIA_TYPE_VIDEO ) {
            video->video_index = i;
			video->video_stream = s;
			video->video_codec =
			avcodec_find_decoder(c->codec_id);
            video->video_ctx = c;
		}
	}

	if( video->video_codec ) {
		avcodec_open2( video->video_ctx,
                       video->video_codec, NULL);
    }
    if( video->audio_codec ) 
		avcodec_open2(
		video->audio_ctx,
        video->audio_codec, NULL);

	return video;
	
	error:
	g_free(video);
	return NULL;
}

#if 0
fVideoFile* vf_audio_new( const char* name, int cid,
                          guint channels ) 
{
	fVideoFile* video;
	int err;
	char buf[256];
	int i;
	AVCodecContext* c;
	AVCodec* codec;

	video = g_malloc0( sizeof(fVideoFile) );

	if( cid == 0 )
		cid = CODEC_ID_VORBIS;
	
	c = avcodec_find_encoder(cid);

	codec = avcodec_alloc_context();
	
	codec->sample_rate = 44100;
	codec->bit_rate = 64000;
	codec->channels = channels;
	video->name = name;
	
	err = avcodec_open(codec, c);
	if( err < 0 )
		goto error;
	
	return video;
	
	error:
	g_free(video);
	return NULL;
}
#endif

fVideoFile* vf_video_new( const char* name, int cid,
                          guint width, guint height,
                          guint fps ) 
{
    
}

float* vf_audio_read( fVideoFile* vf, guint samples )
{
    
}

void vf_audio_write( fVideoFile* vf, float* data,
                     guint samples ) 
{
    
}

fImage* vf_video_read( fVideoFile* vf ) 
{
    
}

void vf_video_write(fVideoFile* vf, fImage* img)
{
    
}

fVideoAudio* vf_read( fVideoFile* vf ) {
    fVideoAudio* ret;
    AVPacket packet;
    AVFrame* frame;
    int i=0;
    guint x, y;
    float* f;
    guint32* f2;
    
    ret = g_malloc( sizeof(fVideoAudio) );
    frame = avcodec_alloc_frame();
    
    
    while(av_read_frame( vf->format_ctx, &packet) >= 0) {
        
        if( packet.stream_index == vf->video_index ) {
            avcodec_decode_video2( vf->video_ctx, frame, &i,
                                &packet);
            
            if( i != 0 ) {
                ret->video = image_new(vf->video_ctx->width,
                                      vf->video_ctx->height,
                                      FALSE );
                
                for( y = 0; y < vf->video_ctx->height; y++ )
                    for( x = 0; x < vf->video_ctx->width; x++ ) {
                        for( i = 0; i < 3; i++ ) {
                            f = color(ret->video, x, y, i);
                            f2 = ((guint32*)((gulong)frame->data[0] +
                            y*frame->linesize[0] + (x*3)+i));
                            /*Set the first 8 bits to 0*/
                            *f2 = (*f2) << 8;
                            *f2 = (*f2) >> 8;
                            *f = (float)(*f2) / 16777215; 
                        }
                            
                    }
                    
                    break;
            }
        } else if( packet.stream_index == vf->audio_index ) {
            
        }
    }
    
    return ret;
}

#endif