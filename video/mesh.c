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

#include <video/mesh.h>
#include <utils/utils.h>

void mesh_texture_set( fMesh* mesh, fImage* tex ) {
	static guint tex_id = 0;

	if( mesh->tex_id > 0 ) {
		glDeleteTextures( 1, &(mesh->tex_id) );
		tex_id--;
	}

	mesh->tex_id = (tex_id++);
	mesh->tex = tex;
	
	glBindTexture( GL_TEXTURE_2D, mesh->tex_id );
	
	glGenTextures( 1, &(mesh->tex_id) );

	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, tex->header.width,
					   tex->header.height, GL_RGBA, GL_FLOAT,
					tex->data );
	
}

void mesh_triangle_new( fMesh** mesh, fImage* tex ) {
	fTriangle* tri;
	
	if( *mesh == NULL )
		*mesh = g_malloc0(sizeof(fMesh));
	
	tri = g_malloc0(sizeof(fTriangle));
	tri->v1.x = -1;
	tri->v1.y = -1;
	tri->v1.z = -1;
	
	tri->v2_tex.x = 1;
	tri->v2_tex.y = 0;
	tri->v2.x = 1;
	tri->v2.y = -1;
	tri->v2.z = -1;
	
	tri->v2_tex.x = 0;
	tri->v2_tex.y = 1;
	tri->v3.x = -1;
	tri->v3.y = 1;
	tri->v3.z = -1;
	
	(*mesh)->tri = g_list_prepend( (*mesh)->tri, tri );
}