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

#ifndef __UTILS_FEVENTS_H__
#define __UTILS_FEVENTS_H__ 1

#include <glib.h>
#include <video/window.h>

/*Macros for casting*/
#define FEVENT(x) \
	((fEvent*)(x))
	
#define FKEYEVENT(x) \
	((fKeyEvent*)(x))

#define FMOUSEEVENT(x) \
	((fMouseEvent*)(x))	

#define FEXPOSEEVENT(x) \
	((fExposeEvent*)(x))

struct _fEvent;
typedef struct _fEvent fEvent;

struct _fKeyEvent;
typedef struct _fKeyEvent fKeyEvent;

struct _fMouseEvent;
typedef struct _fMouseEvent fMouseEvent;

struct _fExposeEvent;
typedef struct _fExposeEvent fExposeEvent;

enum _fEventId;
typedef enum _fEventId fEventId;

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

struct _fMouseEvent {
	int x;
	int y;
	int x_root;
	int y_root;
	
	fButton button;
	fButtonState state;
};

struct _fKeyEvent {
	/*Mouse cordinates*/
	int x;
	int y;
	
	fKeyState state;
	unsigned int keycode;
};

struct _fExposeEvent {
	int x;
	int y;
	int width;
	int height;
};

enum _fEventId {
	FKEY_EVENT,
	FMOUSE_EVENT,
	FEXPOSE_EVENT
};

struct _fEvent {
	/* Is the first for casting */
	union {
		fKeyEvent key;
		fMouseEvent mouse;
		fExposeEvent expose;
	};
	
	fEventId id;
	char* msg;
	/*!
	 * \brief The object that launched the event 
	 */
	gpointer obj;
};

gboolean fevent_process( fEvent* evt );
fEvent* fevent_windowstep( fWindow* w );
void fevent_windowloop( fWindow* w );

#endif