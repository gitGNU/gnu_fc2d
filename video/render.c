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
#include <video/render.h>
#include <video/window.h>
#include <high-level/threads.h>
#include <utils/Vector2.h>
#include <utils/data-connect.h>
#include <high-level/camera.h>

#if HAVE_ALSA
#include <audio/alsa.h>
#endif

#if HAVE_3D

#include <GL/gl.h>
#include <GL/glu.h>

gboolean f_render_reshape=FALSE;

const char* cartoon = 
"";

const char* normal = 
"";

void RenderScene( fWindow* w ) {
	GList* l = f_data_get( w, "MESHES" );
	fMesh* m;
	GList* p;
	fTriangle* tri;
    fCamera* cam;
    
    cam = current_camera;
	
	while( l != NULL ) {
		m = l->data;
		
		glBindTexture( GL_TEXTURE_2D, m->tex_id );
		
        /*Camera tranformations*/
        glTranslatef( -cam->x, -cam->z, -cam->y );
        glRotatef( -cam->rot.x, 0, 0, 1 );
        glRotatef( -cam->rot.z, 0, 1, 0 );
        glRotatef( -cam->rot.y, 1, 0, 0 );
        
 		glRotatef( m->rot.x, 0, 0, 1 );
 		glRotatef( m->rot.z, 0, 1, 0 );
 		glRotatef( m->rot.y, 1, 0, 0 );
        glTranslatef( m->pos.x / 500, m->pos.z / 500,
                      m->pos.y  / 500 );
        
		if( m->render_mode == RENDER_NORMAL )
			glUseProgram(0);
		else if( m->render_mode == RENDER_CARTOON )
			glUseProgram(1);
        
        glBegin(GL_TRIANGLES);
        glColor3f( m->mat_color.r, m->mat_color.g, m->mat_color.b);
		for( p = m->tri; p != NULL; p = p->next ) {
			tri = p->data;
			glTexCoord2f(fVector2M(&(tri->v1_tex)));
            glVertex3f(tri->v1.x * m->scale.x, 
                       tri->v1.z * m->scale.z,
                       tri->v1.y * m->scale.y
                      );
			glTexCoord2f(fVector2M(&(tri->v2_tex)));
            glVertex3f(tri->v2.x * m->scale.x, 
                       tri->v2.z * m->scale.z,
                       tri->v2.y * m->scale.y
            );
			glTexCoord2f(fVector2M(&(tri->v3_tex)));
            glVertex3f(tri->v3.x * m->scale.x, 
                       tri->v3.z * m->scale.z,
                       tri->v3.y * m->scale.y
            );
			
		}
		glEnd();
        
        glTranslatef( -m->pos.x  / 500, -m->pos.z  / 500,
                      -m->pos.y  / 500 );
        glRotatef( -m->rot.x, 0, 0, 1 );
        glRotatef( -m->rot.z, 0, 1, 0 );
        glRotatef( -m->rot.y, 1, 0, 0 );
        
        /*Camera tranformations*/
        glTranslatef( cam->x, cam->z, cam->y );
        glRotatef( cam->rot.x, 0, 0, 1 );
        glRotatef( cam->rot.z, 0, 1, 0 );
        glRotatef( cam->rot.y, 1, 0, 0 );
        
		l = l->next;
	}
}

void RenderGUI( fWindow* w ) {
	
}

void Reshape( fEvent* evt ) {
	fWindow* w = evt->obj;
	fExposeEvent* e = evt;
	fThread* th;
	
	w->width = e->width;
	w->height = e->height;
    
    f_render_reshape = TRUE;

}

void Render( fWindow* w ) {
    
#if HAVE_ALSA
	//thsys_addp( audio_output_mainloop, NULL );
	g_thread_create( audio_output_mainloop, NULL, TRUE, NULL );
#endif
	
	w->glc = glXCreateContext(w->display, w->vi, NULL, GL_TRUE);
	
	f_data_connect( w, "RENDER_THREAD", this_thread);

	window_set(w);
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
    
	glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource( 0, 1, &normal, NULL);
	glShaderSource( 1, 1, &cartoon, NULL);
	glCompileShader(0);
	glCompileShader(1);
	
	glCreateProgram();
	glAttachShader( 0, 0);
	glLinkProgram(0);
	
	glCreateProgram();
	glAttachShader( 1, 1);
	glLinkProgram(1);
	
	f_signal_connect_full( w, "expose-event", Reshape, NULL );
	
	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
	while(1) {
        glClear( GL_COLOR_BUFFER_BIT );
                
        if( f_render_reshape == TRUE ) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(60, w->width / w->height, 0, 20);
            glViewport( 0, 0,  w->width, w->height );
        }
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
		RenderScene(w);
        f_signal_emit_full( w, "render3D", w );
        
		RenderGUI(w);
        f_signal_emit_full( w, "render2D", w );        
		window_draw(w);
        
		wait(1);
	}
}

#endif