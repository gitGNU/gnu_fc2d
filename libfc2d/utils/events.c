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

#include <utils/events.h>
#include <utils/event-basis.h>
#include <video/window.h>
#include <video/render.h>
#include <video/widgets.h>
#include <glib.h>
#include <high-level/threads.h>
#include <utils/data-connect.h>

#if HAVE_X11
#include<X11/X.h>
#include<X11/Xlib.h>
#endif

#if HAVE_ALSA
#include <audio/alsa.h>
#endif

gboolean fevent_process( fEvent* evt ) {
	fWidget* widget;
	fWidget* widget2;
	GList* l;

    g_assert( evt != NULL );
       
	if( evt->obj_type == TYPE_FWIDGET ) {
		widget = evt->obj;
        
        if( widget == NULL )
            return FALSE;
        
		if( evt->x >= widget->x &&
			evt->x < widget->x + widget->width &&
			evt->y >= widget->y &&
			evt->y < widget->y + widget->height
		) {
			for( l = widget->childs; l != NULL; l = l->next ) {
				widget2 = l->data;
				
				if( evt->x >= widget2->x &&
					evt->x < widget2->x + widget2->width &&
					evt->y >= widget2->y &&
					evt->y < widget2->y + widget2->height
				) {
					f_signal_emit_full( widget2, evt->name, evt);
                    evt->obj = widget2;
                    fevent_process( evt );
                }
			}
			
			return TRUE;
		}
	}
	
	return FALSE;
}

#if HAVE_X11

fEvent* fevent_windowstep( gpointer win ) {
//TODO: Support to no X11 events
	fEvent* fevt = NULL;
	fWindow* w = win;
	XEvent evt;
	gboolean check;
	
    g_assert( win != NULL );
    
    fevt = f_data_get( win, "FEVENTS" );
	if( fevt == NULL ) {
		fevt = g_malloc0( sizeof(fEvent) );
        f_data_connect( win, "FEVENTS", fevt );
    }
    
  	check = XCheckMaskEvent( w->display, 
                               KeyPressMask | 
                               KeyReleaseMask |
                               ButtonPressMask |
                               ButtonReleaseMask | 
                               PointerMotionMask |
                               ButtonMotionMask|
                               ExposureMask, &evt);
	
    
    if( check == FALSE )
		return NULL;
	
    fevt->obj = win;
    
	if( evt.type == Expose ) {
		fevt->id = FEXPOSE_EVENT;
		FEXPOSEEVENT(fevt)->x = evt.xexpose.x;
		FEXPOSEEVENT(fevt)->y = evt.xexpose.y;
		FEXPOSEEVENT(fevt)->width = evt.xexpose.width;
		FEXPOSEEVENT(fevt)->height = evt.xexpose.height;
		fevt->name = "expose-event";
		f_signal_emit_full(w, fevt->name, fevt);
		fevent_process( fevt );
	} else if( evt.type == MotionNotify ) {
		fevt->id = FMOUSE_EVENT;
		FMOUSEEVENT(fevt)->x = evt.xmotion.x;
		FMOUSEEVENT(fevt)->y = evt.xmotion.y;
		FMOUSEEVENT(fevt)->x_root = evt.xmotion.x_root;
		FMOUSEEVENT(fevt)->y_root = evt.xmotion.y_root;
		FMOUSEEVENT(fevt)->button = FBUTTON_NONE;
        FMOUSEEVENT(fevt)->type = FMOUSE_MOVE;
		fevt->name = "mouse-motion-event";
		f_signal_emit_full(w, fevt->name, fevt);
		fevent_process( fevt );
	} else if( evt.type == KeyPress ) {
		fevt->id = FKEY_EVENT;
		FKEYEVENT(fevt)->state = FKEY_DOWN;
		FKEYEVENT(fevt)->keycode = evt.xkey.keycode;
		FKEYEVENT(fevt)->x = evt.xkey.x;
		FKEYEVENT(fevt)->y = evt.xkey.y;
		fevt->name = "key-event";
		f_signal_emit_full(w, fevt->name, fevt);
		fevent_process( fevt );
	} else if( evt.type == KeyRelease ) {
		fevt->id = FKEY_EVENT;
		FKEYEVENT(fevt)->state = FKEY_UP;
		FKEYEVENT(fevt)->keycode = evt.xkey.keycode;
		FKEYEVENT(fevt)->x = evt.xkey.x;
		FKEYEVENT(fevt)->y = evt.xkey.y;
		fevt->name = "key-event";
		f_signal_emit_full(w, fevt->name, fevt);
		fevent_process( fevt );
	} else if( evt.type == ButtonPress ) {
		fevt->id = FMOUSE_EVENT;
		FMOUSEEVENT(fevt)->x = evt.xbutton.x;
		FMOUSEEVENT(fevt)->y = evt.xbutton.y;
		FMOUSEEVENT(fevt)->x_root = evt.xbutton.x_root;
		FMOUSEEVENT(fevt)->y_root = evt.xbutton.y_root;
		FMOUSEEVENT(fevt)->state = FBUTTON_DOWN;
		FMOUSEEVENT(fevt)->type = FMOUSE_CLICK;
		if( evt.xbutton.button == 0 )
			FMOUSEEVENT(fevt)->button = FBUTTON_LEFT;
		else if( evt.xbutton.button == 1 )
			FMOUSEEVENT(fevt)->button = FBUTTON_MIDDLE;
		else if( evt.xbutton.button == 2 )
			FMOUSEEVENT(fevt)->button = FBUTTON_RIGHT;
		fevt->name = "mouse-button-event";
		f_signal_emit_full(w, fevt->name, fevt);
		fevent_process( fevt );
	} else if( evt.type == ButtonRelease ) {
		fevt->id = FMOUSE_EVENT;
		FMOUSEEVENT(fevt)->x = evt.xbutton.x;
		FMOUSEEVENT(fevt)->y = evt.xbutton.y;
		FMOUSEEVENT(fevt)->x_root = evt.xbutton.x_root;
		FMOUSEEVENT(fevt)->y_root = evt.xbutton.y_root;
		FMOUSEEVENT(fevt)->state = FBUTTON_UP;
		FMOUSEEVENT(fevt)->type = FMOUSE_CLICK;

		if( evt.xbutton.button == 0 )
			FMOUSEEVENT(fevt)->button = FBUTTON_LEFT;
		else if( evt.xbutton.button == 1 )
			FMOUSEEVENT(fevt)->button = FBUTTON_MIDDLE;
		else if( evt.xbutton.button == 2 )
			FMOUSEEVENT(fevt)->button = FBUTTON_RIGHT;
		fevt->name = "mouse-button-event";
		f_signal_emit_full(w, fevt->name, fevt);
		fevent_process( fevt );
	}
		
		
	return fevt;
}

#endif

void fevent_windowloop( gpointer win ) {
	fEvent* e;
	GList* l;
    fWindow* w = win;
    int tmp;
    
	thsys_add( Render, w );
    wait(1);
    
     XSelectInput( w->display, w->window, KeyPressMask |
     KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
     PointerMotionMask | ButtonMotionMask| ExposureMask);
    
	while(1) {
		fevent_windowstep(w);
		wait(1);
	}
}
