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

#if !HAVE_THREAD
guint LEVEL = 0;
#endif /*Multi-thread not is supported*/

#define push(stack, elem)			\
  g_memmove( &(stack[stack_len]), &(elem),	\
	     sizeof(stack[stack_len]) );	\
  stack_len ++;

#define pop( stack )				\
  ({						\
    stack_len--;				\
    stack[stack_len+1];			\
  })

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
	 str->str[*pos] == ' '  ||
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
  fProgram* p;
    
  interpreter = g_malloc0( sizeof(fInterpreter) );
    
  interpreter->hash = g_hash_table_new( g_direct_hash, 
					g_direct_equal );
    
  p = g_hash_table_lookup( interpreter->hash,
			   0 );
    
  if( p == NULL )
    {
      p = g_malloc0( sizeof(fProgram) );
      g_hash_table_insert( interpreter->hash, 
			   0,
			   p );
    }
    
  if( p->hash == NULL )
    {
      p->hash = g_hash_table_new( g_str_hash,
				  g_str_equal );
    }
    
  p->str = g_string_new( *source_code );
    
  fc2d_interprets_block( interpreter, 
			 0,
			 0 );
    
  *source_code = g_string_free( p->str, FALSE );
    
}

void
f_subst( GString* gstr, gsize pos, gsize len,
         gchar* str1, gchar* str2)
{
  GString* gstr1 = g_string_new( str1 );
  GString* gstr2 = g_string_new( str2 );
  gchar* c;
  gsize i;
  gchar back;
    
  c = &(gstr->str[pos]);
    
  for( i = 0; i < len && pos+i+gstr1->len < gstr->len; i++ )
    {
      back = c[i+gstr1->len];
      c[i+gstr1->len]=0;
        
      if( g_strcmp0( &(c[i]), gstr1->str) == 0 )
        {
	  c[i+gstr1->len] = back;
	  g_string_erase( gstr, pos+i, gstr1->len );
	  g_string_insert( gstr, pos+i, gstr2->str );
        } else 
        {
	  c[i+gstr1->len] = back;
        }
    }
}



gboolean
fc2d_interprets_block( fInterpreter* interpreter,
                       gsize pos,
                       gsize level ) 
{
  fProgram* p;
  GHashTable* h;
  gsize len;
  GString *gstr, *gstr2;
  gchar* c;
  gchar back;
  gsize i, a, b;
    
  p = g_hash_table_lookup( interpreter->hash, level );
    
  g_assert( p != NULL );
    
  while( pos < p->str->len )
    {
      for( len = pos; p->str->str[len] >= 'a' &&
	     p->str->str[len] <= 'z' &&
	     len+1 < p->str->len; len++ );
        
      if( p->str->str[len] == '(' )
        {
	  back = p->str->str[len];
	  p->str->str[len] = 0;
	  if( g_strcmp0( &(p->str->str[pos]), "subst" ) == 0 )
            {
	      p->str->str[len] = back;
	      c = &(p->str->str[len+1]);
                
	      for( i = 0; c[i] != ','; i++ );
                
	      back = c[i];
	      c[i]=0;
                
	      gstr = g_string_new(c);
                
	      c[i] = back;
	      c = &(c[i+1]);
                
	      for( i = 0; c[i] != ','; i++ );
                
	      back = c[i];
	      c[i] = 0;
                
	      gstr2 = g_string_new(c);
	      c[i] = back;
                
	      c = &(c[i+1]);
                
	      for( i = 0; c[i] != ','; i++ );
	      back = c[i];
	      c[i] = 0;
                
	      a = atoi(c);
                
	      c[i] = back;
	      c = &(c[i+1]);
                
	      for( i = 0; c[i] != ')'; i++ );
	      back = c[i];
	      c[i] = 0;
                
	      b = atoi(c);
                
	      c[i] = back;
                
	      c = &(p->str->str[a]);
	      len = b - a;
                
	      for( i = 0; i < len && a+i+gstr->len <= p->str->len; i++ )
                {
                    
		  back = c[i+gstr->len];
		  c[i+gstr->len]=0;
                    
		  if( g_strcmp0(&(c[i]), gstr->str) == 0 )
                    {
		      c[i+gstr->len] = back;
		      g_string_erase( p->str, a+i, gstr->len );
		      g_string_insert( p->str, a+i, gstr2->str );
                    } else 
                    {
		      c[i+gstr->len] = back;
                    }
                }
                
                
            }
	  else 
            {
	      p->str->str[len] = back;
	      back = p->str->str[len];
	      p->str->str[len] = 0;
	      printf("Undefined reference to %s\n", 
		     &(p->str->str[pos]) );
	      p->str->str[len] = back;
	      exit(1);
            }
        }
        
      while( pos < p->str->len && p->str->str[pos] != ';' ) pos++;
      pos++;
        
    }
    
  return TRUE;
}

void 
f_tokenize(GScanner** scan, GString* str, fToken** tokens,
	   gsize* token_len  ) 
{
  GTokenType* type;
  
  *token_len = 0;

  if( scan != NULL )
    g_scannet_destroy(*scan);

  if( *tokens != NULL )
    g_free(*tokens);

  g_assert(token_len != NULL);

  *tokens = g_malloc0(sizeof(fToken));

  scan = g_scanner_new( NULL );
  g_scanner_input_text( scan, *str->str, str->len);

  while(1)
    {
      type = g_scanner_get_next_token( scan );

      token[*token_len].type = type;
      token[*token_len].value = scan->value;
      token[*token_len].line = scan->line;
      token[*token_len].column = scan->position;
      token[*token_len].pos = scan->text - (*str->str);

      *token_len++;

      if( type == G_TOKEN_EOF )
	break;

      *tokens = g_realloc( *tokens, *token_len * sizeof(fToken) );
    }
}

gboolean 
fc2d_process( gchar** code, guint len )
{
  GScanner* scan;
  GTokenType type;
  fToken* token;
  gsize token_len=0;
  gsize i, j;
  GHashTable* hash;
  gpointer p;
  fObj* stack;
  gsize stack_len;
  stack = g_malloc0(len*sizeof(type));
  fObj obj;
  gpointer function_name;
  fFunction* f;
  GString* code_clone;

  hash = g_hash_table_new( g_str_hash, g_str_equal );

  code_clone = g_string_new_len(code, len);

  while(1)
    {
      f_tokenize( &scan, code_clone, &token, &token_len );

      for( i = 0; i < token_len; i++ )
	{
	  if( i+1 < token_len &&
	      token[i].type == G_TOKEN_IDENTIFIER && 
	      token[i+1].type == '(' )
	    {
	      for( j = i+1;  j <  token_len && token[j].type != ')'; j++ );
            
	      if( j == token_len ) 
		{
		  printf("fc2d error: Missing ')'\n"
			 "\tPremature end of file\n");
		}
            
	      j++;
	      if( j < token_len &&
		  token[j].type != '{' )
		break;
	      j++;

	      obj.value = j;
	      push(stack, obj);
	  
	      obj.value = i;
	      push(stack, i);

	      i = 0;

	      for( ; j < token_len; j++ )
		{
		  if( i == 0 && token[j].type == '}' )
		    {
		      i = pop(stack).value;
		  
		      obj.pointer = g_malloc(sizeof(fFunction));
		      ((fFunction*)(obj.pointer))->name =
			token[i].value.v_identifier;
		  
		      ((fFunction*)(obj.pointer))->end = j-1;
		      j = pop(stack).value;
		      ((fFunction*)(obj.pointer))->begin = j;
		  
		      g_hash_table_insert( hash, token[i].value.v_identifier,
					   obj.pointer );

		      function_name = token[i].value.v_identifier;
		      break;
		    }
		
		  if( token[j].type == '{' )
		    i++;
		  else if( token[j].type == '}' )
		    i--;
		}

	      f = g_hash_table_lookup(hash, function_name);
	      for( j = f->begin; j < token_len && j < f->end; )
		{
		  if( token[j].type == G_TOKEN_IDENTIFIER &&
		      g_strcmp0( token[j].value.v_identifier, "when" ) &&
		      j+1 < token_len )
		    {
		      j++;
		      if( token[j].type == '(' )
			{
			  obj.value = j;
			  push(stack, obj);

			  for( j++; j < token_len && 
				 token[j].type != ')'; j++ );

			  if( token[j-1].type == '(' ||
			      token[j].type != ')') 
			    {
			      printf("fc2d: Expected expression between PAREN's\n");
			      return FALSE;
			    }

			  j++;

			  if( j >= token_len || token[j].type != '{' ) 
			    {
			      printf("fc2d: Expected '{' after ')'\n");
			      return FALSE;
			    }

			  while( j < token_len && token[j].type != '}' ) j++;

			  if( token[j].type != '}' )
			    {
			      printf("Premature end of file\n");
			      return FALSE;
			    }

			  j = pop(stack).value;

			  break;
			}
		    } 
		  else
		    {
		      while( j < token_len && token[j] != ')' ) j++;
		      j++;
		    }
		}
	    }
	}
      if( i == token_len )
	break;
    }

  *code = code_clone->str;
    
  return TRUE;
}

void 
echo(const char* msg) 
{
  if( level == 0 )
    printf("%s\n", msg);
}
