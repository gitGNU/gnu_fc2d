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
 * \brief A simple program to interpret the
 *        script using GNU FC2D library
 * 
 * \return If all is OK return 0, on any error
 *         to do, anything return -1
 * 
 * \file bin/fc2d.c
 * 
 */

#include <libfc2d/config.h>
#include <stdio.h>
#include "fc2d.h"

/* Processing command line arguments */
#include <libfc2d/utils/arguments.h>

int 
main( int argc, char** argv )
{
    
    int i;
    GString* str = NULL; /*For append files and left final source code*/
    gboolean b; /*For test if file as been loaded*/
    gchar* c = NULL; /*For the file content */
    gsize len; /* For file size */
    GError* err = NULL; /* For showing errors when loading file */
    int ret = 0;
    
    f_arg_add ("quiet", 'q',
               "Shows only errors that make the program abort.", FALSE,
               FALSE);
    
    f_arg_process( argc, argv,
                   PACKAGE_STRING,
                   "2012  Free Software Foundation Inc.",
                   "An interpreter for GNU FC2D.\nGNU FC2D is a"
                   " programing language that simulates\ntwo temporal"
                   " dimensions.\n\nThis program is part of the GNU project"
                   ",\nreleased under the aegis of GNU.",
                   "<files to interpret>" );
    
    if( f_args_len() < 1 )
    {
        f_arg_help();
        return -1;
    }
    
    /* Joins all file into one string */
    for( i = 1; i <= f_args_len(); i++ ) 
    {
        b = g_file_get_contents( f_arg_get(i), &c, &len, &err );
        
        if(b)
        {
            str = g_string_append_len(str, c, len);
            g_free(c);
        } 
        else 
        {
            if( !f_arg_exists ("quiet") )
            {
                fprintf (stderr, "%s\n", err->message);
            }
            ret = -1;
        }
    }
    
    /* Get string containing source code and
     * interpret the string as a program */
    if( str != NULL ) {
        c = g_string_free( str, FALSE );
        fc2d_interprets( &c );
    }
    
    return ret;
}