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

#include <high-level/camera.h>
#include <video/window.h>
#include <glib.h>

fCamera** f_get_camera() {
    fWindow* w = f_data_get( 0, "default-window" );
    fCamera** ret;
    
    if( w == NULL )
        exit(0);
    
    ret = f_data_get(w, "camera");
    
    if( ret == NULL ) {
        ret = g_malloc0( sizeof(fCamera*) );
        f_data_connect( w, "camera", ret );
    }
    
    return ret;
    
}