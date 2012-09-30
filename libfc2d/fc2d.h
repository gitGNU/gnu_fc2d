/*
  GNU FC2D - A two time dimensional programing language.
  Copyright (C) 2012  Fabio J. Gonzalez <gonzalfj@ymail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the1
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*!
 * \file libfc2d/fc2d.h
 * 
 * \brief Library to process
 *        FC2D code.
 */

#ifndef __LIBFC2D_FC2D_H__
#define __LIBFC2D_FC2D_H__ 1

#include <glib.h>
#include <glib/gi18n.h>

extern guint LEVEL;
#define level LEVEL

typedef struct fSyntaxTree fSyntaxTree;

typedef float var;

typedef enum
  {
    COND_NONE=0,
    COND_TRUE,
    COND_FALSE
  } fCondTest;

typedef enum 
  {
    IF_SOMETIME,
    IF_NEVER,
    IF_ALWAYS,
    IF_CUSTOM
  } f2DCondType;

typedef struct {
  char* name;
  gsize begin;
  gsize end;
} fFunction;

typedef struct {
  f2DCondType type;

  /* It is not necessarily function in
     which "condition" this, plus the
     function for which the condition
     was assigned. */
  fFunction* func;

  fFunction* custom;

  gsize begin;
  gsize end;

  gsize exp_begin;
  gsize exp_end;

} fCondition;

typedef struct
{
  GTokenType type;
  GTokenValue value;
  guint line;
  guint column;
  gsize pos;
  /* The memory location used
     to access this "Token" */
  gsize index;
} fToken;

typedef struct
{
  union {
    gpointer pointer;
    unsigned long long value;
  };
} fObj;

typedef enum {
  TYPE_FUNCTION,
  TYPE_CONDITION,
  TYPE_BLOCK,
  TYPE_NONE=0
} fFieldType;

struct fTokenInfo;
typedef struct fTokenInfo fTokenInfo;

struct fTokenInfo
{
  gsize begin;
  gsize end;
  fSyntaxTree* moment;
  fTokenInfo* parent;
  fTokenInfo* child;
};

/*
  A tree that allows an arbitrary
  number of children.
   
  This tree come from syntatic
  analysis of code.
*/
struct fSyntaxTree
{
  union {
    fFunction func;
    fCondition cond;
  };

  fSyntaxTree* parent;
  fSyntaxTree* child; /* A pointer to one of it's children's */
  fSyntaxTree* left; /* pointer for other child from the
			same parent */

  fFieldType type;
  GList* tokens;

};

/* Get a fFunction structure by
   its name in tree*/
fFunction*
f_get_function( fSyntaxTree* tree,
		const char* name );

/*!
 * \brief Process GNU FC2D code and
 *        generate C code.
 * 
 * \param code Pointer to generated
 *             C source code.
 */
gboolean
fc2d_process( gchar** code, guint len,
	      gboolean shared );

gboolean
fc2d_run( gchar** code, guint len );

void echo(const char* msg);

#endif /* libfc2d/fc2d.h not included */
