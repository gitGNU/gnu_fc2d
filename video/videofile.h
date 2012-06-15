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

/*!
 * \file video/videofile.h
 * \details This file allows you to read
 *          and write images, videos and
 *          audio from a practical, easy
 *          and quick way.
 */

#ifndef __VIDEO_VIDEOFILE_H__
#define __VIDEO_VIDEOFILE_H__ 1

#include <config.h>

#if HAVE_VIDEO

#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <video/image.h>
#include <glib.h>

extern gboolean videofilesystem_INITED;

typedef struct {
    float* audio;
    guint samples;
    fImage* video;
} fVideoAudio;

typedef struct {
   struct SwsContext* sws_ctx;
   AVFormatContext* format_ctx;
   AVStream* audio_stream;
   AVStream* video_stream;
   AVCodec* audio_codec;
   AVCodec* video_codec;
   AVCodecContext* video_ctx;
   AVCodecContext* audio_ctx;
   int audio_index;
   int video_index;
   AVPicture pict;
   char* name;
   fImage* img;
   fVideoAudio fva;
   AVFrame* frame;
} fVideoFile;

typedef struct fVideoFile fMediaFile;

inline void vf_init();

/*!
 * \brief Opens a file of image,
 *        video and/or audio 
 */
fVideoFile* vf_open( const char* name );

fVideoFile* vf_audio_new( const char* name, int cid,
                          guint channels );

fVideoFile* vf_video_new( const char* name, int cid,
						  guint width, guint height,
                          guint fps );

float* vf_audio_read( fVideoFile* vf, guint samples );

void vf_audio_write( fVideoFile* vf, float* data,
                     guint samples );

fImage* vf_video_read( fVideoFile* vf );

void vf_video_write(fVideoFile* vf, fImage* img);


/*! 
 * \brief Reads the next frame of video,
 *        and/or audio and returns it.
 *        It can be used to read image
 *        files(like JPEG).
 * 
 * \warning The file can only be read by
 *          one thread at a time.
 */
fVideoAudio* vf_read( fVideoFile* vf );

#else
#error Before install LIBAVCODEC and LIBAVFORMAT\
	you can NOT use this file.
#endif

#endif