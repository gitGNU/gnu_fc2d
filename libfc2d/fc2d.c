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

#include <libfc2d/fc2d.h>
#include <glib.h>
#include <glib/gi18n.h>

gpointer
variable_get( fScriptBlock** block, 
              const char* name ) 
{
    fScriptBlock* b;
    gpointer ret;
    
    if( *block == NULL )
    {
        *block = g_malloc0( sizeof(fScriptBlock) );
        
        (*block)->hash = g_hash_table_new( g_str_hash,
                                           g_str_equal );
        
        return NULL;
    }
    
    for( b = *block; b != NULL; b = b->parent ) 
    {
        ret = g_hash_table_lookup(b, name);
        if( ret != NULL )
            return ret;
    }
    
    return NULL;
}

void 
check_len( GString* str, gsize pos ) 
{
    if( pos >= str->len ) 
    {
        printf( _("error: Premature end of file\n") );
        exit(1);
    }
}

void
jump_white( GString* str, gsize* pos ) 
{
    
    check_len( str, *pos );
    
    while( str->str[*pos] == '\n' ||
           str->str[*pos] == ' ' ||
           str->str[*pos] == '\t' ) 
    {
        *pos++;
    }
}

fScriptArg*
fc2d_interprets( gchar** source_code ) 
{
    fInterpreter* interpreter;
    gsize pos=0, len=0, i;
    GString* gstr;
    gchar back;
    fScriptArg* arg;
    
    interpreter = g_malloc0( sizeof(fInterpreter) );
    
    interpreter->str = g_string_new(*source_code);
    gstr = interpreter->str;
    interpreter->hash = 
    g_hash_table_new( g_str_hash, g_str_equal );
    
    /*First level: Outside any block( eg. function ) */
    
    for( pos = 0; pos < gstr->len; pos++ ) {
        
        jump_white(gstr, &pos);
        
        if( g_strcmp0( "var", gstr->str) < 0 )
        {
            pos+=3;
            
            jump_white(gstr, &pos);
            
            for( len = 0; gstr->str[pos+len] != ';' &&
                          gstr->str[pos+len] != '('; len++ );
            
            /* Temporarily truncate the string */
            back = gstr->str[len];
            gstr->str[len] = 0;
            
            if( gstr->str[pos+len] == ';' )
            {
                /* Type 'var' is other name to 'float' */
                arg = g_malloc0( sizeof(fScriptArg) );
                arg->data = g_malloc0( sizeof(float) );
                arg->type = TYPE_VAR;
            } else 
            {
                /* Function pointers is only beggining
                   of function code */
                arg = g_malloc0( sizeof(fScriptArg) );
                arg->data = g_malloc0( sizeof(gsize) );
                arg->type = TYPE_FUNCTION;
                ((gsize*)arg->data)[0] = pos+len;
                
                /* Ignore function data */
                pos = pos + len;
                
                while( gstr->str[pos] != '{' ) pos++;
                pos++;
                i++;
                
                for( ; pos < gstr->len && i != 0; pos++ ) 
                {
                    if( gstr->str[pos] == '{' ) i++;
                    if( gstr->str[pos] == '}' ) i--;
                }
                
                if( i < 0 ) 
                {
                    printf("error: missing \'{\'\n");
                    exit(1);
                } else if( i > 0 )
                {
                    printf("error: missing \'}\'\n");
                    exit(1);
                }
                
            }
            
            
            g_hash_table_insert( interpreter->hash,
                                 &(gstr->str[pos]), arg );
            
            /* Restore the string */
            gstr->str[len] = back;
        }
    }
    
    /* Second level and so on: Inside block( eg. function )
     * Jumps to main function and run it. 
     * 
     * Recursively entering and leaving blocks
     */
    arg = g_hash_table_lookup( interpreter->hash,
                               "main" );
    
    interpreter->main = ((gsize*)arg->data)[0];
    
    fc2d_interprets_block( interpreter, 
                           ((gsize*)arg->data)[0],
                           0 );
    
}

void
fc2d_interprets_block( fInterpreter* interpreter,
                       gsize pos,
                       gsize level ) 
{
    
}