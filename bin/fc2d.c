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

#include <libfc2d/config.h>
#include "fc2d.h"

/* Processing command line arguments */
#include <libfc2d/utils/arguments.h>


int main( int argc, char** argv ) {
    
    f_arg_process( argc, argv,
                   PACKAGE_STRING,
                   "2012  Free Software Foundation Inc.",
                   "An interpreter for GNU FC2D.\nGNU FC2D is a"
                   " programing language that simulates\ntwo temporal"
                   " dimensions",
                   "<files to interpret>" );
    
    if( f_args_len() != 1 ) {
        f_arg_help();
        return -1;
    }
    
    return 0;
}