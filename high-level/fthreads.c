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
fThread* thsys_root = NULL;
int FPS_MAX = 0;


void thsys_init() {
	
	/*This function has been called before?*/
	if( thsys_mutex != NULL )
		return;

    g_thread_init(NULL);
    thsys_mutex = g_mutex_new();
}

fThread* _thsys_add( GThread* this ) {
	fThread* new_thread = NULL;
	
	new_thread = g_try_new0( fThread, 1 );
	
	/* Adjust variables for synchronism */
	new_thread->mutex_all = thsys_mutex;
	new_thread->mutex_line = g_mutex_new();
	new_thread->mutex = g_mutex_new();
	new_thread->mutexp = g_mutex_new();
	new_thread->mutex_wait = g_mutex_new();
	new_thread->cond = g_cond_new();
	new_thread->condp = g_cond_new();
	new_thread->timer = g_timer_new();
	
	/*Adjust more variables*/
	new_thread->p = NULL;
	new_thread->thread = this;
	
	g_mutex_lock(new_thread->mutex_line);
	
	new_thread->parallel.next = new_thread;
	new_thread->parallel.prev = new_thread;
	
	g_mutex_unlock(new_thread->mutex_line);
}

void _wait( fThread* this, double value ) {
	
	fThread *x = NULL, *prev = NULL;
	double time;
	
	g_mutex_lock( this->mutex_wait );
	
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
	while( (this->remaining_frames > 0 || value == 0) && x != this ) {
		if( g_mutex_trylock( x->mutex_wait ) ) {
			thsys_remove_him_by_fthread( x );
		}
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
		/* If the thread is not over, I wait it finishes */
		if( !g_mutex_trylock(x->mutexp) )
			g_cond_wait(x->condp, x->mutexp); 
		
		g_cond_signal( x->condp );

	}
	
	g_mutex_unlock( this->mutex_wait );
}

gboolean _thsys_add_with_thread( fThread* parent, FCallback* function, gpointer data ) {
	
	fThread* new_thread = NULL;
	
	new_thread = g_try_new0( fThread, 1 );
	
	/* Adjust variables for synchronism */
	new_thread->mutex_all = thsys_mutex;
	if( parent != NULL )
		new_thread->mutex_line = parent->mutex_line;
	else
		new_thread->mutex_line = g_mutex_new();
	new_thread->mutex = g_mutex_new();
	new_thread->mutexp = g_mutex_new();
	new_thread->mutex_wait = g_mutex_new();
	new_thread->cond = g_cond_new();
	new_thread->condp = g_cond_new();
	new_thread->timer = g_timer_new();
	
	/*Adjust more variables*/
	new_thread->p = parent;
	new_thread->func = function;
	new_thread->data = data;
	
	g_mutex_lock(new_thread->mutex_line);
	
	if( parent )
		list_insert( parent, new_thread, series )
	
	new_thread->parallel.next = new_thread;
	new_thread->parallel.prev = new_thread;
	
	g_mutex_unlock(new_thread->mutex_line);
	if( parent )
		g_mutex_lock( new_thread->mutex );
	
	new_thread->thread = g_thread_create( function, data, TRUE, NULL );
	
	if( parent )
		g_cond_wait( parent->cond, new_thread->mutex );
	
	return TRUE;
}

gboolean _thsys_addp_with_thread( fThread* parent, FCallback* function, gpointer data ) {
	
	fThread* new_thread = NULL;
	
	new_thread = g_try_new0( fThread, 1 );
	
	/* Adjust variables for synchronism */
	new_thread->mutex_all = thsys_mutex;
	if( parent != NULL )
		new_thread->mutex_line = parent->mutex_line;
	else
		new_thread->mutex_line = g_mutex_new();
	new_thread->mutex = g_mutex_new();
	new_thread->mutexp = g_mutex_new();
	new_thread->mutex_wait = g_mutex_new();
	new_thread->cond = g_cond_new();
	new_thread->condp = g_cond_new();
	new_thread->timer = g_timer_new();
	
	/*Adjust more variables*/
	new_thread->p = parent;
	new_thread->func = function;
	new_thread->data = data;
	
	g_mutex_lock(new_thread->mutex_line);
	
	if( parent )
		list_insert( parent, new_thread, parallel )
	
	new_thread->series.next = new_thread;
	new_thread->series.prev = new_thread;
	
	g_mutex_unlock(new_thread->mutex_line);
	
	new_thread->thread = g_thread_create( function, data, TRUE, NULL );
	
	return TRUE;
}

void _thsys_remove( fThread* this ) {
}

gboolean thsys_remove_him_by_fthread(fThread* thread) {
	
	if( thread == NULL )
		return FALSE;
	
	g_mutex_lock(thread->mutex_line);
	thread->series.next->series.prev = thread->series.prev;
	thread->series.prev->series.next = thread->series.next;
	
	thread->parallel.next->parallel.prev = thread->parallel.prev;
	thread->parallel.prev->parallel.next = thread->parallel.next;
	g_mutex_unlock(thread->mutex_line);
	
	g_mutex_free(thread->mutex);
	g_mutex_free(thread->mutexp);
	g_cond_free(thread->cond);
	g_cond_free(thread->condp);
	g_mutex_free(thread->mutexp);
	g_mutex_free(thread->mutex_wait);
	
	g_free(thread);
	
	return TRUE;
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