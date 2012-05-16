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

#ifndef G_THREADS_ENABLED
#error FGameEngine THSYS need glib with thread support to works
#endif

#include "utils/red-black.h"


#ifdef __cplusplus
extern "C" {
#endif

struct fThread;
typedef struct fThread fThread;

custom_list_struct( fThreadl, fThread );

struct fThread {
    fThreadl parallel;
    fThreadl series;
    GMutex* mutex_all;
    GMutex* mutex;
	GMutex* mutex_line;
	GMutex* mutexp;
	GMutex* mutex_wait;
    GCond* cond;
	GCond* condp;
	gpointer* data;
    GThread* thread;
	GTimer* timer;
	double remaining_time;
	int remaining_frames;
	FCallback func;
	int active;
	
	fThread* p; /*!< The thread that created this */

    /* red-black trees
     for current threads */
    TRedBlack rb1;
	
	/* red-black trees
     for all threads */
    TRedBlack rb2;

};

extern TRedBlack* current_threads;
extern GMutex* thsys_mutex;
extern fThread* thsys_root;
extern int FPS_MAX;

#define CFTHREAD fThread* __current_fthread

/*!
 * \brief This macro set __current_thread
 *        corresponding to the calling thread. 
 */
#define FAST_CUR_THREAD \
	static GThread* __current_thread = NULL;\
	static gboolean __current_thread_added = FALSE;\
	\
	if( __current_thread_added == FALSE ) {\
		__current_thread = g_thread_self(); \
		__current_thread_added = TRUE; \
	}


#define FAST_CUR_FTHREAD\
	static fThread* __current_fthread = NULL; \
	\
	if( __current_fthread == NULL ) \
		__current_fthread = fth_tree_search( \
		container_of(current_threads, fThread, rb1), \
		__current_thread);

#define THSYS_LOCK \
    if ( thsys_mutex == NULL )\
        thsys_init();\
\
	g_mutex_lock(thsys_mutex);


#define THSYS_UNLOCK \
	g_mutex_unlock(thsys_mutex);
	
#define THSYS_WAIT(cond) \
	g_cond_wait(cond, thsys_mutex);
	
#define THSYS_WAIT2 \
	g_cond_wait(this->cond, this->mutex_all);

/*!
 * \brief Add current thread to be processed
 *        by threads system
 */
#define thsys_add() \
{\
	THSYS_LOCK\
	__current_fthread = _thsys_add( __current_thread );\
	THSYS_UNLOCK\
	\
}

/*!
 * \brief Revove current thread from
 *        threads system.
 */
#define thsys_remove() \
{\
	THSYS_LOCK\
	_thsys_remove( __current_fthread );\
	THSYS_UNLOCK\
}


#define THREADED \
	fThread* __current_fthread = NULL; \
	thsys_init();\
	FAST_CUR_THREAD \
	thsys_add();
	
#define wait(value) \
	_wait( __current_fthread, value, 0 )

#define waits(value) \
	_waits( __current_fthread, value, 0 )
	
#define waitp() \
	_waitp( __current_fthread, 0 )
	

#define thsys_add_with_thread(function, data)\
	_thsys_add_with_thread( __current_fthread, function, data )

#define thsys_addp_with_thread(function, data)\
	_thsys_addp_with_thread( __current_fthread, function, data )


#define waitk( value, id ) \
	_wait( __current_fthread, value, id )

#define waitsk( value, id ) \
	_waits( __current_fthread, value, id )
	
#define waitpk( id ) \
	_waitp( __current_fthread, id )

#define CUR __current_fthread

/*!
 * \brief Prepares the "thread system" to be used
 */
void thsys_init();

/*!
 * \brief Add a thread to be processed
 *        by threads system. 
 * \warning This function is not thread safe.
 *          I recommend that you use the macro 
 *			THREADED instead of this function.
 */
fThread* _thsys_add( GThread* this );

/*!
 * \brief Revove a thread from
 *        threads system.
 * 
 * \param this A thread to be removed
 * \warning This function is not thread safe.
 *          I recommend that you use the macro 
 *			thsys_remove() instead of this function.
 */
void _thsys_remove( fThread* this );
gboolean _thsys_add_with_thread( fThread* parent, FCallback* function, gpointer data );
gboolean _thsys_addp_with_thread( fThread* parent, FCallback* function, gpointer data );
gboolean thsys_remove_him(GThread* thread);
gboolean thsys_remove_him_by_function(FCallback* function);
gboolean thsys_remove_him_by_fthread(fThread* thread);

void _wait( fThread* this, double value, int id );
void _waitp( fThread* this, int id );
void _waits( fThread* this, double value, int id );

void fth_tree_insert( fThread* tree, fThread* data );
fThread* fth_tree_search( fThread* tree, GThread* data );

void fth_tree_insert_all( fThread* tree, fThread* data );
fThread* fth_tree_search_all( fThread* tree, GThread* data );

#ifdef __cplusplus
}
#endif
