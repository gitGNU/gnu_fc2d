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

#include <glib.h>
#include <video/window.h>
#include <utils/events.h>

#if HAVE_DISPLAY

G_LOCK_DEFINE(window_new);
G_LOCK_DEFINE(window_get);

GHashTable* f_getwindow = NULL;

#if HAVE_X11

#if HAVE_3D
	GLint gsm[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
#endif

Display* window_display_default = NULL;
GHashTable* f_getdisplay = NULL;

Display* window_getdisplay( const char* name ) {
	GHashTable** hash = &f_getdisplay;
	Display** display = NULL;
	
	G_LOCK_DEFINE(window_get);
	
	if( *hash == NULL ) {
		*hash = g_hash_table_new( g_str_hash, g_str_equal );
		XInitThreads();
	}
	if( name != NULL )
		display = g_hash_table_lookup ( *hash, name );
	
	if( display == NULL ) {
		display = g_malloc( sizeof( Display* ) );
		
		*display = XOpenDisplay(name);
		
		if( name != NULL )
			g_hash_table_insert( *hash, name, display );
		
		if( window_display_default == NULL || name == NULL )
			window_display_default = *display;

	}
	
	G_UNLOCK(window_get);
	
	return *display;
}


void window_deletedisplay( const char* name ) {
	GHashTable** hash = &f_getdisplay;
	Display** display = NULL;
	
	if( *hash == NULL )
		return;
	
	if( name != NULL )
		display = g_hash_table_lookup ( *hash, name );
	else {
		display = &window_display_default;
	}
	
	if( display != NULL ) {

#if HAVE_3D
		glXMakeCurrent(*display, None, NULL);
#endif
		XCloseDisplay(*display);
		
		g_hash_table_remove( *hash, name );
		g_free( display );
	}
	
}

#if HAVE_3D
void window_set( fWindow* w ) {
	glXMakeCurrent(w->display, w->window, w->glc);
}

void window_draw( fWindow* w ) {
	glXSwapBuffers(w->display, w->window);
}


#endif

#endif 

fWindow* window_new_full( int x, int y, int bits,
						  gboolean fullscreen, const char* display,
						  const char* wname )
{
	fWindow* w = window_get(wname);
	int flags;
	GThread* th;
	fThread* this;
	
	G_LOCK(window_new);
#if !HAVE_X11
	SDL_Init(SDL_INIT_VIDEO);
#else
	w->display = window_getdisplay(display);
	w->screen_num = DefaultScreen(w->display);
	w->root = RootWindow( w->display, w->screen_num);
#endif

	w->width = x;
	w->height = y;
	w->bits = bits;
	w->fullscreen = (fullscreen != FALSE)? TRUE: FALSE;
	
#if !HAVE_X11
	if( w->fullscreen )
		flags |= SDL_FULLSCREEN;
	w->screen = SDL_SetVideoMode( x, y, bits, SDL_OPENGL | flags );
#else
	
#if HAVE_3D
	w->vi = glXChooseVisual( w->display, 0, gsm );
	
	w->cmap = XCreateColormap(w->display, w->root, w->vi->visual, AllocNone);
	w->swa.colormap = w->cmap;
	w->swa.event_mask = ExposureMask | KeyPressMask;
	 
	w->window = XCreateWindow(w->display, w->root,
						0, 0, w->width, w->height, 0, w->vi->depth, InputOutput, w->vi->visual, CWColormap | CWEventMask, &(w->swa));
	XMapWindow(w->display, w->window);
#else
	//TODO: Support to work without GL and GLU
#warning Work without GL and GLU is not supported yet. \
	Install GL and GLU and recompile FGameEngine
#endif
	
#endif
		
#if HAVE_X11
	//TODO: We avoid calling this function all the time?
	XAllowEvents( w->display, GrabModeAsync, CurrentTime );
#endif
	G_UNLOCK(window_new);
	
#if HAVE_X11
	
#if HAVE_3D
	this = thsyshash_get();
	//thsys_addleaving( this, window_draw, w );
#endif
	this->child_coming = NULL;
	this->child_leaving = NULL;
	
	thsys_add( fevent_windowloop, w );

#endif
	
	return w;
}

void window_free( fWindow* w ) {
	
#if !HAVE_X11
	SDL_Quit();
#else
	/* TODO: Check for error and remove this 
	 * text if no one
	 */
#if HAVE_3D
 	glXDestroyContext(w->display, w->glc);
#endif
 	XDestroyWindow(w->display, w->window);

#endif
	
	g_free(w);
}

fWindow* window_get( const char* name ) {
	GHashTable** hash = &f_getwindow;
	fWindow** w = NULL;
	
	if( *hash == NULL )
		*hash = g_hash_table_new( g_str_hash, g_str_equal );
	
	if( name != NULL )
		w = g_hash_table_lookup ( *hash, name );
	
	if( w == NULL ) {
		w = g_malloc( sizeof( fWindow* ) );
		*w = g_try_new0( fWindow, 1 );
		if( name != NULL )
			g_hash_table_insert( *hash, name, w );
	}
	
	(*w)->name = name;
	
	return *w;
}

#endif