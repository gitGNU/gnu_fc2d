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

#include <utils/fevents.h>
#include <glib.h>

#if HAVE_X11
#include<X11/X.h>
#include<X11/Xlib.h>
#endif

void fevent_process( fEvent* evt ) {
	
}

void fevent_windowstep( fWindow* w ) {
//TODO: Support to no X11 events
#if HAVE_X11
	XEvent evt;
#endif
	
#if HAVE_X11
	XWindowEvent( w->display, w->window, KeyPressMask | KeyReleaseMask |
		ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
		ExposureMask, &evt);
#endif
	
	
}

void fevent_windowloop( fWindow* w ) {
	
}