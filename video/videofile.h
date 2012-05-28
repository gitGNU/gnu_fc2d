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

#ifndef __VIDEO_VIDEOFILE_H__
#define __VIDEO_VIDEOFILE_H__ 1

#include <config.h>

#if HAVE_VIDEO

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <glib.h>

extern gboolean videofilesystem_INITED;

typedef struct {
   AVFormatContext *format_ctx;
   AVStream* audio_stream;
   AVStream* video_stream;
   AVCodec* audio_codec;
   AVCodec* video_codec;
} fVideoFile;

inline void vf_init();
fVideoFile* vf_open(const char* name);


#else
#error Before install LIBAVCODEC and LIBAVFORMAT\
	you can NOT use this file.
#endif

#endif