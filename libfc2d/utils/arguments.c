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

#include <utils/arguments.h>
#include <utils/data-connect.h>
#include <glib.h>
#include <stdlib.h>

typedef struct {
    char* name;
    char* value;
    char cut;
    char* description;
    gboolean required;
    gboolean value_required;
    gboolean exists;
} fArgument;

typedef struct {
    char* full_name;
    char* copyright;
    char* description;
    char* syntax;
    char* program_name;
} fArgsInfo;

gint f_arg_process_name_compare( fArgument* arg, char* name ) 
{
    return g_strcmp0( arg->name, name );
}

void 
f_arg_process( int argc, char* argv[],
                    const char* full_name,
                    const char* copyright,
                    const char* description,
                    const char* syntax ) 
{
    fArgsInfo* info;
    int i;
    GList* l;
    fArgument* arg;
    
    info = g_malloc0(sizeof(fArgsInfo));
    
    f_data_connect(0, "args_info", info);
    
    f_arg_add( "version", 'v',
               "Shows version of this software", FALSE, 
                FALSE );
    
    info->full_name = full_name;
    info->copyright = copyright;
    info->description = description;
    info->syntax = syntax;
    
    if( argc > 0 )
        info->program_name = argv[0];
    
    for( i = 1; i < argc; i++ ) {
        l = f_data_get(0, "args");
        
        if( strlen( argv[i] ) > 2 && argv[i][0] == '-' &&
            argv[i][1] == '-' ) {
            for( ; l != NULL; l = l->next ) {
                arg = l->data;
                if( g_strcmp0( argv[i][2], arg->name) == 0 ) {
                    arg->exists = TRUE;
                    if( i+1 < argc && arg->value_required == TRUE ) {
                        if( argv[i+1][0] != '-' ) {
                            arg->value = argv[i+1];
                            i++;
                        }
                        else {
                            f_arg_help();
                            exit(1);
                        }
                            
                    }
                }
            }
        } else if(  strlen( argv[i] ) == 2 && argv[i][0] == '-' ) {
                    for( ; l != NULL; l = l->next ) {
                        arg = l->data;
                        if( argv[i][1] == arg->cut) {
                            arg->exists = TRUE;
                            if( i+1 < argc && arg->value_required == TRUE ) {
                                if( argv[i+1][0] != '-' ) {
                                    arg->value = argv[i+1];
                                    i++;
                                }
                                else {
                                    f_arg_help();
                                    exit(1);
                                }
                                    
                            }
                        }
                    }
        } else {
            f_arg_help();
            exit(1);
        }
            
    }
    
    if( f_arg_exists("version") ) {
        printf("%s\n\n", info->full_name);
        printf("Copyright (C) %s\n", info->copyright);
        printf("License GPLv3+: GNU GPL version 3 or later"
        " <http://gnu.org/licenses/gpl.html>\n"
        "This is free software: you are free to change and"
        " redistribute it.\nThere is NO WARRANTY, to the "
        "extent permitted by law.\n\n");
        exit(0);
    }
}

char* f_option_get( const char* arg ) {
    GList* l;
    fArgument* args;
    
    l = f_data_get(0, "args");
    for( ; l != NULL; l=l->next ) {
        args = l->data;
        
        if( g_strcmp0(arg, args->name) )
            return args->value;
    }
    
    return NULL;
}

char* f_arg_get( guint id );

guint f_args_len();

void f_arg_add( const char* arg, char cut,
                 const char* d, gboolean required, 
                gboolean with_value )
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
    a->value_required = with_value;
}

gboolean f_arg_exists( const char* arg ) {
    GList* l;
    fArgument* arg;
    
    l = f_data_get(0, "args");
    
    if( l == NULL )
        return FALSE;
    
    arg = g_list_find_custom(l, arg, 
        f_arg_process_name_compare);
    
    if( arg && arg->exists )
        return TRUE;
    
    return FALSE;
}

void f_arg_help() {
    fArgsInfo* info;
    fArgument* args;
    GList* l;
    char* str[1000];
    int i, j;
    
    info = f_data_get( 0, "args_info" );
    l = f_data_get( 0, "args" );
    
    printf("%s\n\n", info->full_name);
    printf("Copyright (C) %s\n", info->copyright);
    printf("License GPLv3+: GNU GPL version 3 or later"
    " <http://gnu.org/licenses/gpl.html>\n"
    "This is free software: you are free to change and"
    " redistribute it.\nThere is NO WARRANTY, to the "
    "extent permitted by law.\n\n");
    
    printf("Syntax: %s ", info->program_name);
    
    l = f_data_get( 0, "args" );
    for( l = 0; l != NULL; l++ ) {
        args = l->data;
        
        if( !args->required )
            continue;
        
        if( args->value_required )
            printf("-%c <value>", args->cut);
        else
            printf("-%c ", args->cut);
    }
    
    printf("%s\n%s\n\n", info->syntax, info->description);
    
    printf("Options:\n\n");
    
    i = 0;
    l = f_data_get( 0, "args" );
    for( ; l != NULL; l = l->next ) {
        args = l->data;
        
        if( strlen(args->name) > i )
            i = strlen(args->name); 
    }
    
    i += 10;
    l = f_data_get( 0, "args" );
    for( ; l != NULL; l = l->next ) {
        sprintf(str, " -%c, --%s", args->cut, args->name);
        printf("%s\n", str);
        
        for( j = 0; j < i - strlen(str); j++) {
            printf(" ");
        }
        
        printf("%s\n", args->description);
    }
    
}