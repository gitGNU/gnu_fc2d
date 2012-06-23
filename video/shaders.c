/*
FDiamondEngine - Complete engine for 3D games development.
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

#include <utils/data-connect.h>
#include <video/window.h>
#include <video/mesh.h>
#include <glib.h>
#include <stdio.h>

guint32 shader_load( const char* name ) {
    fWindow* w;
    guint32 shader_num;
    const char* shader_prefix="F_SHADER:";
    GString* str;
    char* shader;
    guint32 shader_count=0;
    /*For load shader from file*/
    char** file;
    gsize lenght;
    gboolean readed;
    /*For set shader in OpenGL*/
    int compiled;
    int linked;
    gsize length;
    char* log;
    
    w = f_data_get(0, "default-window");
    
    if( w == NULL || name == NULL )
        return 0;
    
    str = g_string_new(shader_prefix);
    str = g_string_append( str, name );
    shader = g_string_free( str, FALSE );
    
    shader_num = f_data_get( w, shader );
    
    if( shader_num > 0 ) {
        g_free( shader );
        return shader_num;
    }
    
    file = g_malloc0(sizeof(char*));
    
    readed = g_file_get_contents( name, file,  &lenght,
                                  NULL);
    
    if( readed == FALSE ) {
        fprintf(stderr, "Unable to open shader:%s\n", name);
        g_free( shader );
        return 0;
    }
    
    //Aways NULL terminated string
    *file = g_realloc( *file, (lenght+1) * sizeof(char) );
    (*file)[lenght] = 0;
    
    
//     shader_count = f_data_get(w, "SHADERS_COUNT");
//     shader_count++;
//     f_data_connect( w,  "SHADERS_COUNT", shader_count);
    
    shader_count = glCreateShader(GL_VERTEX_SHADER);
    f_data_connect( w, shader, shader_count );

    glShaderSource( shader_count, 1, file, NULL);
    glCompileShader(shader_count);
    
    glGetShaderiv( shader_count, 
                   GL_COMPILE_STATUS,
                   &compiled );
    
    if( !compiled ) {
        glGetShaderiv( shader_count, 
                       GL_INFO_LOG_LENGTH,
                       &length );

        /*The sizeof(char), can change?*/
        log = g_malloc0(length * sizeof(char));
        glGetShaderInfoLog( shader_count, length,
                            &length, log );

        fprintf(stderr, "| Unable to compile shader:%s\n"
        "|-> %s\n|__________\n", name, log);
        /*No show again this message*/
        f_data_connect( w, shader, 1 );
    }
    
    glCreateProgram();
    glAttachShader( shader_count, shader_count );
    glLinkProgram(shader_count);
    
    return shader_count;
}

gboolean shader_set( const char* name ) {
    
    guint32 shader;
    
    if( (shader = shader_load(name)) > 0) {
        glUseProgram(shader);
        return TRUE;
    } else {
        glUseProgram( shader_load(RENDER_NORMAL) );
        return FALSE;
    }
    
}