1/*
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

guint LEVEL = 0;

#define push(stack, elem)			\
  g_memmove( &(stack[stack_len]), &(elem),	\
	     sizeof(stack[stack_len]) );	\
  stack_len ++;

#define pop( stack )				\
  ({						\
    stack_len--;				\
    stack[stack_len+1];				\
  })

void 
f_tokenize( GScanner** scan, GString* str,
	    fToken** tokens, gsize* token_len ) 
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

void f_tree_free(fSyntaxTree* tree)
{
  if( tree == NULL )
    return;

  if( tree->child ) 
    {
      if( tree->child->left )
	f_tree_free( tree->child->left );
      f_tree_free(tree->child );
    }

  g_list_free(tree->tokens);
  g_free(tree);
}

void 
f_make_tree_scan( fSyntaxTree** tree, GScanner** scan,
		  GString* str, fToken** tokens,
		  gsize* token_len, fTokenInfo* info )
{
  guint i;
  guchar* tmp;

  if( info->moment == NULL )
    {
      //Create tree for function's
      for(; info->begin < info->end; info->begin++)
	{
	  if( *tokens[info->begin].type != 
	      G_TOKEN_IDENTIFIER ) continue;

	  tmp = *tokens[info->begin].v_identifier;

	  info->begin++;

	  if( info->begin >= info->end )
	    return;

	  if( *token[info->begin].type != '(' )
	    continue;

	  info->begin++;

	  if( info->begin >= info->end )
	    return;

	  for( ; info->begin < info->end &&
		 *tokens[info->begin].type != ')';
	       info->begin++);

	  if( *tokens[info->begin].type != ')')
	    continue;

	  info->begin++;

	  if( *token[info->begin].type != '{' )
	    continue;

	  i = 1;
	  info->begin++;

	  if( *tree == NULL)
	    *tree = g_malloc0(sizeof());
	  info->moment = *tree;

	  *tree->func.begin = info->begin - 1;
	  *tree->func.name = tmp;

	  for(; info->begin < info->end &&
		i != 0; info->begin++ )
	    {
	      if( *tokens[info->begin].type == '{')
		i++;
	      else if( *token[info->begin].type == '}')
		i--;

	      
	    }

	  if( i != 0 )
	    {
	      g_print("Syntax error: Missing '}' in:\n"
		      "\t%s\n", *tree->func.name );
	    }

	  *tree->func.end = info->begin;
	}
    }
}



void f_make_tree( fSyntaxTree** tree, GScanner** scan,
		  GString* str, fToken** tokens,
		  gsize* token_len  )
{
  gsize i, j;
  fToken* token;
  fSyntaxTree* t;
  gsize begin, end;
  fTokenInfo* info = 
    g_malloc0(sizeof(fTokenInfo));

  f_tokenize( scan, str, tokens, token_len );
  
  token = *tokens;

  if( *tree != NULL )
    f_tree_free(*tree);

  info->begin = 0;
  info->end = *token_len;
  info->moment = NULL;

  f_make_tree_scan( tree, scan, str, tokens,
		    token_len, info );
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
