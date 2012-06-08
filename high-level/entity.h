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
#include <video/mesh.h>
#include <video/render.h>
#include <utils/data-connect.h>

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
			fMaterialPropStruct;
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
	fThread* thread;
};

#define me \
	((entity_get())[0])

#define my (*me)

#define you \
	(my.you)

#define your (*you)

fEntity** entity_get();

/*!
 * \brief Load a 3ds model from file
 * \param name Name of file to open
 * \param pos The initial position of model
 * \param fun The function name to represents
 *            loaded entity
 */
fEntity* ent_new( const char* name, fVector3 pos,
                  FCallback fun);

/*!
 * \brief Load a complete scene
 */
void scene_load( const char* name );
#endif