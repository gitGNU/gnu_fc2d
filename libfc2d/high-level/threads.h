/*
GNU FC2D - A two time dimensional programing language.
Copyright (C) 2012  Free Software Foundation Inc.

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

#ifndef __LIBFC2D_HIGH_LEVEL_THREADS_H__
#define __LIBFC2D_HIGH_LEVEL_THREADS_H__ 1

#include <glib.h>
#include <utils/utils.h>
#include <utils/event-basis.h>
#include <utils/data-connect.h>

/*!
 * \file fthreads.h
 * This is the GNU FC2D threads system.
 * Using this system you can add threads.
 * To save programmer effort it is strongly
 * automated. Just create threads and call 
 * wait() function!
 */

#ifdef __cplusplus
extern "C" {
#endif /* It is C++ */

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
        fThread* p; /*!< The thread that created this */
        fThread* pp; /*!< parent parallel */
        fThread* ps; /*!< parent series */
        ListType call_mode;
        unsigned int series_num;
        unsigned int parallel_num;
        unsigned int series_count;
        unsigned int parallel_count;
    };

    extern GHashTable* thsys_hash;

#define this_thread \
	(thsyshash_get())

#define current_master \
    (parent_for(this_thread))

    /*!
     * \brief The fthreads system prevents that the
     *        number of frames per second, exceeds
     *        the specified value, doing the
     *        "wait" functions waiting the free time.
     * 
     * \details If you want to keep the number of frames
     *          per second stable independently on the
     *          speed of the machine and save processor,
     *          then set this variable to a value greater
     *          than zero and call wait* function whenever
     *          you want to correct the speed.
     */
#define FPS_MAX \
    (f_data_get_p( current_master, \
                    "FPS_MAX", \
                    double))

#define fps_max FPS_MAX

    /*!
     * \brief Duration of the last frame
     * 
     * \details If you want to keep the number of frames
     *          per second, stable without leaving the
     *          processor idle, simply by multiply this
     *          variable. This variable has the value
     *          1 when 20 frame per second, 2 when 10
     *          frames per second, etc
     */
#define TIME_STEP \
    (f_data_get_p( this_thread, \
                    "TIME_STEP", \
                    double))

#define time_step TIME_STEP

    /*!
     * \brief Use it to speed up and slow down 
     *        the program. This variable changes
     *        the time that the "wait" function wait
     *        (work in seconds only), and also modifies
     *        the value of the time_step variable.
     */
#define TIME_VELOCITY \
    (f_data_get_p( current_master, \
                    "TIME_VELOCITY", \
                    double))

#define time_vel TIME_VELOCITY

    /*!
     * \brief The number of times per second that
     *        this thread is awake
     */
#define FPS (f_data_get_p( current_master, \
                    "FPS", \
                    double))

#define fps FPS

/*! 
    *\brief Actions to be performed ​​when entering thread
    */
void thsys_addcoming(fThread* th,
        FCallback2 function, gpointer data);

/*! 
    *\brief Actions to be performed ​​when leaving thread
    */
void thsys_addleaving(fThread* th,
        FCallback2 function, gpointer data);

/*! 
    *\brief Actions to be performed ​​when entering thread
    */
void thsys_removecoming(fThread* th,
        FCallback2 function);

/*! 
    *\brief Actions to be performed ​​when leaving thread
    */
void thsys_removeleaving(fThread* th,
        FCallback2 function);

/*!
    * \brief Alloc memory and set default values
    */
void thsys_initvalues(fThread* fth);

/*!
    * \brief Free memory and delete it from lists
    */
void thsys_deletevalues(fThread* fth);

/*!\brief Get a unique fThread for some thread */
fThread* thsyshash_get1(GThread* th);

/*!\brief Get a unique fThread for this thread
    \note Never returns NULL*/
fThread* thsyshash_get();

/*!\brief Try to get a unique fThread for this thread
    \return The fthread. If no exists returns NULL. */
fThread* thsyshash_try_get();

/*! \brief Adds a thread to run series to this
    * @return The identifier for new fThread 
    */
fThread* thsys_add(FCallback function, gpointer data);

/*!
    * \brief This function lets other threads work.
    * \param value The number of cycles to wait if
    * positive. The number of second to wait if 
    * negative. 
    */
void wait(double value);

/*!
    * \brief Run one step of another threads 
    */
void thsys_step(ListType mode);

/*!
    * \brief Lock anything by it's pointer
    */
void f_lock(gpointer obj);

/*!
    * \brief Unlock anything by it's pointer 
    */
void f_unlock(gpointer obj);

fThread* parent_for(fThread* th);

/*!
    * \brief Wait for time requested by FPS_MAX
    *        and sets TIME_STEP
    *
    * \warning Call this function outside the proper
    *          context can prevent the variables to
    *          be adjusted correctly
    * 
    * \note If well used this function can be used
    *       to save CPU time.
    */
void thsys_fps();

/*!
    * \brief Wait for time requested by FPS_MAX
    * 
    * \details Similar to thsys_fps, but just wait
    *          does not change global values. You
    *          can use this function to wait for
    *          the appropriate time to maintain a
    *          specified fps_max. If you used a
    *          long time between calls to this
    *          function, it returns without waiting.
    * 
    * \note If well used this function can be used
    *       to save CPU time.
    */
void thsys_fps2();

#ifdef __cplusplus
}
#endif /* It is C++ */

#endif /* Not defined libfc2d/high-level/threads.h */