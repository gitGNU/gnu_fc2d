/*
GNU FC2D - A two time dimensional programing language.
Copyright (C) 2012  Fabio J. Gonzalez <gonzalfj@ymail.com>

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
 *        GNU FC2D programming
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
#include <glib/gi18n.h>

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
    gchar* code_out; /* File to write code */
    gchar* bin_out; /* File to write binaries */
    GRand* rand; /* Name for temporary files */
    int compiler_status;
    gchar* params; /* For libraries and included files to compile */

    f_arg_add ("quiet", 'q',
               "Shows only errors that do program abort.", FALSE,
               FALSE);

    f_arg_add ("output", 'o',
               "The output for objects or programs.", FALSE,
               TRUE);

    f_arg_add ("make-object", 'c',
               "Compile only; do not assemble or link.", FALSE,
               FALSE);

    f_arg_add ("code-output", 's',
               "Write resulting source-code to specified file.", FALSE,
               TRUE);

    additional_information( TRUE,
			    FC2D_BUGREPORT,
                            FC2D_URL );

    f_arg_process( argc, argv,
                   "GNU FC2D "FC2D_VERSION,
                   "2012  Fabio J. Gonzalez <gonzalfj@ymail.com>",
                   "An interpreter for GNU FC2D.\nGNU FC2D is a"
                   " programing language that simulates\ntwo temporal"
                   " dimensions.\n\nThis program is part of the GNU project"
                   ",\nreleased under the aegis of GNU.",
                   "<files to interpret>" );

    rand = g_rand_new();
    if( f_args_len() < 1 )
    {
        f_arg_help();
        return -1;
    }

    if( f_arg_exists("make-object") &&
        !f_arg_exists("output") )
    {
        printf(_("To make a object, you need specify a output.\n"));
        return -1;
    }
    
    /* Joins all file into one string */
    str = g_string_new("");
    
    for( i = 0; i < f_args_len(); i++ ) 
    {
        b = g_file_get_contents( f_arg_get(i), &c, &len,
                                 &err );
        
        if(b)
        {
            str = g_string_append_len(str, c, len);
            g_free(c);
            c = NULL;
        } 
        else 
        {
            if( !f_arg_exists ("quiet") && err != NULL )
            {
                fprintf (stderr, "%s\n", err->message);
            }
            ret = -1;
        }
    }
    
    /* Get string containing source code and
     * interpret the string as a program */
    if( str && str->len > 0 ) 
    {    
        c = str->str;
        fc2d_process( &c, str->len );
        g_string_free(str, FALSE);
        
        if( f_arg_exists ("code-output") )
            code_out = f_option_get("code-output");
        else
        {
            str = g_string_new("");
            g_string_printf( str, "%s/fc2d-%u.c", g_get_tmp_dir(), 
                             g_rand_double(rand) * (~((guint)0)) );
            code_out = g_string_free( str, FALSE );
        }

        b = g_file_set_contents( code_out,
                                 c, -1, &err);

        if( !b )
        {
            if( err != NULL )
                printf ( "%s\n", err->message);
            g_remove(code_out);
            return -1;
        }
        
        if( f_arg_exists("output") )
            bin_out = f_option_get("output");
        else
        {
            str = g_string_new("");
            g_string_printf( str, "%s/fc2d-%u.bin", g_get_tmp_dir(), 
                             g_rand_double(rand) * (~((guint)0)) );
            bin_out = g_string_free( str, FALSE );
        }
        
        str = g_string_new("");
        
        /*Compiler flags*/
        b = g_spawn_command_line_sync ( "fc2d-config --libs --cflags",
                                        &params, NULL, NULL, &err);
        
        if( !b ) 
        {
            if( err != NULL )
                printf("%s\n", err->message);
            g_remove(code_out);
            
            printf( _("%s has been instaled?\n"), PACKAGE_NAME );
            
            return -1;
        }
        
        if( f_arg_exists("make-object") )
        {
            g_string_printf( str, "gcc -c -o %s %s %s", 
                             bin_out, code_out, params);
        } else 
        {
            g_string_printf( str, "gcc -o %s %s %s", 
                             bin_out, code_out, params);
        }
        
        /*Call gcc to compile code*/
        b = g_spawn_command_line_sync( g_string_free(str, FALSE), NULL,
                                       NULL, &compiler_status, &err );
        
        if( !f_arg_exists("code-output") )
            g_remove(code_out);
        
        if( !b ) 
        {
            if( err != NULL )
                printf("%s\n", err->message);
            
            return -1;
        }
        
        if( compiler_status != 0 )
            return -1;
        
        /* Simply run the program and delete it */
        if( !f_arg_exists("output") )
        {
            str = g_string_new(bin_out);
            g_string_printf( str, "fc2d-run %s", bin_out);
            
            b = g_spawn_command_line_sync( g_string_free(str, FALSE), NULL,
                                           NULL, NULL, &err );
            
            if( !b && err)
                printf("%s\n", err);
            
            g_remove(bin_out);
        }
    }
    
    return ret;
}
