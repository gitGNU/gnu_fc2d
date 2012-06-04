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

#ifndef __HIGH_LEVEL_ENTITY_H__
#define __HIGH_LEVEL_ENTITY_H__ 1

#include <high-level/threads.h>
#include <utils/Vector3.h>
#include <video/image.h>
#include <utils/data-connect.h>

#define me \
	((fEntity*)(f_data_get(g_thread_self(), "ME")))

#define my me

#define you \
	(me->you)

#define your you

struct fEntity;
typedef struct fEntity fEntity;

struct fEntity {
	union {
		struct {
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
		};
		fMesh mesh;
	};
	
	union {
		struct {
			float red;
			float green;
			float blue;
		};
		fColor color;
	};
	
	float lightrange;
	fEntity* you;
};

#endif