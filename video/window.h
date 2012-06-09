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

#ifndef __VIDEO_WINDOW_H__
#define __VIDEO_WINDOW_H__ 1

#include <config.h>

#if HAVE_DISPLAY

#ifdef __cplusplus
extern "C" {
#endif

#if HAVE_SDL
#include <SDL.h>
#include <SDL_opengl.h>
#endif 

#if HAVE_X11
#include<X11/X.h>
#include<X11/Xlib.h>
#endif

#if HAVE_3D
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
#endif

#include <glib.h>
#include <high-level/threads.h>
#include <video/widgets.h>

extern GHashTable* f_getwindow;

struct fWindow;
typedef struct fWindow fWindow;

#if HAVE_X11

extern GHashTable* f_getdisplay;
extern Display* window_display_default;

Display* window_getdisplay( const char* name );
void window_deletedisplay( const char* name );

#if HAVE_3D
void window_set( fWindow* w );
void window_draw( fWindow* w );
#endif

#endif

struct fWindow {
	union {
		fWidget widget;
		struct {
			unsigned int x;
			unsigned int y;
			unsigned int width;
			unsigned int height;
		};
	};
#if HAVE_SDL
	SDL_Surface* screen;
#endif
	
#if HAVE_X11
	Display* display;
	Window window;
	GLXContext glc;
    XWindowAttributes gwa;
	int screen_num;
	XVisualInfo *vi;
	XSetWindowAttributes swa;
	Window root;
	Colormap cmap;	
#endif
	
	char* name;
	gboolean fullscreen;
	unsigned int bits;
	
};

#define window_new( x, y, bits, fullscreen ) \
	window_new_full( x, y, bits, fullscreen, NULL, NULL )

fWindow* window_get( const char* name );
fWindow* window_new_full( int x, int y, int bits, gboolean fullscreen, const char* display, const char* wname );
void window_free( fWindow* w );

/*!
 * \brief Set default window
 */
#define window_default( w ) \
    (f_data_connect( 0, "default-window", (w) ))

#ifdef __cplusplus
}
#endif

#else

#warning There was found no way to draw window. The include window.h has been disabled. \
	Install X11 or/and SDL(preferably both)

#endif

#endif