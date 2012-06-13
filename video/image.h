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

#ifndef __VIDEO_IMAGE_H__
#define __VIDEO_IMAGE_H__ 1

#include <glib.h>

#define color(image, x, y, c) \
    ((float*)(((gulong)((image)->data)) + \
    (gulong)((((((fImageHeader*)(image))->width) *\
    (y) * (((fImageHeader*)(image))->channels_num)) + \
    ((x) * (((fImageHeader*)(image))->channels_num)) + \
    c) * sizeof(float))))\

#define pixel( image, x, y, c ) \
    (color(image, x, y, c)[0])

typedef struct {
	union {
		struct {
			float r;
			float g;
			float b;
		};
		struct {
			float red;
			float green;
			float blue;
		};
		float c[3];
	};
} fColor;

typedef struct {
	guint8 header_size;
	guint32 width;
	guint32 height;
	guint16 emitter_len;
	guint8 channels_num; /*!< Channels Number */
	guint16 color_size;
} fImageHeader; 

typedef struct {
	fImageHeader header;
	char* emitter;
	float* data;
} fImage;

fImage* image_new( guint32 width, guint32 height, gboolean alpha );
fImage* image_load( const char* filename );
gboolean image_save( const char* filename, fImage* img );

#endif