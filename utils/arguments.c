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

#include <utils/arguments.h>
#include <glib.h>

typedef struct {
    char* name;
    char* value;
    char cut;
    char* description;
    gboolean required;
} fArgument;

typedef struct {
    char* full_name;
    char* copyright;
    char* description;
    char* syntax;
} fArgsInfo;

gint f_arg_process_name_compare( fArgument* arg, char* name ) {
    return g_strcmp0( arg->name, name );
}

void f_arg_process( int argc, char* argv[],
                 const char* full_name,
                 const char* copyright,
                 const char* description,
                 const char* syntax );

char* f_option_get( const char* arg );

char* f_arg_get( guint id );

guint f_args_len();

void f_arg_add( const char* arg, char cut,
                 const char* d, gboolean required )
{
    fArgument* a;
    GList* l;
    
    a = g_malloc0(sizeof(fArgument));
    
    l = f_data_get(0, "args");
    l = g_list_append( l, a );
    f_data_connect(0, "args", l);
    
    a->name = arg;
    a->cut = cut;
    a->description = d;
    a->required = required;
}

gboolean f_arg_exists( const char* arg ) {
    GList* l;
    
    l = f_data_get(0, "args");
    
    if( l == NULL )
        return FALSE;
    
    if( g_list_find_custom(l, arg, 
        f_arg_process_name_compare) != NULL )
        return TRUE;
    
    return FALSE;
}

void f_arg_help() {
    fArgsInfo* info;
    fArgs* args;
    
    info = f_data_get( 0, "args_info" );
    args = f_data_get( 0, "args" );
    
    printf("%s\n\n", info->full_name);
    printf("Copyright (C) %s\n", info->copyright);
    printf("License GPLv3+: GNU GPL version 3 or later"
    " <http://gnu.org/licenses/gpl.html>\n"
    "This is free software: you are free to change and"
    " redistribute it.\n There is NO WARRANTY, to the "
    "extent permitted by law.\n\n");
    
    printf("Syntax: %s\n%s\n\n", info->syntax, 
           info->description);
    
    printf("Commands:\n\n");
    
    
}