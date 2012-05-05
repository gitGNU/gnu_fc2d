/*
FGameEngine - Complete tool kit for 3D games development.
Copyright (C) 2012  Fabio J. Gonzalez

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

#include "utils/red-black.h"
#include <glib.h>


void t_delete( TRedBlack** root, TRedBlack* x ) {
  
}

void t_insert( TRedBlack** root, TRedBlack* x ) {
	TRedBlack* y;
	
	x->color = RED;
  
	while( x != *root && x->p->color == RED ) {
		if( x->p == x->p->p->left ) {
			y = x->p->p->right;
			if( y->color == RED ) {
				x->p->color = BLACK;
				y->color = BLACK;
				x->p->p->color = RED;
				x = x->p->p;
			} else if( x == x->p->right ) {
				x = x->p;
				t_rotate_left(root, x);
			}
			x->p->color = BLACK;
			x->p->p->color = RED;
			t_rotate_right( root, x->p->p );
		} else { 
			y = x->p->p->left;
			if( y->color == RED ) {
				x->p->color = BLACK;
				y->color = BLACK;
				x->p->p->color = RED;
				x = x->p->p;
			} else if( x == x->p->left ) {
				x = x->p;
				t_rotate_right(root, x);
			}
			x->p->color = BLACK;
			x->p->p->color = RED;
			t_rotate_left( root, x->p->p );
		}
	}
	
	(*root)->color = BLACK;
  
}

void t_rotate_right( TRedBlack** root, TRedBlack* y ) {
  TRedBlack* x = y->left;

  x->p = y->p;
  y->p = x;
  y->left = x->right;
  x->right = y;
  
  if( y->left != NULL )
    y->left->p = y;
  
  if( x->p == NULL )
    *root = x;
  else if( x->p->left == y )
    x->p->left = x;
  else 
    x->p->right = x;
  
}

void t_rotate_left( TRedBlack** root, TRedBlack* x ) {
  TRedBlack* y = x->right;
  
  y->p = x->p;
  x->p = y;
  x->right = y->left;
  y->left = y;
  
  if( x->right != NULL )
    x->right->p = x;
  
  if( y->p == NULL )
    *root = y;
  else if( y->p->left == x )
    y->p->left = y;
  else 
    y->p->right = y;
}

TRRedBlack* rb_new( FCallbackCompare compare, void* data ) {
	TRRedBlack* x;
	
	x = g_try_new(TRRedBlack, 1);
	
	x->compare = compare;
	x->rb.data = data;
	x->rb.color = BLACK;
	x->rb.p = NULL;
	x->rb.left = NULL;
	x->rb.right = NULL;
	
	return x;
}

TRedBlack* rb_search( TRRedBlack** root, void* data ) {
	
	TRedBlack *x;
	
	x = (*root);
	
	while( x != NULL ) {
		
		if( (*root)->compare( data, x->data ) < 0 )
			x = x->left;
		else if( (*root)->compare( data, x->data ) > 0 )
			x = x->right;
		else
			return x;
	}
	
	return NULL;
}

void rb_insert( TRRedBlack** root, void* data ) {
	
	TRedBlack *x, *y;
	
	x = (*root);
	
	while( x != NULL ) {
		y = x;
		
		if( (*root)->compare( data, x->data ) < 0 )
			x = x->left;
		else if( (*root)->compare( data, x->data ) > 0 )
			x = x->right;
	}
	
	x = g_try_new0( TRedBlack, 1 );
	x->data = data;
	
	x->p = y;
	
	if( (*root)->compare( data, x->data ) < 0 )
		y->left = x;
	else
		y->right = x;
	
	t_insert( root, x );
	
}

void rb_delete( TRRedBlack** root, void* data ) {
	TRedBlack* x;
	
	x = rb_search( root, data );
	
	if( x != NULL ) {
		t_delete(root, x);
	}
}