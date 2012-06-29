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

#ifndef __UTILS_UTILS_H__
#define __UTILS_UTILS_H__ 1

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif


#define str(x) str2(x)
#define str2(x) #x

#define mod(x) \
	(x > 0? x : (-x))

#define FCALLBACK(x) \
	((FCallback)(x))

#define FCALLBACK2(x) \
	((FCallback2)(x))


#define inside( x, y, width, height, ax, ay ) \
    (((ax) >= (x) && \
     (ay) >= (y) && \
     (ax) < (width) && \
     (ay) < (height) ) ? \
     TRUE : FALSE)

    typedef void (*FCallback)(void* data);
    typedef void (*FCallback2)(void* data, void* data2);
    typedef int (*FCallbackCompare)(void* a, void* b);

    typedef struct {
        void* data;
        guint len;
    } fData;

    typedef struct {
        FCallback function;

        union {

            struct {
                void* data;
                guint len;
            };
            fData data_info;
        };
    } Function;

#define custom_list(type) \
	type* next; \
	type* prev;


#define custom_list_struct( name, type )\
	typedef struct {\
		custom_list(type)\
	} name

    /*! \brief Pointers to begin and end of lists */
    typedef struct {
        GList* first;
        GList* last;
    } GListBase;


/*!
 * \brief Insert a new member on a linked list
 
 * \param previ a old member of the list
 
 * \param this The new member being added to the list
 *             after the former
 
 * \param list the name of list member
*/
#define list_insert( previ, this, list ) \
	this->list.next = previ->list.next;\
	this->list.prev = previ;\
	previ->list.next = this;\
	previ->list.next->list.prev = this;

#ifdef __cplusplus
}
#endif

#endif