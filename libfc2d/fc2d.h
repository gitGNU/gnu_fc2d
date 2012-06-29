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
 * \brief Include for interpreter
 *        of FC2D
 */

#ifndef __LIBFC2D_FC2D_H__
#define __LIBFC2D_FC2D_H__ 1

#include <libfc2d/utils/string.h>

struct fScriptCond;
typedef struct fScriptCond fScriptCond;

struct fScriptBlock;
typedef struct fScriptBlock fScriptBlock;

typedef enum 
    {
        TYPE_VAR='v',
        TYPE_DOUBLE='d',
        TYPE_CHAR='c',
        TYPE_INT='i',
        TYPE_FUNCTION='f'
    } fScriptType;

typedef struct
    {
        void* data;
        char type;
    } fScriptArg;

    
struct fScriptCond
    {
        gsize pos;
        fScriptCond* next;
    };

typedef struct 
{
    GString* str;
    fScriptCond* cond;
    GHashTable* hash;
    gsize main; /*Position of main function*/
    fScriptBlock* block;
} fInterpreter;
    
struct fScriptBlock 
{
    fScriptBlock* parent;
    GHashTable* hash;
};

/*!
 * \brief Allow call to C function
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
void fc2d_interprets_block( fInterpreter* interpreter,
                            gsize pos,
                            gsize level );


#endif /* libfc2d/fc2d.h not included */