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

#ifndef __UTILS_UTILS_H__
#define __UTILS_UTILS_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define mod(x) \
	(x > 0? x : (-x))

#define FCALLBACK(x) \
	((FCallback)(x))
	
#define FCALLBACK2(x) \
	((FCallback2)(x))
/*!
 * \brief cast a member of a structure out to the containing structure
 * \param ptr the pointer to the member
 * \param type the type of the container struct this is embedded in.
 * \param member the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})


typedef void (*FCallback)(void* data);
typedef void (*FCallback2)(void* data, void* data2);
typedef int (*FCallbackCompare)( void* a, void* b );

typedef struct {
	FCallback function;
	void* data;
} Function;

#define custom_list(type) \
	type* next; \
	type* prev;


#define custom_list_struct( name, type )\
	typedef struct {\
		custom_list(type)\
	} name

	
/*!
 * \brief Insert a new member on a linked list
 * \param previ a old member of the list
 * \param this The new member being added to the list after the former
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