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
    GCond* cond;
    GThread* thread;
	GTimer* timer;
	double remaining_time;
	int remaining_frames;

    /* red-black trees
     for current threads */
    TRedBlack rb1;
	
	/* red-black trees
     for all threads */
    TRedBlack rb2;

};

extern TRedBlack* current_threads;
extern GMutex* thsys_mutex;
extern fThread* thsys_last_inserted;
extern int FPS_MAX;

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
	g_mutex_wait(cond, thsys_mutex);
	
#define THSYS_WAIT2 \
	g_mutex_wait(this->cond, this->mutex_all);

/*!
 * \brief Add current thread to be processed
 *        by threads system
 */
#define thsys_add() \
{\
	THSYS_LOCK\
	__current_fthread = _thsys_add( __current_thread );\
	\
}

/*!
 * \brief Add current thread to be processed
 *        by threads system to runs simultane-
 *        ously with another thread.
 */
#define thsys_addp(thread) \
{\
	THSYS_LOCK\
	__current_fthread = _thsys_addp( __current_fthread, thread );\
	\
}

/*!
 * \brief Revove current thread from
 *        threads system.
 */
#define thsys_remove() \
\{\
	THSYS_LOCK\
	_thsys_remove( __current_fthread );\
	THSYS_UNLOCK\
\}


#define THREADED\
	FAST_CUR_THREAD \
	thsys_add();

#define THREADEDP( thread )\
	FAST_CUR_THREAD \
	thsys_addp( thread );

/*!
 * \brief Prepares the "thread system" to be used
 */
void thsys_init();

/*!
 * \brief Add a thread to be processed
 *        by threads system. 
 * \warning This function is not thread safe.
 *          I recommend that you use the macro 
 *			thsys_add() instead of this function.
 */
fThread* _thsys_add( GThread* this );

/*!
 * \brief Add a thread to be processed
 *        by threads system. Runs simultane-
 *        ously with another thread.
 * 
 * \warning This function is not thread safe.
 *          I recommend that you use the macro 
 *			thsys_addp() instead of this function.
 */
fThread* _thsys_addp( fThread* this, fThread* thread );

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
void thsys_add_with_thread( FCallback* function, gpointer data );
void thsys_addp_with_thread( FCallback* function, gpointer data );
void thsys_remove_him(GThread* thread);
void thsys_remove_him_by_function(FCallback* function);

void _wait( fThread* this, double value );
void _waitp( fThread* this, double value );
void _waits( fThread* this, double value );

void fth_tree_insert( fThread* tree, fThread* data );
fThread* fth_tree_search( fThread* tree, GThread* data );

void fth_tree_insert_all( fThread* tree, fThread* data );
fThread* fth_tree_search_all( fThread* tree, GThread* data );

#ifdef __cplusplus
}
#endif
