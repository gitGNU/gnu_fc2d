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

#include <high-level/entity.h>
#include <high-level/threads.h>
#include <video/window.h>
#if HAVE_LIB3DS
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#endif

fEntity** entity_get() {
	fEntity** ret;
	fThread* th;
	
	ret = f_data_get( this_thread, "ME" );
	
	if( ret != NULL )
		return ret;
	
	th = this_thread;
	while( ret == NULL && th->p != NULL ) {
		th = th->p;
		ret = f_data_get( this_thread, "ME");
	}
	
	if( ret != NULL ) {
		f_data_connect( this_thread, "ME", ret );
		return ret;
	}
	
	ret = g_malloc0(sizeof(fEntity*));
    
	f_data_connect( this_thread, "ME", ret);
	
	return ret;
}

fEntity* ent_new( const char* name, fVector3 pos,
                  FCallback fun, gpointer data)
{
#if HAVE_LIB3DS
    Lib3dsFile* file = lib3ds_file_load(name);
    fMesh* mesh;
    fEntity** ent = &mesh;
    Lib3dsMesh* mesh3ds;
    Lib3dsFace* face;
    fVector3* tri;
    guint i, j;
    fWindow* w;
    GList* l;
    fThread* thread;
    fEntity** p;
    fEntity* tmp;
    
    if( file == NULL )
        return NULL;
    
    mesh3ds = file->meshes;
    
    if( mesh3ds == NULL ) {
        fprintf(stderr, "No one mesh found\n");
        return NULL;
    }
    
    mesh = g_malloc0( sizeof(fEntity) );
    
    for( i = 0; i < mesh3ds->faces; i++ ) {
        tri = g_malloc0(sizeof(fTriangle));
        face = &(mesh3ds->faceL[i]);
        for( j = 0; j < 3; j++ ) {
            g_memmove( &(tri[j]), 
            &(mesh3ds->pointL[ face->points[j] ].pos),
            sizeof(fVector3)
            );
        }
        mesh->tri = g_list_prepend( mesh->tri, tri );
    }
    
    w = f_data_get( 0, "default-window" );
    
    if( w == NULL ) {
        fprintf(stderr, "No window found(%s:%d)\n",
                __FILE__, __LINE__);
        return NULL;
    }
    
    l = f_data_get( w, "MESHES" );
    
    l = g_list_prepend( l, mesh );
    
    f_data_connect( w, "MESHES", l );
    
    thread = thsys_add( fun, data );
    
    p = g_malloc0( sizeof(fEntity*) );
    *p = mesh;
    f_data_connect( thread, "ME", p );
    
    if( me != NULL ) {
        tmp = me;
        me = mesh;
        you = tmp;
        me = tmp;
    }
        
    
    return mesh;
#else
    fprintf(stderr, "Without lib3ds, I can`t
                    "load anything(%s:%d)\n",
                    __FILE__, __LINE__);
    return NULL;
#endif
    
}

void scene_load( const char* name );