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

#ifndef __VIDEO_MESH_H__
#define __VIDEO_MESH_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <video/image.h>
#include <utils/Vector3.h>
#include <utils/Vector2.h>
#include <video/render.h>
#include <glib.h>

typedef enum {
	RENDER_NORMAL=0,
	RENDER_CARTOON
} fRenderMode;

#define fMaterialPropStruct \
	struct { \
		float opacity;\
		float mirror;\
		float emission;\
		fColor mat_color;\
		fRenderMode render_mode;\
		guint shade_model;\
	}

#define fVector3M(v) \
	((v)->x), ((v)->y), ((v)->z)

typedef struct {
	fVector3 v1;
	fVector3 v2;
	fVector3 v3;
	fVector2 v1_tex;
	fVector2 v2_tex;
	fVector2 v3_tex;
} fTriangle;

typedef struct {
	fVector3 pos;
	fVector3 scale;
	fVector3 rot;
	fMaterialPropStruct;
	GList* tri;
	fImage* tex;
	guint tex_id;
} fMesh;

/*!
 * \brief texturize a mesh 
 */
void mesh_texture_set( fMesh* mesh, fImage* tex );

/*!
 * \brief Generate a triangle
 */
void mesh_triangle_new( fMesh** mesh, fImage* tex );


#ifdef __cplusplus
}
#endif

#endif