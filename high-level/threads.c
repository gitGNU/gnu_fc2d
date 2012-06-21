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

#include <config.h>
#include <glib.h>
#include <high-level/threads.h>
#include <utils/event-basis.h>
#include <utils/data-connect.h>

#if TIME_WITH_SYS_TIME
#include <time.h>
#elif HAVE_SDL
#include <SDL.h>
#endif


GHashTable* thsys_hash = NULL;
GMutex* thsys_mutex = NULL;

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
    wait(1);
	fun->function( fun->data, NULL );
	
	for( l = this->leaving; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
	}
	
	/*FIXME: Let thread finalize*/
	while(1) wait(1);
	
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
	fth->removeme = FALSE;
	
	fth->p = g_hash_table_lookup ( thsys_hash, 
                                   g_thread_self() );
	
	if( fth->p != NULL )
		fth->mutex_line = fth->p->mutex_line;
	else
		fth->mutex_line = g_mutex_new();
	
	g_mutex_lock( fth->mutex );
}

void thsys_deletevalues( fThread* fth ) {
	/*Remove from lists*/
	fth->parallel.next->parallel.prev = fth->parallel.prev;
	fth->parallel.prev->parallel.next = fth->parallel.next;
	fth->series.next->series.prev = fth->series.prev;
	fth->series.prev->series.next = fth->series.next;
	
	g_mutex_unlock(fth->mutex);
	g_mutex_unlock(fth->mutexp);
	g_mutex_unlock(fth->mutex_wait);
	
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
	g_mutex_lock( fth->mutex_line );
	list_insert( fth->p, fth, series )
	fth->ltype = SERIES;
	fth->call_mode = SERIES;
	g_mutex_unlock( fth->mutex_line );
	g_mutex_lock( fth->mutexp );
	
	if( this->ps == NULL )
		fth->ps = this;
	else
		fth->ps = this->ps;
	
	fth->ps->series_num++;
	
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
	
	g_mutex_lock( fth->mutex_line );
	list_insert( fth->p, fth, parallel )
	fth->ltype = PARALLEL;
	fth->call_mode = PARALLEL;
	g_mutex_unlock( fth->mutex_line );
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
    GTimer* master_timer;
    gdouble time;
	
	this = thsyshash_get();

    if( value < 0 )
        g_timer_start( this->timer );
    
	/* Wait the `parallel` thread finish, 
	   synchronize this point. */
	if( this->parallel.next != this ) {
		if(  this->ltype != this->call_mode 
			&& this->pp != NULL )
			synchronize(SERIES);
		else
			synchronize(PARALLEL);
	}
	
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
 			if( this->ltype != this->call_mode &&
 				this->ps != NULL )
 				thsys_step(PARALLEL);
 			else
				thsys_step(SERIES);
			}
			thsys_fps();
	}
	
    while( this->remaining_frames > 0 ) {
        
        if( this->series.next == this) 
            break;
        
        if( this->ltype != this->call_mode &&
            this->ps != NULL )
            thsys_step(PARALLEL);
        else
            thsys_step(SERIES);
        
        thsys_fps();
        
        this->remaining_frames--;
    }
    
    while( this->remaining_time > 0 ) {
        if( this->series.next == this) 
            break;
        
        if( this->ltype != this->call_mode &&
            this->ps != NULL )
            thsys_step(PARALLEL);
        else
            thsys_step(SERIES);
        
        g_timer_stop( this->timer );
        this->remaining_time -= 
            (g_timer_elapsed(this->timer, NULL) * mod(time_vel));
        g_timer_start( this->timer );
        thsys_fps();
    }
	
	if( this->remaining_time > 0 )
        g_timer_stop( this->timer );
	
	/* Left 'parallel' thread runs,
	 * synchronize this point. */
	if( this->parallel.next != this ) {
		if( this->pp != NULL &&
			this->ltype != this->call_mode )
			synchronize(SERIES);
		else
			synchronize(PARALLEL);
	}
	
	for( l = this->coming; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
	}
	
}

/* FIXME: The list made ​​to run in series
 * to operate in parallel */
void waitp() {
    GTimer* master_timer;
    gdouble time;
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
	
	thsys_fps();
	
}

void waits( double value ) {
    GTimer* master_timer;
    gdouble time;
	fThread* this;
	GList* l;
	
	this = thsyshash_get();
	
    if( value < 0 )
        g_timer_start( this->timer );
    
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
	
	while( this->remaining_time > 0 ) {
        if( this->series.next == this) 
            break;
        
            thsys_step(PARALLEL);
            thsys_step(SERIES);
        
        g_timer_stop( this->timer );
        this->remaining_time -= g_timer_elapsed(this->timer, NULL);
        g_timer_start( this->timer );
    }
	
	for( l = this->coming; l != NULL; l = l->next ) {
		FEVENTFUNCTION(l->data)->function(
			FEVENTFUNCTION(l->data)->data,
			this );
	}

    /* Limits the number of cycles
    * per second FPS_MAX */
    master_timer = f_data_get(this, "MAX_TIMER");
    
    if( !master_timer ) {
        master_timer = g_timer_new();
        f_data_connect( this, "MAX_TIMER",
                        master_timer );
        g_timer_start( master_timer );
        
        time_step = 1;
        if( time_vel == 0 )
            time_vel = 1;
    } else {
        g_timer_stop( master_timer );
        time = g_timer_elapsed( master_timer, 
                            NULL );
        
        time_step = (time * 20) * time_vel;

        if( FPS_MAX > 0 ) {
            time = (1. / FPS_MAX) - time;

            if( time > 0 ) {
                SDL_Delay( time * 1000 );
                time_step += (time*20*time_vel);
            }
        }

        g_timer_start( master_timer );
    }
    thsys_fps();
}

void synchronize( ListType mode ) {
	fThread* this;
	fThread *l, *l2;
	fThread* master;
	GMutex* mutex;
	GCond* cond;
	GMutex* tmutex;
	GCond* tcond;
	unsigned int* count;
	unsigned int* num;
	
	this = thsyshash_get();
	
	if( mode == NONE || mode == PARALLEL ) {
		g_mutex_lock( this->mutex_line );
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
		g_mutex_unlock( this->mutex_line );
	}
	else {
		g_mutex_lock( this->mutex_line );
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
		g_mutex_unlock( this->mutex_line );
	}
	
    if( master != NULL ) {
        g_mutex_lock( this->mutex_line );
        g_mutex_lock( master->mutex_data );
        (*count)++;
        g_mutex_unlock( master->mutex_data );
        g_mutex_lock(mutex);
        g_cond_signal(cond);
        g_mutex_unlock(mutex);
        g_mutex_unlock( this->mutex_line );
        g_cond_wait( tcond, tmutex );
    } else {
        
        /*It awakens all the threads to ensure that
         * when they get to this point are all active.
         * If not we will be waiting for them when
         * they expect never finished.*/
        g_mutex_lock( this->mutex_line );
        if( mode == SERIES )
            l2 = this->series.next;
        else 
            l2 = this;
        g_mutex_unlock( this->mutex_line );
        
        while( l2 != this ) {
            g_cond_signal( l2->cond );
            l2 = this->series.next;
         }
        
        /* Only continue when all the
         * threads reach this point. */
        while( 1 ) {
            g_mutex_lock( this->mutex_line );
            g_mutex_lock( this->mutex_data );
            if( (*count) >= (*num) ) {
                g_mutex_unlock( this->mutex_data );
                break;
            }
            g_mutex_unlock( this->mutex_data );
            g_mutex_unlock( this->mutex_line );
            g_cond_wait( tcond, tmutex );
        }

        g_mutex_lock( this->mutex_data );
        (*count) = 0;
        g_mutex_unlock( this->mutex_data );
        
        g_mutex_unlock( this->mutex_line );
        g_mutex_lock( this->mutex_line );
        
        if( mode == NONE || mode == PARALLEL )
            l = this->parallel.next;
        else
            l = this->series.next;

        g_mutex_unlock( this->mutex_line );
        
        while( l != this ) {
            
            if( mode == NONE || mode == PARALLEL ) {
                g_mutex_lock( this->mutex_line );
                l->call_mode = PARALLEL;
                g_mutex_lock(l->mutexp);
                g_cond_signal(l->condp);
                g_mutex_unlock(l->mutexp);
                l = l->parallel.next;
                g_mutex_unlock( this->mutex_line );
            }
            else {
                g_mutex_lock( this->mutex_line );
                l->call_mode = PARALLEL;
                g_mutex_lock(l->mutex);
                g_cond_signal(l->cond);
                g_mutex_unlock(l->mutex);
                l = l->series.next;
                g_mutex_unlock( this->mutex_line );
            }
        }
        
    }
}

void thsys_step( ListType mode ) {
	fThread* this;
	fThread* next;
        
	this = thsyshash_get();

	if( mode == SERIES ) {
		g_mutex_lock( this->mutex_line );
		next = this->series.next;
		next->call_mode = SERIES;

		if( next->removeme == TRUE )
			next->removeme = REMOVED;

		g_mutex_lock( next->mutex );
		g_cond_signal( next->cond );
		g_mutex_unlock( next->mutex );

		g_mutex_lock( next->mutex_data );
		g_mutex_unlock( this->mutex_line );
		if( next->removeme == FALSE ) {
			g_mutex_unlock( next->mutex_data );
			if( this->removeme != REMOVED )
				g_cond_wait( this->cond, this->mutex );
		} else {
			g_mutex_unlock( next->mutex_data );

			thsys_remove( next );
			g_mutex_lock( this->mutex_line );
			next = this->series.next;

			g_mutex_lock( next->mutex );
			g_cond_signal( next->cond );
			g_mutex_unlock( next->mutex );
			g_mutex_unlock( this->mutex_line );
			if( this->removeme != REMOVED )
				g_cond_wait( this->cond, this->mutex );

		}
	}
	else {
		g_mutex_lock( this->mutex_line );
		next = this->parallel.next;
		next->call_mode = SERIES;
		g_mutex_lock( next->mutexp );
		g_cond_signal( next->condp );
		g_mutex_unlock( next->mutexp );
		g_mutex_unlock( this->mutex_line );
		g_cond_wait( this->condp, this->mutexp );
	}

// #if HAVE_SDL

// #endif

}


fThread* series_insert( fThread* th, fThread* it ) {
	GList* l;
	g_mutex_lock( th->mutex_line );
	l = f_data_get( it, "SERIES" );
	l = g_list_prepend( l, &(it->series) );
	f_data_connect( it, "SERIES", l );
	it->series.next->series.prev = it->series.prev;
	it->series.prev->series.next = it->series.next;
    g_mutex_lock( it->series.next->mutex );
    g_cond_signal( it->series.next->cond );
    g_mutex_unlock( it->series.next->mutex );
	list_insert( th, it, series )
	g_mutex_unlock( th->mutex_line );
}

/*FIXME: Reactivate threads in parallel,
 * as well as in series.  */
fThread* parallel_insert( fThread* th, fThread* it ) {
	GList* l;
	g_mutex_lock( th->mutex_line );
	l = f_data_get( it, "PARALLEL" );
	l = g_list_prepend( l, &(it->parallel) );
	f_data_connect( it, "PARALLEL", l );
	it->parallel.next->parallel.prev = it->parallel.prev;
	it->parallel.prev->parallel.next = it->parallel.next;
	list_insert( th, it, parallel )
	g_mutex_unlock( th->mutex_line );
}

void series_restore( fThread* th ) {
	GList* l;
	g_mutex_lock( th->mutex_line );
	l = f_data_get( th, "SERIES" );
    if( l != NULL ) {
        th->series.next->series.prev = th->series.prev;
        th->series.prev->series.next = th->series.next;
        g_memmove(&(th->series), l->data, sizeof(th->series) );
        th->series.next->series.prev = th;
        th->series.prev->series.next = th;
        l = g_list_delete_link( l, l );
        f_data_connect( th, "SERIES", l );
    }
	g_mutex_unlock( th->mutex_line );
}

void parallel_restore( fThread* th ) {
	GList* l;
	g_mutex_lock( th->mutex_line );
	l = f_data_get( th, "PARALLEL" );
    if( l != NULL ) {
        th->parallel.next->parallel.prev = th->parallel.prev;
        th->parallel.prev->parallel.next = th->parallel.next;
        g_memmove(&(th->parallel), l->data, sizeof(th->parallel) );
        th->parallel.next->parallel.prev = th;
        th->parallel.prev->parallel.next = th;
        l = g_list_delete_link( l, l );
        f_data_connect( th, "PARALLEL", l );
    }
	g_mutex_unlock( th->mutex_line );
}

void wait_for( fThread* th ) {
    series_insert( th, this_thread );
    wait(1);
    series_restore(th);
}

void f_lock( gpointer obj ) {
    GMutex* m;
    
    thsys_init();
    
    if( !(m = f_data_get(obj, "MUTEX")) ) {
        m = g_mutex_new();
        f_data_connect( obj, "MUTEX", m );
    }
    
    g_mutex_lock(m);
}

void f_unlock( gpointer obj ) {
    GMutex* m;
    
    if( !(m = f_data_get(obj, "MUTEX")) ) {
        m = g_mutex_new();
        f_data_connect( obj, "MUTEX", m );
    }
    
    g_mutex_unlock(m);
}

fThread* parent_for( fThread* th ) {
    if( th->ps != NULL )
        return th->ps;
    
    return th;
}

void thsys_fps() {
    fThread* this = this_thread;
    GTimer* master_timer;
    double time;
    gulong time2;
#if TIME_WITH_SYS_TIME
    struct timespec t, t2;
#endif

    master_timer = f_data_get(this, "MAX_TIMER");
    
    if( !master_timer ) {
        master_timer = g_timer_new();
        f_data_connect( this, "MAX_TIMER",
                        master_timer );
        g_timer_start( master_timer );
        
        time_step = 1;
        if( time_vel == 0 )
            time_vel = 1;
    } else {
        g_timer_stop( master_timer );
        time = g_timer_elapsed( master_timer, 
                            NULL );
              
        time_step = (time * 20) * time_vel;
        
        FPS = time;
        
        if( FPS_MAX > 0 ) {
            time = (1. / FPS_MAX) - time;
            
            if( time > 0 )
                FPS += time;
            
            while( time > 0 ) {
#if TIME_WITH_SYS_TIME
                time2 = time * 1000000000;
                    
                if( time2 >= 1000000000 )
                    time2 = 100000000;
                
                t.tv_nsec = time2;
                t.tv_sec = 0;
                t2.tv_sec = 0;
                t2.tv_nsec = 0;
                nanosleep(&t, &t2);
                time -= ((double)(time2 - t2.tv_nsec)
                / 100000000);
#else
#if HAVE_SDL
                SDL_Delay( time * 1000 );
#endif
                time = 0;
#endif
            }
        }
        
        FPS = 1. / FPS;

        g_timer_start( master_timer );
    }
}