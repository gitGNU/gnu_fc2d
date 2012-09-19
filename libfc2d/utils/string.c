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

#include <libfc2d/utils/string.h>

gchar
f_string_get ( fString* str, gsize pos )
{
  fString* l;
  gsize i = 0;

  for ( l = str; l != NULL && pos - i > l->gstr->len;
        l = l->next ) i += l->gstr->len;
    

  if ( l != NULL )
    return l->gstr->str[pos - i];

  return 0;
}

fString*
f_string_set ( fString* str, gsize pos,
               gchar* value )
{
  fString *l, *l2;
  gsize i = 0;
  char* c;

  for ( l = str; l != NULL && pos - i >= l->gstr->len;
        l = l->next )
   {
            i += l->gstr->len;
            l2 = l;
   }
    
  if( str == NULL )
   {
      l = g_malloc0( sizeof(fString) );
      str = l;
   } 
  else if( l == NULL )
   { 
        l = l2;
   }

  l->gstr = g_string_overwrite( l->gstr, pos - i, value );
  
  return str;
}

void 
f_string_untrucate( fString* str ) 
{
    if( str->prev == NULL )
        return;
    
    str->gstr = g_string_append( str->prev, str );
    
    str->prev->next = str->next;
    str->next->prev = str->prev;
    
    g_string_free( str->gstr, TRUE );
}

fString*
f_string_truncate( fString* str, gsize len ) 
{
    fString* ret;
    
    ret = g_malloc0( sizeof(fString) );
    
    ret->gstr = g_string_truncate( str->gstr, len );
    
    ret->prev = str;
    ret->next = str->next;
    str->next = ret;
    
    return ret;
}

gboolean f_string_word_equal( fString* str, gsize* pos,
                              const char* c) 
{
    gsize i;
    char ch = 1;
    
    for( ; (ch == '\n' || ch == ' ') && ch != 0; *pos++ )
                ch = f_string_get( str, *pos );
    
    ch = f_string_get( str, *pos );
    for( i = 0; (ch != ' ' && ch != '\n') ||
        c[i] != 0; i++ ) 
     {
         ch = f_string_get( str, *pos + i );
         
         if( c[i] != ch )
            return FALSE;
     }
        
    return TRUE;
}

GString* 
f_string_word_get( fString* str, gsize* pos ) 
{
    char ch = 1;
    gsize i, len;
    gchar* ret;
    GString* gstr;
    
     for( ; (ch == '\n' || ch == ' ') && ch != 0; *pos++ )
                ch = f_string_get( str, *pos );
     
     ch = f_string_get( str, *pos );
     for( len = 0; (ch != '\n' && ch != ' ') && ch != 0;
         len++ ) 
      {
          ch = f_string_get( str, *pos + len );
      }
      
     ret = g_malloc( (len+1) * sizeof(gchar) );
     ret[len] = 0;
     
     for( i = 0; i < len; i++ ) 
      {
          ch = f_string_get( str, *pos + i );
          ret[i] = ch;
      }
      
     gstr = g_malloc(sizeof(GString));
     
     gstr->str = ret;
     gstr->len = len;
     gstr->allocated_len = len;

     return gstr;
}
