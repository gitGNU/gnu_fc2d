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
#include <high-level/threads.h>
#include <utils/event-basis.h>
#include <SDL.h>

GHashTable* thsys_hash = NULL;
GMutex* thsys_mutex = NULL;
int FPS_MAX = 0;

void thsys_coming( FEventFunction* fun ) {
	
	GList* l;
	fThread* this;
	
	g_mutex_lock( fun->obj );
	this = thsyshash_get();
	for( l = this->coming; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
	}
	
	g_mutex_unlock( fun->obj );
	
	fun->function( fun->data, NULL );
	
	for( l = this->leaving; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
	}
	
	thsys_remove( this );
	
}

void thsys_addcoming( fThread* th, 
					  FCallback2 function, gpointer data )
{
	FEventFunction* fun;
	
	fun = g_malloc( sizeof(FEventFunction) );
	
	fun->function = function;
	fun->data = data;
	
	if( th->coming )
		th->coming = g_list_prepend( th->coming, fun );
	else 
		th->coming = g_list_append( NULL, fun );
	
	th->child_coming = th->coming;
}


void thsys_addleaving( fThread* th, 
					  FCallback2 function, gpointer data ) 
{
	FEventFunction* fun;
	
	fun = g_malloc( sizeof(FEventFunction) );
	
	fun->function = function;
	fun->data = data;
	
	if( th->leaving )
		th->leaving = g_list_prepend( th->leaving, fun );
	else 
		th->leaving = g_list_append( NULL, fun );
	
	th->child_leaving = th->leaving;
}


void thsys_removecoming( fThread* th, 
					  FCallback2 function ) 
{
	GList* m;
	FEventFunction fun;
	
	fun.function = function;
	
	m = g_list_find_custom( th->coming, &fun,
							feventb_searchfunction );
	
	if( m != NULL ) {
		g_free(m->data);
		th->coming = g_list_delete_link ( th->coming, m );
		g_free(m);
	}
}


void thsys_removeleaving( fThread* th, 
					  FCallback2 function ) 
{
	GList* m;
	FEventFunction fun;
	
	fun.function = function;
	
	m = g_list_find_custom( th->leaving, &fun,
							feventb_searchfunction );
	
	if( m != NULL ) {
		g_free(m->data);
		th->leaving = g_list_delete_link ( th->coming, m );
		g_free(m);
	}
}

inline void thsys_init() {
	
	/*This function has been called before?*/
	if( thsys_mutex != NULL )
		return;

    g_thread_init(NULL);
    thsys_mutex = g_mutex_new();

}

fThread* thsyshash_try_get() {
	if( thsys_hash == NULL )
		thsys_hash = g_hash_table_new( g_direct_hash, g_direct_equal );

	return g_hash_table_lookup ( thsys_hash, g_thread_self() );
	
}

void thsys_initvalues( fThread* fth ) {
	if( fth == NULL )
		return;
	
	thsys_init();
	
	fth->parallel.next = fth;
	fth->parallel.prev = fth;
	fth->series.next = fth;
	fth->series.prev = fth;
	
	fth->mutex_all = thsys_mutex;
	fth->mutex = g_mutex_new();
	fth->mutexp = g_mutex_new();
	fth->mutex_wait = g_mutex_new();
	fth->mutex_data = g_mutex_new();
	fth->cond = g_cond_new();
	fth->condp = g_cond_new();
	fth->timer = g_timer_new();
	fth->coming = NULL;
	fth->leaving = NULL;
	
	fth->p = g_hash_table_lookup ( thsys_hash, g_thread_self() );
	
	if( fth->p != NULL )
		fth->mutex_line = fth->p->mutex_line;
	else
		fth->mutex_line = g_mutex_new();
	
	g_mutex_lock( fth->mutex );
}

void thsys_deletevalues( fThread* fth ) {
	
	g_mutex_lock(fth->mutex_line);
	/*Remove from lists*/
	fth->parallel.next->parallel.prev = fth->parallel.prev;
	fth->parallel.prev->parallel.next = fth->parallel.next;
	fth->series.next->series.prev = fth->series.prev;
	fth->series.prev->series.next = fth->series.next;
	g_mutex_unlock(fth->mutex_line);
	
	g_mutex_lock( fth->mutex );
	g_cond_signal( fth->cond );
	g_mutex_unlock( fth->mutex );
	
	g_mutex_lock( fth->mutexp );
	g_cond_signal( fth->condp );
	g_mutex_unlock( fth->mutexp );
	
	if( fth->p == NULL )
		g_free(fth->mutex_line);
	
	g_mutex_free(fth->mutex);
	g_mutex_free(fth->mutexp);
	g_mutex_free(fth->mutex_wait);
	g_cond_free(fth->cond);
	g_cond_free(fth->condp);
	g_free(fth->timer);
	g_free(fth->evtf);
	
}

fThread* thsyshash_get1( GThread* th ) {
	fThread* h;
	
	if( thsys_hash == NULL )
		thsys_hash = g_hash_table_new( g_direct_hash, g_direct_equal );
	
	h = g_hash_table_lookup ( thsys_hash, th );
	
	if( h == NULL ) {
		h = g_malloc0(sizeof(fThread));
		thsys_initvalues(h);
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
		thsys_initvalues(h);
		g_hash_table_insert( thsys_hash, g_thread_self(), h );
	}
	
	return h;
}

void thsyshash_delete() {
	if( thsys_hash != NULL )
		g_hash_table_remove( thsys_hash, g_thread_self() );
}

void thsyshash_delete1( GThread* th ) {
	if( thsys_hash != NULL )
		g_hash_table_remove( thsys_hash, th );
}

fThread* thsys_add( FCallback function, gpointer data ) {
	GThread* th;
	fThread* fth;
	fThread* this;
	FEventFunction* evtf;
	
	this = thsyshash_get();
	
	evtf = g_malloc( sizeof( FEventFunction ) );
	
	evtf->function = function;
	evtf->data = data;
	evtf->obj = g_mutex_new();
	g_mutex_lock( evtf->obj );
	th = g_thread_create( thsys_coming, evtf, TRUE, NULL);
	fth = thsyshash_get1(th);
	fth->evtf = evtf;
	fth->coming = this->child_coming;
	fth->leaving = this->child_leaving;
	
	list_insert( fth->p, fth, series )
	fth->call_mode = fth->ltype = SERIES;
	g_mutex_lock( fth->mutexp );
	
	if( this->ps == NULL )
		fth->ps = this;
	else
		fth->ps = this->ps;
	
	g_mutex_lock( fth->ps->mutex_data );
	fth->ps->series_num++;
	g_mutex_unlock( fth->ps->mutex_data );
	
	g_mutex_unlock( evtf->obj );
	
	g_cond_wait(this->cond, this->mutex);
	
	return fth;
}

fThread* thsys_addp( FCallback function, gpointer data ) {
	GThread* th;
	fThread* fth;
	fThread* this;
	FEventFunction* evtf;
	
	this = thsyshash_get();
	
	evtf = g_malloc( sizeof( FEventFunction ) );
	
	evtf->function = function;
	evtf->data = data;
	evtf->obj = g_mutex_new();
	g_mutex_lock( evtf->obj );
	th = g_thread_create( thsys_coming, evtf, TRUE, NULL);
	fth = thsyshash_get1(th);
	fth->evtf = evtf;
	fth->coming = this->child_coming;
	fth->leaving = this->child_leaving;
	g_mutex_lock( fth->mutexp );
	
	if( this->pp == NULL )
		fth->pp = this;
	else
		fth->pp = this->pp;
	
	g_mutex_lock( fth->pp->mutex_data );
	fth->pp->parallel_num++;
	g_mutex_unlock( fth->pp->mutex_data );
	
	list_insert( fth->p, fth, parallel )
	fth->call_mode = fth->ltype = PARALLEL;
	g_mutex_unlock( evtf->obj );
	
	return fth;
}

gboolean thsys_remove(fThread* thread) {
	thsyshash_delete1( thread->thread );
	thsys_deletevalues( thread );
	g_free( thread );
}

void wait( double value ) {
	
	fThread* this;
	GList* l;
	
	this = thsyshash_get();
	
	/* Wait the `parallel` thread finish, 
	   synchronize this point. */
// 	if( this->ltype != this->call_mode )
// 		synchronize(SERIES);
// 	else
// 		synchronize(PARALLEL);
	
	for( l = this->leaving; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
		}
	
	if( value < 0 ) {
		this->remaining_time = -value;
		this->remaining_frames = 1;
	} else if( ((int)(value)) >= 0 ) {
		this->remaining_time = 0;
		this->remaining_frames = ((int)(value));
	}
	
	if( this->remaining_frames == 0 ) {
		while(1) {
			if( this->ltype != this->call_mode )
			{
				thsys_step(PARALLEL);
			} else {
				thsys_step(SERIES);
			}
		}
	}
	
	while( this->remaining_frames > 0 ) {
		
		if( this->series.next == this) 
			break;
		
		if( this->ltype != this->call_mode )
		{
			thsys_step(PARALLEL);
		} else {
			thsys_step(SERIES);
		}
		this->remaining_frames--;
	}
	
	/* Left 'parallel' thread runs,
	 * synchronize this point. */
//  	if( this->ltype != this->call_mode )
// 		synchronize(SERIES);
// 	else
// 		synchronize(PARALLEL);
	
	for( l = this->coming; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
	}
}

void waitp() {
	
	fThread* this;
	GList* l;
	
	this = thsyshash_get();
	
	for( l = this->leaving; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
		FEVENTFUNCTION(l->data)->data,
		this );
	}
	
	synchronize(PARALLEL);
	synchronize(SERIES);
	synchronize(SERIES);
	synchronize(PARALLEL);
	
	for( l = this->coming; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
	}
}

void waits( double value ) {
	fThread* this;
	GList* l;
	
	this = thsyshash_get();
	
	for( l = this->leaving; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
		}
	
	if( value < 0 ) {
		this->remaining_time = -value;
		this->remaining_frames = 1;
	} else if( ((int)(value)) >= 0 ) {
		this->remaining_time = 0;
		this->remaining_frames = ((int)(value));
	}
	
	if( this->remaining_frames == 0 ) {
		while(1) {
			thsys_step(PARALLEL);
			thsys_step(SERIES);	
		}
	}
	
	while( this->remaining_frames > 0 ) {
		
		if( this->series.next == this) 
			break;
		
		thsys_step(PARALLEL);
		thsys_step(SERIES);
		this->remaining_frames--;
	}
	
	
	for( l = this->coming; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
	}
}

void synchronize( ListType mode ) {
	fThread* this;
	fThread* l;
	fThread* master;
	GMutex* mutex;
	GCond* cond;
	GMutex* tmutex;
	GCond* tcond;
	unsigned int* count;
	unsigned int* num;
	
	this = thsyshash_get();
	
	if( mode == NONE || mode == PARALLEL ) {
		master = this->pp;
		tcond = this->condp;
		tmutex = this->mutexp;
		if( master != NULL ) {
			mutex = master->mutexp;
			cond = master->condp;
			count = &(master->parallel_count);
			num = &(master->parallel_num);
		} else {
			count = &(this->parallel_count);
			num = &(this->parallel_num);
		}
	}
	else {
		master = this->ps;
		tcond = this->cond;
		tmutex = this->mutex;
		if( master != NULL ) {
			mutex = master->mutex;
			cond = master->cond;
			count = &(master->series_count);
			num = &(master->series_num);
		} else {
			count = &(this->series_count);
			num = &(this->series_num);
		}
	}
	
		if( master != NULL ) {
			g_mutex_lock( master->mutex_data );
			(*count)++;
			g_mutex_unlock( master->mutex_data );
			g_mutex_lock(mutex);
			g_cond_signal(cond);
			g_mutex_unlock(mutex);
			g_cond_wait( tcond, tmutex );
		} else {

			while( 1 ) {
				g_mutex_lock( this->mutex_data );
				if( (*count) >= (*num) ) {
					g_mutex_unlock( this->mutex_data );
					break;
				}
				g_mutex_unlock( this->mutex_data );
				
				g_cond_wait( tcond, tmutex );
			}

			g_mutex_lock( this->mutex_data );
			(*count) = 0;
			g_mutex_unlock( this->mutex_data );
			
			if( mode == NONE || mode == PARALLEL )
				l = this->parallel.next;
			else
				l = this->series.next;
			
			while( l != this ) {
				
			if( mode == NONE || mode == PARALLEL ) {
				l = l->parallel.next;
				l->call_mode = PARALLEL;
				g_mutex_lock(l->mutexp);
				g_cond_signal(l->condp);
				g_mutex_unlock(l->mutexp);
			}
			else {
				l = l->series.next;
				l->call_mode = PARALLEL;
				g_mutex_lock(l->mutex);
				g_cond_signal(l->cond);
				g_mutex_unlock(l->mutex);
			}
			}
			
		}
}

void thsys_step( ListType mode ) {
	fThread* this;
	fThread* next;
	
	this = thsyshash_get();
	
	if( mode == SERIES ) {
		next = this->series.next;
		next->call_mode = SERIES;
		g_mutex_lock( next->mutex );
		g_cond_signal( next->cond );
		g_mutex_unlock( next->mutex );
		g_cond_wait( this->cond, this->mutex );
	}
	else {
		next = this->parallel.next;
		next->call_mode = SERIES;
		g_mutex_lock( next->mutexp );
		g_cond_signal( next->condp );
		g_mutex_unlock( next->mutexp );
		g_cond_wait( this->condp, this->mutexp );
	}
}