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

#include <libfc2d/fc2d.h>


guint LEVEL;

#define push(stack, elem)			\
  g_memmove( &(stack[stack_len]), &(elem),	\
	     sizeof(stack[stack_len]) );	\
  stack_len ++;

#define pop( stack )				\
  ({						\
    stack_len--;				\
    stack[stack_len+1];				\
  })

fFunction*
f_get_function( fSyntaxTree* tree,
		const char* name )
{
  for(; tree != NULL; tree = tree->left)
    {
      if( g_strcmp0( tree->func.name, name)==0 )
	return &(tree->func);
    }

  return NULL;
}

void 
f_tokenize( GScanner** scan, GString* str,
	    fToken** tokens, gsize* token_len ) 
{
  GTokenType type;

  *token_len = 0;

  if( *scan != NULL )
    g_scanner_destroy(*scan);

  if( *tokens != NULL )
    g_free(*tokens);

  g_assert(token_len != NULL);

  *tokens = g_malloc0(sizeof(fToken));

  *scan = g_scanner_new( NULL );
  g_scanner_input_text( *scan, *str->str, str->len);

  while(1)
    {
      type = g_scanner_get_next_token( scan );

      (*tokens)[*token_len].type = type;
      (*tokens)[*token_len].value = (*scan)->value;
      (*tokens)[*token_len].line = (*scan)->line;
      (*tokens)[*token_len].column = (*scan)->position;
      (*tokens)[*token_len].pos = (*scan)->text - (*str->str);

      *token_len++;

      if( type == G_TOKEN_EOF )
	break;

      *tokens = g_realloc( *tokens, 
			   *token_len * sizeof(fToken) );
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
  GList* l;
  fToken* token;
  fSyntaxTree* t2;

  if( info->moment == NULL )
    {
      //Create tree for function's
      info->moment = *tree;
      for(; info->begin < info->end; info->begin++)
	{
	  if( (*tokens)[info->begin].type !=
	      G_TOKEN_IDENTIFIER ) continue;

	  tmp = (*tokens)[info->begin].value.v_identifier;

	  info->begin++;

	  if( info->begin >= info->end )
	    return;

	  if( (*tokens)[info->begin].type != '(' )
	    continue;

	  info->begin++;

	  if( info->begin >= info->end )
	    return;

	  for( ; info->begin < info->end &&
		 ((*tokens)[info->begin]).type != ')';
	       info->begin++);

	  if( (*tokens)[info->begin].type != ')')
	    continue;

	  info->begin++;

	  if( (*tokens)[info->begin].type != '{' )
	    continue;

	  i = 1;
	  info->begin++;

	  info->moment->func.begin = info->begin - 1;
	  info->moment->func.name = tmp;

	  info->moment->tokens = g_list_append
	    ( info->moment->tokens, 
	      &((*tokens)[info->begin-1]));

	  for(; info->begin < info->end &&
		i != 0; info->begin++ )
	    {
	      if( (*tokens)[info->begin].type == '{')
		i++;
	      else if( (*tokens)[info->begin].type == '}')
		i--;

	      info->moment->tokens = 
		g_list_append
		( info->moment->tokens, 
		  &(*tokens)[info->begin] );
	    }

	  if( i != 0 )
	    {
	      g_print("Syntax error: Missing '}' in:\n"
		      "\t%s\n", (*tree)->func.name );
	    }

	  info->moment->func.end = info->begin;

	  info->begin = info->moment->func.begin;
	  info->end = info->moment->func.end;
	  info->moment->type = TYPE_FUNCTION;

	  f_make_tree_scan( tree, scan, str, tokens,
			    token_len, info );

	  /*This "for" will run some more cycle?*/
	  if( info->begin+1 < info->end )
	    {
	      info->moment->left =
		g_malloc0(sizeof(fSyntaxTree));
	      info->moment = info->moment->left;
	    }
	}
    } 
  else if( info->moment->type == TYPE_FUNCTION )
    {
      /* Looking for conditions in a function */

      info->child = g_malloc0(sizeof(fTokenInfo));

      for( l=info->moment->tokens; l!=NULL; l=l->next)
	{
	  token = l->data;

	  if( token->type != G_TOKEN_IDENTIFIER ||
	      g_strcmp0(token->value.v_identifier,"if")!=0 )
	    continue;

	  l = l->next;
	  if( l == NULL )
	    break;
	  token = l->data;

	  if( token->type != G_TOKEN_IDENTIFIER ||
	      ( g_strcmp0( token->value.v_identifier,
			   "sometime" ) != 0 &&
		g_strcmp0( token->value.v_identifier,
			   "always" ) != 0 &&
		g_strcmp0( token->value.v_identifier,
			   "never" ) != 0 ) )
	    continue;

	  l = l->next;
	  if( l == NULL)
	    break;
	  token = l->data;

	  if( token->type == G_TOKEN_IDENTIFIER && 
	      g_strcmp0(token->value.v_identifier,
			"never") == 0 )
	    {
	      if( info->moment->child == NULL )
		{
		  info->moment->child =
		    g_malloc0( sizeof(fSyntaxTree) );
		  info->child->moment =
		    info->moment->child;
		}
	      else
		{
		  for( info->child->moment=
			 info->moment->child;
		       info->child->moment->left!=NULL; 
		       info->child->moment=
			 info->child->moment->left );

		  info->child->moment->left =
		    g_malloc0( sizeof(fSyntaxTree) );
		  info->child->moment =
		    info->child->moment->left;
		}
	    }
	     
	}
      g_free( info->child );
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

  *tree = g_malloc0(sizeof(fSyntaxTree));

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

  return TRUE;
}

void 
echo(const char* msg) 
{
  if( level == 0 )
    printf("%s\n", msg);
}
