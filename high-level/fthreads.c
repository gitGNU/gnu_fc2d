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
fThread* thsys_mutex_line = NULL;
fThread* thsys_root = NULL;
int FPS_MAX = 0;


void thsys_init() {
    g_thread_init(NULL);

    thsys_mutex = g_mutex_new();
	thsys_mutex_line = g_mutex_new();
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
		new_thread->mutex_line = thsys_mutex_line;
		new_thread->mutex = g_mutex_new();
		new_thread->mutexp = g_mutex_new();
		new_thread->cond = g_cond_new();
		new_thread->condp = g_cond_new();
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
	
	g_mutex_lock(new_thread->mutexp);
	g_mutex_lock(new_thread->mutex);
	
	return new_thread;
	
}

fThread* _thsys_add( fThread* __current_fthread, GThread* this ) {
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
		new_thread->mutex_line = thsys_mutex_line;
		new_thread->mutex = g_mutex_new();
		new_thread->mutexp = g_mutex_new();
		new_thread->cond = g_cond_new();
		new_thread->condp = g_cond_new();
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
	
	g_mutex_lock(new_thread->mutexp);
	g_mutex_lock(new_thread->mutex);
	
	return new_thread;
	
}

void _wait( fThread* this, double value ) {
	
	fThread *x = NULL, *prev = NULL;
	double time;
	
	g_timer_start( this->timer );
		
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


	g_mutex_lock(this->mutex_line);
	x = this->series.next;
	g_mutex_unlock(this->mutex_line);
		
	/* wait frames cycles */
	while( this->remaining_frames > 0 && x != this ) {
		g_cond_signal( x->cond );
		g_cond_wait( this->cond, this->mutex );
		
		this->remaining_frames--;
	}
	
	/*wait time*/
#if 0
	if( this->remaining_time > 0 && x != this ) {

		/*Remove me from "series" list*/
		g_mutex_lock(this->mutex_line);		
		this->series.prev->series.next =
			this->series.next;
		this->series.next->series.prev =
			this->series.prev;
		g_mutex_unlock(this->mutex_line);
		
		g_mutex_lock(x->mutex);
		g_cond_signal( x->cond );
		g_mutex_unlock( x->mutex );
		
		g_timer_stop( this->timer );
		this->remaining_time -= g_timer_elapsed( this->timer, NULL );
		
		SDL_Delay( this->remaining_time * 1000 );
		this->remaining_time = 0;
				
		/* This thread return to list
			and wait be called */
		g_mutex_lock(this->mutex_line);
		prev = this->series.prev;
		this->series.next = prev->series.next;
		prev->series.next = this;
		prev->series.next->series.prev = this;
		g_mutex_unlock(this->mutex_line);
		
		g_cond_wait( this->cond, this->mutex );
		
	} else if ( this->remaining_time > 0 ) {
		g_timer_stop( this->timer );
		this->remaining_time -= g_timer_elapsed( this->timer, NULL );
		
		SDL_Delay( this->remaining_time * 1000 );
	}
#else
	while( this->remaining_time > 0 && x != this ) {
		g_timer_stop( this->timer );
		this->remaining_time -= g_timer_elapsed( this->timer, NULL );
		g_timer_start( this->timer );
		
		g_cond_signal( x->cond );
		g_cond_wait( this->cond, this->mutex );
	}
	
	if ( this->remaining_time > 0 && x == this ) {
		g_timer_stop( this->timer );
		this->remaining_time -= g_timer_elapsed( this->timer, NULL );
		
		SDL_Delay( this->remaining_time * 1000 );
	}
#endif
	
	x = this->parallel.next;
	if( x != this ) {
		/* If the thread is not over, I hope it finishes */
		if( !g_mutex_trylock(x->mutexp) )
			g_cond_wait(x->condp, x->mutexp); 
		
		g_cond_signal( x->condp );

	}
}

gboolean _thsys_add_with_thread( fThread* parent, FCallback* function, gpointer data ) {
	
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