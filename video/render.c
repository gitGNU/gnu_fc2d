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

#if HAVE_ALSA
#include <audio/alsa.h>
#endif

#if HAVE_3D

#include <GL/gl.h>

const char* cartoon = 
"";

const char* normal = 
"";

void RenderScene( fWindow* w ) {
	GList* l = f_data_get( w, "MESHES" );
	fMesh* m;
	GList* p;
	fTriangle* tri;
	
	while( l != NULL ) {
		m = l->data;
		
		glBindTexture( GL_TEXTURE_2D, m->tex_id );
		
		glRotatef( m->rot.x, 0, 0, 1 );
		glRotatef( m->rot.y, 0, 1, 0 );
		glRotatef( m->rot.z, 1, 0, 0 );
		glScalef( m->scale.x, m->scale.y, m->scale.z );
		glTranslatef( m->pos.x, m->pos.y, m->pos.z );
		
		if( m->render_mode == RENDER_NORMAL )
			glUseProgram(0);
		else if( m->render_mode == RENDER_CARTOON )
			glUseProgram(1);
		
		for( p = m->tri; p != NULL; p = p->next ) {
			tri = p->data;
			glBegin(GL_TRIANGLES);
			glTexCoord2f(fVector2M(&(tri->v1_tex)));
			glVertex3f(fVector3M(&(tri->v1)));
			glTexCoord2f(fVector2M(&(tri->v2_tex)));
			glVertex3f(fVector3M(&(tri->v2)));
			glTexCoord2f(fVector2M(&(tri->v3_tex)));
			glVertex3f(fVector3M(&(tri->v3)));
			glEnd();
		}
		
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
 		glViewport( 0, 0,  w->width, w->height );
		glClear( GL_COLOR_BUFFER_BIT );
 	 	
		RenderScene(w);
		RenderGUI(w);
		window_draw(w);
		wait(1);
	}
}

#endif