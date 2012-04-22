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
#ifndef __UTILS_RED_BLACK_H__
#define __UTILS_RED_BLACK_H__ 1

#include <utils/utils.h>
#include <stdio.h>

#define RED 1
#define BLACK 0

/*!
 \brief Inserts a member, in a binary tree.
 \details This macro expands to the source code that inserts 
 into the tree. 
 For example: The smallest number is the left of the
 tree. The largest number (or equal) is the right of the tree.
 */
#define t_insert_custom( type, tree, member, data, redblack ) \
{\
       type *x, *y;\
\
        x = tree;\
\
        while (x != NULL) {\
                y = x;\
\
                if ( data->member < x->member )\
                        x = container_of( x->redblack.left, type, redblack);\
                else\
                        x = container_of( x->redblack.right, type, redblack);\
        }\
\
	data->redblack.p = &(y->redblack);\
	data->redblack.left = NULL;\
	data->redblack.right = NULL;\
\
        if ( data->member < y->member )\
                y->redblack.left = &(data->redblack);\
        else\
                y->redblack.right = &(data->redblack);\
}

/*!
 \brief Search a member, in a binary tree.
 */
#define t_search_custom( type, tree, member, data, redblack ) \
{\
       type *x;\
\
        x = tree;\
\
        while (x != NULL || x->member != data) {\
\
                if ( data < x->member )\
                        x = container_of( x->redblack.left, type, redblack);\
                else\
                        x = container_of( x->redblack.right, type, redblack);\
        }\
\
        return x;\
}
                
typedef struct _TRedBlack TRedBlack;

struct _TRedBlack{
  /* FALSE - Black
     TRUE - Red */
  char color;
  struct _TRedBlack* p;
  struct _TRedBlack* left;
  struct _TRedBlack* right;
};

void t_delete( TRedBlack* node );

/*!
 * \brief Called after insert a node on a tree
 * \details If you mess up the tree, will call this
 * function fixes it. Not only arranges the colors
 * to be a legitimate red-black tree, and also,
 * prevent the tree becomes unbalanced. For people
 * who have no certain knowledge of red-black trees,
 * I recommend calling this function after every 
 * insertion. From what I know, you can not call 
 * when you insert nodes underneath the red scroll
 * through black. One of the properties that the
 * red-black trees respect is should have the same
 * number of nodes black, in all paths of the leaf
 * nodes. For further clarification of what is a 
 * red-black tree, I recommend reading the red-black
 * section (page 123) trees of this book:
 * http://books.google.com.br/books?&id=NLngYyWFl_YC&oi=fnd&pg=PR13&ots=BxQpJz4kHc&sig=hT1PNiQNondtdWVe6uSl7ZAFdSg#v=onepage
 * If you do not know what a binary tree also read the
 * section that begins on page 253.
 */
void t_insert( TRedBlack** root, TRedBlack* x );
void t_rotate_right( TRedBlack** root, TRedBlack* y );
void t_rotate_left( TRedBlack** root, TRedBlack* x );

#endif