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

#ifndef __UTILS_ARGUMENTS_H__
#define __UTILS_ARGUMENTS_H__ 1

#include <glib.h>

/*!
 * \brief Obtain parameters from
 *        command line
 */
void f_arg_process(int argc, char** argv,
        const char* full_name,
        const char* copyright,
        const char* description,
        const char* syntax);

/*!
 * \brief Return argument value
 *        from argument names
 * 
 * \param arg The argument name
 * 
 * \return Value its argument
 */
char* f_option_get(const char* arg);

char* f_arg_get(guint id);

guint f_args_len();

/*!
 * \brief Adds a argument to help screen
 * 
 * \param arg The argument name
 * 
 * \param cut The short form to invoke
 *            this argument
 */
void f_arg_add(const char* arg, char cut,
        const char* d, gboolean required,
        gboolean with_value);

/*!
 * \brief Tests if this parameter was placed
 *        in command line
 * 
 * \param arg Argument name
 */
gboolean f_arg_exists(const char* arg);

/*!
 * \brief Show help
 */
void f_arg_help();

#endif