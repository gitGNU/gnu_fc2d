/*
FDiamondEngine - Complete engine for 3D games development.
Copyright (C) 2012  Fabio J. Gonzalez <fabiojosue@gmail.com>

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

#ifndef __UTILS_LOW_LEVEL_H__
#define __UTILS_LOW_LEVEL_H__ 1

#include <utils/data-connect.h>
#include <utils/utils.h>

/*! 
 * \file utils/low-level.h
 * 
 * \brief This file uses "low-level" access,
 *        it helps to have greater control of
 *        language and make a more dynamic,
 *        which can increase speed. But,
 *        more caution may be necessary to
 *        program.
 */

#define f_asm_prefix(name) "f_asm_" str(name) "_"
#define f_begin_prefix(name) f_asm_prefix(name) "begin"
#define f_end_prefix(name) f_asm_prefix(name) "end"
#define f_goto_prefix(name) f_asm_ ##name## _goto

/*!
 * \brief Defines a name for exactly this point
 *        in the function. You call this point
 *        without calling the entire function.
 */
#define function_subdivide(name) \
    asm(name ":")

/*!
 * \brief Returns the position in which
 *        the tag was added
 */
#define f_pointer_of(name)  \
({ \
        void* __f_funpos__; \
        asm( "mov %0, " name: \
        "r=" (__f_funpos__) ); \
        __f_funpos__;\
})

/*!
 * \brief defines begin of a block
 */
#define f_begin(name) \
    f_block_register(name); \
    function_subdivide( \
    f_begin_prefix(name) \
    )

    
/*!
 * \brief defines end of a block
 */
#define f_end(name) \
    function_subdivide(\
    f_end_prefix(name)\
    );
/*!
 * \brief Get the begin of block that anywhere
 *        in the program, provided it has been
 *        created before calling this macro.
 */
#define f_block_begin( name ) \
    ((gsize)(f_data_get(0, f_asm_prefix(name))))

/*!
 * \brief Get the end of block that anywhere
 *        in the program, provided it has
 *        been created before calling
 *        this macro.
 */
#define f_block_end( name ) \
    ((gsize)(f_data_get( f_block_begin(name),\
    f_end_prefix(name))))

/*!
 * \brief Gets block size
 * \return The block size
 *         in bytes
 */
#define f_block_size(name) \
    (gsize)(f_block_end(name) - f_block_begin(name))

/*!
 * \brief Swap information of two
 *        pointers For all these
 *        points
 * 
 * \example
 * 
 * c[x] = a[x];
 * a[x] = b[x];
 * b[x] = c[x];
 * 
 */
void f_swap( gpointer a, gpointer b, gsize len );

/*!
 * \brief Define a named block, allowing
 *        to get the beginning and end of
 *        the block.
 * 
 * \note This macro also prepares to run
 *          from other blocks, even if they
 *          are defined elsewhere. You can
 *          get this data from anywhere,
 *          provided that this macro has been
 *          called before the function that
 *          tries to get its data.
 * 
 * \param block Name of new block
 *
 * \param content The block centent
 * 
 * \return A pointer to new block
 */
#define f_block_new(name, content) \
({\
        f_begin(name);\
        content;\
        f_end(name);\
        f_block_begin( name ); \
})

#define f_goto_begin(name) \
    asm("call "f_begin_prefix(name) );

#define f_goto_end(name) \
    asm("call "f_end_prefix(name) );
    
#define f_block_register(name) \
    f_data_connect( 0, f_asm_prefix(name),\
                    f_pointer_of(f_begin_prefix(name)) );\
    \
    f_data_connect( f_pointer_of(f_begin_prefix(name)),\
                    f_end_prefix(name),\
                    f_pointer_of(f_end_prefix(name)) );

#define f_remove_of( child, parent ) \
    _f_remove_of( f_block_begin(child), \
                  f_block_end(child), \
                  f_block_end(parent) )

#define f_block_dup(name) \
    (g_memdup(f_block_begin(name), \
    f_block_size(name)))

void _f_remove_of( char* c_begin, char* c_end,
                  char* p_end );

#endif
