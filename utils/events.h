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

#ifndef __UTILS_FEVENTS_H__
#define __UTILS_FEVENTS_H__ 1

#include <glib.h>
#include <video/window.h>
#include <video/widgets.h>

/*Macros for casting*/
#define FEVENT(x) \
	((fEvent*)(x))
	
#define FKEYEVENT(x) \
	((fKeyEvent*)(x))

#define FMOUSEEVENT(x) \
	((fMouseEvent*)(x))	

#define FEXPOSEEVENT(x) \
	((fExposeEvent*)(x))

typedef enum {
	FKEY_DOWN,
	FKEY_UP
} fKeyState;

typedef enum {
	FBUTTON_DOWN,
	FBUTTON_UP
} fButtonState;

typedef enum {
	FBUTTON_NONE,
	FBUTTON_LEFT,
	FBUTTON_MIDDLE,
	FBUTTON_RIGHT
} fButton;

typedef enum {
    FMOUSE_CLICK,
    FMOUSE_MOVE
} fMouseEventType;

typedef struct {
	int x;
	int y;
	int x_root;
	int y_root;
	
	fButton button;
	fButtonState state;
    fMouseEventType type;
} fMouseEvent;

typedef struct {
	/*Mouse cordinates*/
	int x;
	int y;
	
	fKeyState state;
	unsigned int keycode;
} fKeyEvent;

typedef struct {
	int x;
	int y;
	int width;
	int height;
} fExposeEvent;

typedef struct {
    int x;
    int y;
    int begin_x;
    int begin_y;
    union {
        struct {
            int end_x;
            int end_y;
        };
        struct {
            int drop_x;
            int drop_y;
        };
    };
    
    /*widget initial positions*/
    int widget_x;
    int widget_y;
} fDropEvent;

typedef fDropEvent fDragEvent;

typedef enum {
	FKEY_EVENT,
	FMOUSE_EVENT,
	FEXPOSE_EVENT,
    FDRAG_EVENT,
    FDRAG_BEGIN_EVENT,
    FDRAG_END_EVENT,
    FDROP_EVENT = FDRAG_END_EVENT
} fEventId;

typedef enum {
	TYPE_FWIDGET,
	TYPE_NONE
} fObjType;

typedef struct {
	/* Is the first for casting */
	union {
		struct {
			int x;
			int y;
		};
		fKeyEvent key;
		fMouseEvent mouse;
		fExposeEvent expose;
        fDropEvent drag_drop;
	};
	
	fEventId id;
	char* msg;
	char* name;
	/*!
	 * \brief The object that launched the event 
	 */
	gpointer obj;
	fObjType obj_type;
} fEvent;

gboolean fevent_process( fEvent* evt );
#if HAVE_X11
fEvent* fevent_windowstep( fWindow* w );
#endif
void fevent_windowloop( fWindow* w );



#endif