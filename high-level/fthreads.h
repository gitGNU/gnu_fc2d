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
#include "utils/red-black.h"

#ifdef __cplusplus
extern "C" {
#endif

custom_list_struct( fThreadl, fThread );

struct fThread;
typedef struct fThread fThread;

struct fThread {
    fThreadl parallel;
    fThreadl series;
    GMutex* mutex_all;
    GMutex* mutex;
    GCond* cond;
    GThread* thread;

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

#define FAST_CUR_THREAD \
	static GThread* __current_thread = NULL;\
	\
	if( __current_thread == NULL )\
		__current_thread = g_thread_self();


#define THSYS_LOCK \
    if ( thsys_mutex == NULL )\
        thsys_init();\
\
	g_mutex_lock(thsys_mutex);


#define THSYS_UNLOCK \
	g_mutex_unlock(thsys_mutex);


/*!
 * \brief Add current thread to be processed(unstable)
 *        by threads system
 * \warning This macro can not be called by two
 *          processes simultaneously.
 */
#define thsys_add() \
{\
	FAST_CUR_THREAD\
	THSYS_LOCK\
	_thsys_add( __current_thread );\
	\
	THSYS_UNLOCK\
}

/*!
 * \brief Prepares the "thread system" to be used
 */
void thsys_init();

/*!
 * \brief Add current thread to be processed(unstable)
 *        by threads system
 * \warning This function is not thread safe.
 *          I recommend that you use the macro 
 *			thsys_add() instead of this function.
 */
void _thsys_add( GThread* this );


void thsys_addp( fThread* thread );
void thsys_remove();
void thsys_add_with_thread( FCallback* function, gpointer data );
void thsys_addp_with_thread( FCallback* function, gpointer data );
void thsys_remove_him(GThread* thread);
void thsys_remove_him_by_function(FCallback* function);

void _wait( float value );
void _waitp( float value );
void _waits( float value );

void fth_tree_insert( fThread* tree, fThread* data );
fThread* fth_tree_search( fThread* tree, GThread* data );

void fth_tree_insert_all( fThread* tree, fThread* data );
fThread* fth_tree_search_all( fThread* tree, GThread* data );

#ifdef __cplusplus
}
#endif
