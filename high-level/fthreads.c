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

TRedBlack* current_threads = NULL;
TRedBlack* all_threads = NULL;

GMutex* thsys_mutex = NULL;
fThread* thsys_last_inserted = NULL;
int FPS_MAX = 0;


void thsys_init() {
    g_thread_init(NULL);

    thsys_mutex = g_mutex_new();
}

fThread* _thsys_addp( fThread* this, fThread* thread ) {
	fThread* new_thread = NULL;
	
	if( 1/*all_threads == NULL || 
		(new_thread = fth_tree_search_all( container_of( all_threads, fThread, rb2 ), this )) == NULL*/ ) {
			
		new_thread = g_try_new0(fThread, 1);
#if 0
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
#endif
		/*Adjust variables*/
		new_thread->mutex_all = thsys_mutex;
		new_thread->mutex = g_mutex_new();
		new_thread->cond = g_cond_new();
		new_thread->timer = g_timer_new();
		
		new_thread->series.next = new_thread;
		new_thread->series.prev = new_thread;
		
		new_thread->parallel.next = thread->parallel.next;
		new_thread->parallel.prev = thread;
			
		thread->parallel.next = new_thread;
		thread->parallel.next->parallel.prev = new_thread;
		
		THSYS_UNLOCK
		/*From this moment, the function is not thread safe*/
	}
	
	g_mutex_lock(new_thread->mutex);
	
	return new_thread;
	
}

fThread* _thsys_add( GThread* this ) {
	fThread* new_thread = NULL;
	
	if( 1/*all_threads == NULL || 
		(new_thread = fth_tree_search_all( container_of( all_threads, fThread, rb2 ), this )) == NULL*/ ) {
			
		new_thread = g_try_new0(fThread, 1);
	
#if 0
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
	
#endif
		/*Adjust variables*/
		new_thread->mutex_all = thsys_mutex;
		new_thread->mutex = g_mutex_new();
		new_thread->cond = g_cond_new();
		new_thread->timer = g_timer_new();
		
		/*Add "new_thread" to execution stack(serie)*/
		if( thsys_last_inserted != NULL ) {
			/*List insert procedure*/
			new_thread->series.next = thsys_last_inserted->series.next;
			new_thread->series.prev = thsys_last_inserted;
			
			thsys_last_inserted->series.next = new_thread;
			thsys_last_inserted->series.next->series.prev = new_thread;
			
			thsys_last_inserted = new_thread;
			/* Wait your turn in line. To prevent
			   two or more threads run at the same
			   time */
			THSYS_WAIT(new_thread->cond)
		} else {
			/* It begins with a circular list,
			   to always be a circular list */
			new_thread->series.next = new_thread;
			new_thread->series.prev = new_thread;
			
			thsys_last_inserted = new_thread;
			
			THSYS_UNLOCK
		}
		
		/*From this point, the function is not thread safe*/
		new_thread->parallel.next = new_thread;
		new_thread->parallel.prev = new_thread;
	}
	
	g_mutex_lock(new_thread->mutex);
	
	return new_thread;
	
}

void _wait( fThread* this, double value ) {
	
	fThread *x = NULL, *y = NULL, *z = NULL;
	double time;
	
	g_timer_start( this->timer );
	
	g_mutex_unlock(this->mutex);
	
	if( this != this->parallel.next ) {
		for( x = this->parallel.next; x != this && !g_mutex_trylock(x->mutex); x = x->parallel.next )
			g_cond_wait( this->cond, this->mutex );
		
 		if( x != this )
			y = x;
		
		z = x;
		
	}
	
	if( ((int)value) >= 0 ) {
		this->remaining_time = 0;
		this->remaining_frames = ((int)value);
	} else {
		this->remaining_time = -value;
		this->remaining_frames = 0;
	}

	/* 'this' is already locked by "wait", 
	   so I know it's not 'this', I know
	   also that thread is not running */

	x = this->series.next;
	while( this->remaining_time > 0 || this->remaining_frames > 0 ) {
		
		if( x == this ) {
			if( this->remaining_time > 0 )
				SDL_Delay( this->remaining_time * 1000 );
		} else {
			if( !g_mutex_trylock(x->mutex) ) {
				x = x->series.next;
			} else {
				g_cond_signal( x->cond );
				g_mutex_unlock( x->mutex );
				g_cond_wait( this->cond, this->mutex );
			}
		}

		g_timer_stop( this->timer );

		this->remaining_frames--;
		this->remaining_time -= (time = g_timer_elapsed( this->timer, NULL ));
		
		g_timer_start( this->timer );
		
		if( x == this )
			break;
	}

	if( y != NULL )
		g_cond_signal( y->cond );
	
	if( z != NULL )
		g_mutex_unlock( z->mutex );
	

	g_mutex_lock( this->mutex );
	
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
