/*
FGameEngine - Complete tool kit for 3D games development.
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

#include <video/render.h>
#include <video/window.h>
#include <high-level/threads.h>

void RenderScene( fWindow* w ) {
	
}

void RenderGUI( fWindow* w ) {
	
}

void Render( fWindow* w ) {
	w->glc = glXCreateContext(w->display, w->vi, NULL, GL_TRUE);
	
	window_set(w);
	
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	
	while(1) {
		
 		glViewport( 0, 0,  w->width, w->height );
 	
 		glClear( GL_COLOR_BUFFER_BIT );
 	
 		glMatrixMode( GL_PROJECTION );
 		glLoadIdentity();
 	
 		glOrtho(0.0f, w->width, w->height, 0.0f, -1.0f, 1.0f);
 	
 		glMatrixMode( GL_MODELVIEW );
 		glLoadIdentity();
		
		RenderScene(w);
		RenderGUI(w);
		window_draw(w);
		wait(1);
	}
}