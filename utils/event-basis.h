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

#ifndef __UTILS_EVENT_BASIS_H__
#define __UTILS_EVENT_BASIS_H__ 1

#include <utils/utils.h>
#include <glib.h>

extern GHashTable* f_connect_hash;

#define FEVENTFUNCTION(x) \
	((FEventFunction*)(x))

#define f_signal_connect(name,function) \
	f_signal_connect_full((gpointer)0, name, function, NULL)

	
#define f_signal_disconnect(name,function) \
	f_signal_disconnect_full((gpointer)0, name, function)
	
#define f_signal_emit(name, data) \
	f_signal_emit_full((gpointer)0, name, data)
	
typedef struct {
	FCallback2 function;
	gpointer data;
	gpointer obj;
} FEventFunction;

int feventb_searchfunction( 
	FEventFunction* a,
	FEventFunction* b );

GHashTable* f_signal_obj_get( gpointer id );

int f_signal_connect_full( gpointer obj, 
						   const char* name,
						   FCallback function,
						   gpointer data);

void f_signal_disconnect_full( gpointer obj,
							   const char* name,
							   FCallback function );

void f_signal_emit_full( gpointer obj,
						 const char* name,
						 gpointer data );

#endif