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

/*!
 * \file libfc2d/utils/string.h
 *
 * \brief Divide strings to provide
 *        fast insertion and deletion
 * 
 * \details The smaller the size of "fString"'s, the faster
 *          can insert. The larger, the slower will insert,
 *          but characters will get faster.
 */

#ifndef __LIBFC2D_UTILS_STRING_H__
#define __LIBFC2D_UTILS_STRING_H__ 1

#include <glib.h>

struct fString;
typedef struct fString fString;

struct fString
  {
    fString* next;
    fString* prev;
    GString* gstr;
  };

/*!
 * \brief Get a character of a fString
 *        using it's position
 *
 * \param str The string
 *
 * \param pos position of character
 *
 * \return The character
 */
gchar f_string_get ( fString* str, gsize pos );

/*!
 * \brief Set a character of a fString
 *        using it's position
 *
 * \param str The string
 *
 * \param pos Position of character
 *
 * \param value The character
 * 
 * \return New beginning of the list
 */
fString*
f_string_set ( fString* str, gsize pos,
                    gchar* value );

fString* f_string_truncate( fString* str, gsize len );

void f_string_untrucate( fString* str );

/*!
 * \brief Compare a fString with a normal C string
 */
gboolean f_string_word_equal( fString* str, gsize* pos,
                              const char* c);

GString* f_string_word_get( fString* str, gsize* pos );

#endif /* Not defined libfc2d/utils/string.h */
