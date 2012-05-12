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

#ifndef __VIDEO_WINDOW_H__
#define __VIDEO_WINDOW_H__ 1

#include <SDL.h>
#include <SDL_opengl.h>
#include <glib.h>

typedef struct {
	SDL_Surface* screen;
	gboolean fullscreen;
	unsigned int width;
	unsigned int height;
	unsigned int bits;
	
} fWindow;

fWindow* window_new( int x, int y, int bits, gboolean fullscreen );
void window_free( fWindow* w );

#endif