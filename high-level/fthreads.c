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
#include "fthreads.h"
#include "utils/utils.h"
#include <SDL.h>


GHashTable* thsys_hash = NULL;
GMutex* thsys_mutex = NULL;
int FPS_MAX = 0;


void thsys_init() {
	
	/*This function has been called before?*/
	if( thsys_mutex != NULL )
		return;

    g_thread_init(NULL);
    thsys_mutex = g_mutex_new();
}

void thsys_initvalues( fThread* fth ) {
	if( fth == NULL )
		return;
	
	fth->parallel.next = fth;
	fth->parallel.prev = fth;
	fth->series.next = fth;
	fth->series.prev = fth;
	
	mutex_all = thsys_mutex;
	fth->mutex = g_mutex_new();
	fth->mutexp = g_mutex_new();
	fth->mutex_wait = g_mutex_new();
	fth->cond = g_cond_new();
	fth->condp = g_cond_new();
	fth->timer = g_timer_new();
	
	fth->p = g_hash_table_lookup ( thsys_hash, g_thread_self() );
	
	if( fth->p != NULL )
		fth->mutex_line = fth->p->mutex_line;
	else
		fth->mutex_line = g_mutex_new();
}

fThread* thsyshash_get1( GThread* th ) {
	fThread* h;
	
	if( thsys_hash == NULL )
		thsys_hash = g_hash_table_new( g_direct_hash, g_direct_equal );
	
	h = g_hash_table_lookup ( thsys_hash, th );
	
	if( h == NULL ) {
		h = g_malloc0(sizeof(fThread));
		thsys_initvalues( th );
		g_hash_table_insert( thsys_hash, th, h );
	}
	
	return h;
}

fThread* thsyshash_get() {
	fThread* h;
	
	if( thsys_hash == NULL )
		thsys_hash = g_hash_table_new( g_direct_hash, g_direct_equal );
	
	h = g_hash_table_lookup ( thsys_hash, g_thread_self() );
	
	if( h == NULL ) {
		h = g_malloc0(sizeof(fThread));
		thsys_initvalues( th );
		g_hash_table_insert( thsys_hash, g_thread_self(), h );
	}
	
	return h;
}

void thsyshash_delete() {
	if( thsys_hash != NULL )
		g_hash_table_remove( thsys_hash, g_thread_self() );
}

fThread* thsys_add( FCallback function, gpointer data ) {
	GThread* th;
	fThread* fth;
	
	th = g_thread_create( function, data, TRUE, NULL);
	fth = thsyshash_get1(th);
	
	list_insert( fth->p, fth, series )
	fth->ltype = SERIES;
	
	g_cond_wait(fth->cond, fth->mutex);
}

fThread* thsys_addp( FCallback function, gpointer data );

gboolean thsys_remove(fThread* thread);

void wait( float value );

void waitp();

void waits( float value );
