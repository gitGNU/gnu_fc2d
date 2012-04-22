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

void t_delete( TRedBlack* node ) {
  
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
