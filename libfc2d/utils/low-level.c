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

#include <utils/low-level.h>

void f_swap( gpointer a, gpointer b, gsize len ) {
    char *x = a, *y = b;
    char z;
    gsize i;
    
    for( i = 0; i < len; i++ ) {
        z = x[i];
        x[i] = y[i];
        y[i] = z;
    }
}

void _f_remove_of( char* c_begin, char* c_end,
                  char* p_end )
{
    char* c;
    
    f_data_connect( c_begin, 
                    "backup", 
                    g_memdup(c_begin, 
                             c_end - c_begin) );
       
    for( c = c_begin; c < c_end && 
        ((gsize)c_end + c) - c_begin < p_end; c++ )
    {
        *c = *((char*)(((gsize)c_end+c) - c_begin));
    }
    
    
}