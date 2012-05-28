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

#include <utils/events.h>
#include <utils/event-basis.h>
#include <video/window.h>
#include <video/render.h>
#include <video/widgets.h>
#include <glib.h>
#include <high-level/threads.h>

#if HAVE_X11
#include<X11/X.h>
#include<X11/Xlib.h>
#endif

gboolean fevent_process( fEvent* evt ) {
	fWidget* widget;
	fWidget* widget2;
	GList* l;
	
	if( evt->obj_type == TYPE_FWIDGET ) {
		widget = evt->obj;
		
		if( evt->x > widget->x &&
			evt->x <= widget->x &&
			evt->y > widget->y &&
			evt->y <= widget->y
		) {
			for( l = widget->childs; l != NULL; l = l->next ) {
				widget2 = l->data;
				
				if( evt->x > widget2->x &&
					evt->x <= widget2->x &&
					evt->y > widget2->y &&
					evt->y <= widget2->y
				) 
					f_signal_emit_full( widget2, evt->name, evt);
			}
			
			return TRUE;
		}
		
	}
	
	return FALSE;
}

fEvent* fevent_windowstep( fWindow* w ) {
//TODO: Support to no X11 events
	static fEvent* fevt = NULL;
	
	if( fevt == NULL )
		fevt = g_malloc0( sizeof(fEvent) );
	
	fevt->obj = w;
	
#if HAVE_X11
	XEvent evt;
	
	XWindowEvent( w->display, w->window, KeyPressMask | KeyReleaseMask |
		ButtonPressMask | ButtonReleaseMask | PointerMotionMask | 
		ButtonMotionMask| ExposureMask, &evt);
	
	if( evt.type == Expose ) {
		fevt->id = FEXPOSE_EVENT;
		FEXPOSEEVENT(fevt)->x = evt.xexpose.x;
		FEXPOSEEVENT(fevt)->y = evt.xexpose.y;
		FEXPOSEEVENT(fevt)->width = evt.xexpose.width;
		FEXPOSEEVENT(fevt)->height = evt.xexpose.height;
		fevt->name = "expose-event";
		f_signal_emit_full(w, "expose-event", fevt);
		fevent_process( w );
	} if( evt.type == MotionNotify ) {
		fevt->id = FMOUSE_EVENT;
		FMOUSEEVENT(fevt)->x = evt.xmotion.x;
		FMOUSEEVENT(fevt)->y = evt.xmotion.y;
		FMOUSEEVENT(fevt)->x_root = evt.xmotion.x_root;
		FMOUSEEVENT(fevt)->y_root = evt.xmotion.y_root;
		FMOUSEEVENT(fevt)->button = FBUTTON_NONE;
		fevt->name = "mouse-motion-event";
		f_signal_emit_full(w, "mouse-motion-event", fevt);
		fevent_process( w );
	} else if( evt.type == KeyPress ) {
		fevt->id = FKEY_EVENT;
		FKEYEVENT(fevt)->state = FKEY_DOWN;
		FKEYEVENT(fevt)->keycode = evt.xkey.keycode;
		FKEYEVENT(fevt)->x = evt.xkey.x;
		FKEYEVENT(fevt)->y = evt.xkey.y;
		fevt->name = "key-event";
		f_signal_emit_full(w, "key-event", fevt);
		fevent_process( w );
	} else if( evt.type == KeyRelease ) {
		fevt->id = FKEY_EVENT;
		FKEYEVENT(fevt)->state = FKEY_UP;
		FKEYEVENT(fevt)->keycode = evt.xkey.keycode;
		FKEYEVENT(fevt)->x = evt.xkey.x;
		FKEYEVENT(fevt)->y = evt.xkey.y;
		fevt->name = "key-event";
		f_signal_emit_full(w, "key-event", fevt);
		fevent_process( w );
	} else if( evt.type == ButtonPress ) {
		fevt->id = FMOUSE_EVENT;
		FMOUSEEVENT(fevt)->x = evt.xbutton.x;
		FMOUSEEVENT(fevt)->y = evt.xbutton.y;
		FMOUSEEVENT(fevt)->x_root = evt.xbutton.x_root;
		FMOUSEEVENT(fevt)->y_root = evt.xbutton.y_root;
		FMOUSEEVENT(fevt)->state = FBUTTON_DOWN;
		
		if( evt.xbutton.button == 0 )
			FMOUSEEVENT(fevt)->button = FBUTTON_LEFT;
		else if( evt.xbutton.button == 1 )
			FMOUSEEVENT(fevt)->button = FBUTTON_MIDDLE;
		else if( evt.xbutton.button == 2 )
			FMOUSEEVENT(fevt)->button = FBUTTON_RIGHT;
		fevt->name = "mouse-button-event";
		f_signal_emit_full(w, "mouse-button-event", fevt);
		fevent_process( w );
	} else if( evt.type == ButtonRelease ) {
		fevt->id = FMOUSE_EVENT;
		FMOUSEEVENT(fevt)->x = evt.xbutton.x;
		FMOUSEEVENT(fevt)->y = evt.xbutton.y;
		FMOUSEEVENT(fevt)->x_root = evt.xbutton.x_root;
		FMOUSEEVENT(fevt)->y_root = evt.xbutton.y_root;
		FMOUSEEVENT(fevt)->state = FBUTTON_UP;
		
		if( evt.xbutton.button == 0 )
			FMOUSEEVENT(fevt)->button = FBUTTON_LEFT;
		else if( evt.xbutton.button == 1 )
			FMOUSEEVENT(fevt)->button = FBUTTON_MIDDLE;
		else if( evt.xbutton.button == 2 )
			FMOUSEEVENT(fevt)->button = FBUTTON_RIGHT;
		fevt->name = "mouse-button-event";
		f_signal_emit_full(w, "mouse-button-event", fevt);
		fevent_process( w );
	}
	
#else
#warning Without X11 no have events yet.
#endif
	
	return fevt;
}

void fevent_windowloop( fWindow* w ) {
	fEvent* e;
	
	thsys_addp( Render, w );
	
	while(1) {
		e = fevent_windowstep(w);
		wait(1);
	}
}
