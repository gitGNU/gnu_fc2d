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

/*!
 * \file libfc2d/fc2d.h
 * 
 * \brief Library to process
 *        FC2D code.
 */

#ifndef __LIBFC2D_FC2D_H__
#define __LIBFC2D_FC2D_H__ 1

#include <libfc2d/utils/string.h>

#if HAVE_THREAD

#include <libfc2d/high-level/threads.h>

#define LEVEL \
    (f_data_get_p( current_master, \
                    "LEVEL", \
                    guint))
#else
extern guint LEVEL;
#endif /*Multi-thread not is supported*/

#define level LEVEL

struct fScriptCond;
typedef struct fScriptCond fScriptCond;

struct fScriptBlock;
typedef struct fScriptBlock fScriptBlock;

typedef var float;

typedef enum
{
    COND_NONE=0,
    COND_TRUE,
    COND_FALSE
} fCondTest;

typedef enum 
{
    TYPE_VAR='v',
    TYPE_DOUBLE='d',
    TYPE_CHAR='c',
    TYPE_INT='i',
    TYPE_FUNCTION='f'
} fScriptType;

typedef enum 
{
    IF_SOMETIME,
    IF_NEVER,
    IF_ALWAYS
} f2DCondType;

typedef struct {
  char* name;
  gsize begin;
  gsize end;
} fFunction;

typedef struct
{
    void* data;
    char type;
} fScriptArg;

    
struct fScriptCond
{
    gsize pos;
    fCondTest test_result;
    f2DCondType cond_type;
    fScriptCond* next;
};

typedef struct 
{
    GHashTable* hash;
} fInterpreter;

typedef struct 
{
    GString* str;
    fScriptCond* cond;
    GHashTable* hash;
    gsize main; /*Position of main function*/
    fScriptBlock* block;
    GHashTable* functions;
} fProgram;
    
struct fScriptBlock 
{
    fScriptBlock* parent;
    GHashTable* hash;
};

typedef struct
{
    GTokenType type;
    GTokenValue value;
    guint line;
    guint column;
    gsize pos;
} fToken;

typedef struct
{
  fScriptType type;
  union {
    gpointer pointer;
    unsigned long long value;
  };
} fObj;

/*!
 * \brief Allow call to C functions
 *        from script
 * 
 * \param level Position on second 
 *            cordenad of time
 * 
 * \param args Pointer to function arguments
 * 
 * \param type A string containing arguments
 *             types
 */
typedef fScriptArg* (*fCallbackScript)( gsize level,
                                        gpointer* args,
                                        char* type);

/*!
 * \brief Interprets FC2D programing language
 *        source
 * 
 * \param source_code pointer to string containing
 *                    the source. Can be modified
 *                    by script.
 * 
 * \return Value returned by script
 */
fScriptArg* fc2d_interprets( gchar** source_code );

/*!
 * \brief Execute  block and stop at end
 */
gboolean fc2d_interprets_block( fInterpreter* interpreter,
                                gsize pos,
                                gsize level );

/*!
 * \brief Process GNU FC2D code and
 *        generate C code.
 * 
 * \param code Pointer to generated
 *             C source code.
 */
gboolean fc2d_process( gchar** code, guint len );

void echo(const char* msg);

#endif /* libfc2d/fc2d.h not included */
