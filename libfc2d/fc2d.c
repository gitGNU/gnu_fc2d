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
      (*tokens)[*token_len].pos = 
	(*scan)->text - (*str->str);
      (*tokens)[*token_len].index = *token_len;

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
  guint i, j;
  guchar* tmp;
  GList* l;
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

      info->moment->child =
	g_malloc0( sizeof(fSyntaxTree));

      info->child->moment = info->moment->child;

      for( i = info->begin; i < info->end; i++ )
	{
	  if( (*tokens)[i].type != G_TOKEN_IDENTIFIER ||
	      g_strcmp0
	      ( (*tokens)[i].value.v_string,"if")!=0 )
	    continue;

	  if( i+1 < info->end )
	    i++;
	  else break;

	  if( (*tokens)[i].type != G_TOKEN_IDENTIFIER )
	    continue;

	  if( g_strcmp0( (*tokens)[i].value.v_string,
			 "sometime" ) == 0 )
	    {
	      info->child->moment->cond.type =
		IF_SOMETIME;
	    }
	  else if( g_strcmp0( (*tokens)[i].value.v_string,
			      "always" ) == 0 )
	    {
	      info->child->moment->cond.type =
		IF_ALWAYS;
	    }
	  else if( g_strcmp0( (*tokens)[i].value.v_string,
			      "never" ) == 0 )
	    {
	      info->child->moment->cond.type =
		IF_NEVER;
	    }
	  else if( g_strcmp0( (*tokens)[i].value.v_string,
			      "custom" ) == 0 )
	    {
	      info->child->moment->cond.type =
		IF_CUSTOM;

	      if( i+1 < info->end )
		i++;

	      if( (*tokens)[i].type != '=' )
		{
		  g_print( _("fc2d error:%d:%d: Expected '='"
			     " sign after word custom."
			     "\tUse: custom=<function>\n"),
			   (*tokens)[i].line,
			   (*tokens)[i].column);
		  continue;
		}

	      if( i+1 < info->end )
		i++;
	      else break;

	      if( (*tokens)[i].type != G_TOKEN_IDENTIFIER )
		{
		  g_print( _("fc2d error:%d:%d:"
			     " Expected <function>"
			     " name(or this_function)"
			     "after '=' sign."
			     "\tUse: custom=<function>\n"),
			   (*tokens)[i].line,
			   (*tokens)[i].column);
		  continue;
		}

	      if( g_strcmp0( (*tokens)[i].value.v_string,
			     "this_function") == 0 )
		{
		  info->child->moment->cond.custom =
		    &(info->moment->func);
		}
	      else
		{
		  info->child->moment->cond.custom =
		    f_get_function( *tree,
				    (*tokens)[i].value.v_string );

		  if( info->child->moment->cond.custom 
		      == NULL )
		    {
		      g_print( _("fc2d error:%d:%d:"
				 "Function %s not found"
				 "Only be found this "
				 "function or functions"
				 " declared before this."
				 " You can not get"
				 " around this error "
				 "yet.\n"),
			       (*tokens)[i].line,
			       (*tokens)[i].column,
			       (*tokens)[i].value.v_string );
		      continue;
		    }
		}
	    }
	  else
	    continue;

	  if( i+1 < info->end )
	    i++;
	  else break;

	  if( (*tokens)[i].type != G_TOKEN_IDENTIFIER )
	    continue;

	  if(g_strcmp0( (*tokens)[i].value.v_string, "in")
	     != 0)
	    continue;

	  if( i+1 < info->end )
	    i++;
	  else break;

	  if( (*tokens)[i].type != G_TOKEN_IDENTIFIER )
	    continue;

	  if( g_strcmp0( (*tokens)[i].value.v_string,
			 "this_function") == 0 )
	    {
	      info->child->moment->cond.func =
		&(info->moment->func);
	    }
	  else
	    {
	      info->child->moment->cond.func =
		f_get_function(*tree,
			       (*tokens)[i].value.v_string);

	      if( info->child->moment->cond.func 
		  == NULL )
		{
		  g_print( _("fc2d error:%d:%d:"
			     "Function %s not found"
			     "Only be found this "
			     "function or functions"
			     " declared before this."
			     " You can not get"
			     " around this error "
			     "yet.\n"),
			   (*tokens)[i].line,
			   (*tokens)[i].column,
			   (*tokens)[i].value.v_string );
		  continue;
		}
	    }

	  if( i+1 < info->end )
	    i++;
	  else break;

	  if( (*tokens)[i].type != '(' )
	    {
	      g_print( _("fc2d error:%d:%d: missing "
			 "( after \"if\"\n"),
		       (*tokens)[i].line,
		       (*tokens)[i].column );
	      continue;
	    }

	  info->child->moment->cond.exp_begin = i;
	  if( i+1 < info->end )
	    i++;
	  else break;
	  j = 1;
 
	  for( ; i < info->end && j != 0; i++ )
	    {
	      if( (*tokens)[i].type == '(' )
		j++;
	      else if( (*tokens)[i].type == ')' )
		j--;

	      if( j == 0 )
		break;
	    }

	  if( j > 0 )
	    {
	      g_print("fc2d error in %s:"
		      "Missing %d ')'\n", 
		      info->moment->func.name, j);
	      break;
	    }
	  else if( j < 0 )
	    {
	      g_print("fc2d error in %s:"
		      "Missing %d '('\n", 
		      info->moment->func.name, j);
	      break;
	    }

	  info->child->moment->cond.exp_end = i;

	  if( i+1 < info->end )
	    i++;
	  else break;

	  if( (*tokens)[i].type != '{' )
	    {
	      g_print("error fc2d:%d:%d: Missing '{'\n",
		      (*tokens)[i].line,
		      (*tokens)[i].column );
	      continue;
	    }
	  
	  info->child->moment->cond.begin = i;

	  if( i+1 < info->end )
	    i++;
	  else break;

	  j = 1;

	  for(; i < info->end && j != 0; i++)
	    {
	      if( (*tokens)[i].type == '{' )
		j++;
	      else if( (*tokens)[i].type == '}' )
		j--;

	      if( j == 0 )
		break;
	    }

	  if( j > 0 )
	    {
	      g_print("fc2d error in %s:"
		      "Missing %d '}'\n", 
		      info->moment->func.name, j);
	      break;
	    }
	  else if( j < 0 )
	    {
	      g_print("fc2d error in %s:"
		      "Missing %d '{'\n", 
		      info->moment->func.name, j);
	      break;
	    }

	  info->child->moment->cond.end = i;

	  info->child->moment->type = TYPE_CONDITION;

	  if( i < info->end )
	    {
	      info->child->moment->left = 
		g_malloc0( sizeof(fSyntaxTree) );
	      info->child->moment =
		info->child->moment->left;
	    }
	}

      if( info->moment->child->type == TYPE_NONE )
	{
	  g_free( info->moment->child );
	  info->moment->child = NULL;
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
