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

#ifndef __UTILS_DATA_CONNECT__
#define __UTILS_DATA_CONNECT__ 1

#include <glib.h>

extern GHashTable* f_dataconnect_hash;

#define f_data_get_pp( obj, name, type ) \
    (((type**)(f_data_get_pointer((obj), (name),\
    sizeof(type))))[0])

#define f_data_get_p( obj, name, type ) \
    (*f_data_get_pp( obj, name, type ))

void f_data_connect(gpointer obj, const char* name,
        gpointer data);

void f_data_disconnect(gpointer obj, const char* name);

gpointer f_data_get(gpointer obj, const char* name);

gpointer* f_data_get_pointer(gpointer obj,
        const char* type_name,
        gsize len);
#endif