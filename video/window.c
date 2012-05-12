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

#include "window.h"
#include <glib.h>

fWindow* window_new( int x, int y, int bits, gboolean fullscreen ) {
	fWindow* w = g_try_new0( fWindow, 1 );
	int SDL_flags;
	
	w->width = x;
	w->height = y;
	w->bits = bits;
	w->fullscreen = (fullscreen != FALSE)? TRUE: FALSE;
	
	if( w->fullscreen )
		SDL_flags |= SDL_FULLSCREEN;
	
	w->screen = SDL_SetVideoMode( x, y, bits, SDL_SWSURFACE | SDL_OPENGL | SDL_flags );
	
	glEnable( GL_TEXTURE_2D );
 
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
 
	glViewport( 0, 0,  w->width, w->height );
 
	glClear( GL_COLOR_BUFFER_BIT );
 
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
 
	glOrtho(0.0f, w->width, w->height, 0.0f, -1.0f, 1.0f);
 
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	return w;
}

void window_free( fWindow* w ) {
	
	SDL_Quit();
	
	g_free(w);
}