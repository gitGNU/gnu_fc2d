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

TRedBlack* current_threads = NULL;
TRedBlack* all_threads = NULL;

GMutex* thsys_mutex = NULL;

fThread* thsys_last_inserted = NULL;

void thsys_init() {
    g_thread_init(NULL);

    thsys_mutex = g_mutex_new();
}

void _thsys_add( GThread* this ) {
	fThread* new_thread = NULL;
	
	if( all_threads == NULL || 
		fth_tree_search_all( container_of( all_threads, fThread, rb2 ), this ) == NULL ) {
			
		new_thread = g_try_new0(fThread, 1);
	
		/*Insert this thread in "all threads" tree*/
		fth_tree_insert_all( 
		container_of( all_threads, fThread, rb2 ),
		new_thread
		);
		t_insert( &all_threads, &(new_thread->rb2) );
	
		/*Insert this thread in "current running threads" tree*/
		fth_tree_insert( 
		container_of( current_threads, fThread, rb1 ),
		new_thread
		);
		t_insert( &current_threads, &(new_thread->rb1) );
		
		/*Add "new_thread" to execution stack(serie)*/
		if( thsys_last_inserted != NULL ) {
			/*List insert procedures*/
			new_thread->series.next = thsys_last_inserted->series.next;
			new_thread->series.prev = thsys_last_inserted;
			
			thsys_last_inserted->series.next = new_thread;
			thsys_last_inserted->series.next->series.prev = new_thread;
			
		} else {
			/*It begins with a circular list, to always be a circular list*/
			new_thread->series.next = new_thread;
			new_thread->series.prev = new_thread;
		}
		
		thsys_last_inserted = new_thread;
		
		/*Adjust variables*/
		new_thread->mutex_all = thsys_mutex;
		new_thread->mutex = g_mutex_new();
		new_thread->cond = g_cond_new();
	}
	
}

void fth_tree_insert( fThread* tree, fThread* data ) {
    t_insert_custom( fThread, tree, thread, data, rb1 );
	
}

fThread* fth_tree_search( fThread* tree, GThread* data )
	t_search_custom( fThread, tree, thread, data, rb1 );
	
	
void fth_tree_insert_all( fThread* tree, fThread* data )
	t_insert_custom( fThread, tree, thread, data, rb2 );
	
fThread* fth_tree_search_all( fThread* tree, GThread* data )
	t_search_custom( fThread, tree, thread, data, rb2 );
