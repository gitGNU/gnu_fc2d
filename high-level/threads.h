/*
FGameEngine - Complete tool kit for 3D games development.
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

#ifndef __HIGH_LEVEL_FTHREADS_H__
#define __HIGH_LEVEL_FTHREADS_H__ 1

#include <glib.h>
#include <utils/utils.h>
#include <utils/event-basis.h>

#ifndef G_THREADS_ENABLED
#error FGameEngine THSYS need glib with thread support to works
#endif

/*!
 * \file fthreads.h
 * This is the FGameEngine threads system.
 * Using this system you can add fthreads 
 * the two lines of fthreads(parallel and series).
 * To save programmer effort it is strongly
 * automated. Just create fthreads and call 
 * wait() function!
 */

#ifdef __cplusplus
extern "C" {
#endif

struct fThread;
typedef struct fThread fThread;

typedef struct {
	fThread* next;
	fThread* prev;
} fThreadl;

typedef enum {
	PARALLEL,
	SERIES,
	NONE
} ListType;

struct fThread {
	GThread* thread;
	ListType ltype;
    fThreadl parallel;
    fThreadl series;
    GMutex* mutex_all;
    GMutex* mutex;
	GMutex* mutex_line;
	GMutex* mutex_data;
	GMutex* mutexp;
	GMutex* mutex_wait;
    GCond* cond;
	GCond* condp;
	GTimer* timer;
	double remaining_time;
	int remaining_frames;
	GList* coming;
	GList* leaving;
	FEventFunction* evtf;
	GList* child_coming;
	GList* child_leaving;
	GHashTable* data;
	fThread* p; /*!< The thread that created this */
	fThread* pp; /*!< parent parallel */
	fThread* ps;/*!< parent series */
	ListType call_mode;
	unsigned int series_num;
	unsigned int parallel_num;
	unsigned int series_count;
	unsigned int parallel_count;
};

extern GHashTable* thsys_hash;
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
 *\brief Actions to be performed ​​when entering thread
 */
void thsys_addcoming( fThread* th, 
					  FCallback2 function, gpointer data );

/*! 
 *\brief Actions to be performed ​​when leaving thread
 */
void thsys_addleaving( fThread* th, 
					  FCallback2 function, gpointer data );

/*! 
 *\brief Actions to be performed ​​when entering thread
 */
void thsys_removecoming( fThread* th, 
					  FCallback2 function );

/*! 
 *\brief Actions to be performed ​​when leaving thread
 */
void thsys_removeleaving( fThread* th, 
					  FCallback2 function );

/*!
 * \brief Alloc memory and set default values
 */
void thsys_initvalues( fThread* fth );

/*!
 * \brief Free memory and delete it from lists
 */
void thsys_deletevalues( fThread* fth );

/*!\brief Get a unique fThread for some thread */
fThread* thsyshash_get1( GThread* th );

/*!\brief Get a unique fThread for this thread
   \note Never returns NULL*/
fThread* thsyshash_get();

/*!\brief Try to get a unique fThread for this thread
   \return The fthread. If no exists returns NULL. */
fThread* thsyshash_try_get();

/*! \brief Remove this thread from hash table
  * \warning Only use this function if you know
 * what you are doing. Misuse of it can 
 * compromise the internal state of 
 * FGameEngine. 
 * \note FGameEngine automatically deletes
 * fthreads */
void thsyshash_delete();

/*! \brief Remove some thread from hash table
  * \warning Only use this function if you know
 * what you are doing. Misuse of it can 
 * compromise the internal state of 
 * FGameEngine. 
 * \note FGameEngine automatically deletes
 * fthreads */
void thsyshash_delete1( GThread* th );

/*! 
 * \brief Simply start the fthreads system
 * \note You not have to call this function
 * to use fthreads system
 */
inline void thsys_init();

/*! \brief Adds a thread to run series to this
 * @return The identifier for new fThread 
 */
fThread* thsys_add( FCallback function, gpointer data );

/*! \brief Adds a thread to run parallel to this
 * \return The identifier for new fThread 
 */
fThread* thsys_addp( FCallback function, gpointer data );

/*! \brief Remove a thread from fthreads system
 * \return The identifier for new fThread 
 * \warning Only use this function if you know
 * what you are doing. Misuse of it can 
 * compromise the internal state of 
 * FGameEngine. 
 * \note FGameEngine automatically deletes
 * fthreads 
 */
gboolean thsys_remove(fThread* thread);

/*!
 * \brief This function lets other threads work.
 * \param value The number of cycles to wait if
 * positive. The number of second to wait if 
 * negative. 
 */
void wait( double value );

/*!
 * \brief This function lets other threads work,
 *        but no stop calling function.
 */
void waitp();

/*!
 * \brief While this function is running,
 *        nobody connected to it runs.
 */
void waits( double value );

/*!
 * \brief Wait until all threads are here.
 */
void synchronize( ListType mode );

void thsys_step( ListType mode );

#ifdef __cplusplus
}
#endif

#endif