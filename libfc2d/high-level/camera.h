/*
 FDiamondEngine - Complete engine for 3D games development.
 Copyright (C) 2012  Fabio J. Gonzalez <fabiojosue@gmail.com>         *
 
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

#ifndef __HIGH_LEVEL_CAMERA_H__
#define __HIGH_LEVEL_CAMERA_H__ 1

#include <utils/Vector3.h>

typedef struct {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        fVector3 pos;
    };
    union {
        struct {
            float scale_x;
            float scale_y;
            float scale_z;
        };
        fVector3 scale;
    };
    union {
        struct {
            float pan;
            float tilt;
            float roll;
        };
        fVector3 rot;
    };
} fCamera;

fCamera** f_get_camera();

#define current_camera \
    ((f_get_camera())[0])
    
#define camera \
    (*current_camera)

#endif