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
#include <video/shaders.h>
#include <video/widgets.h>

#if HAVE_ALSA
#include <audio/alsa.h>
#endif

#if HAVE_3D

#include <GL/gl.h>
#include <GL/glu.h>

gboolean f_render_reshape=FALSE;

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

        glBegin(GL_TRIANGLES);
        glColor3f( m->mat_color.r, m->mat_color.g, m->mat_color.b);
		
        shader_set(m->render_mode);
        
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

void RenderWidget( fWidget* w, guint x, guint y ) {
    GList* m;
    fColor *px1, *px2;
    guint k, l, i, k2, l2;
    fImageHeader *img1, *img2;
    float alpha=1;
    
    for( m = w->childs; m != NULL; m = m->next ) {
        FWIDGET(m->data)->p = w;
        RenderWidget( m->data, x + w->x, y + w->y );
    }
    
    glTranslate2i( x, y );
    
    if( w->pixels ) {
        //Put some data in "fImage"
        f_signal_emit_full( w, "draw", w );
        img2 = w->pixels;
        
        if( w->p && w->p->pixels ) {
            img1 = w->p->pixels;
            
            for( k = 0; k < img2->width; k++ ) {
                for( l = 0; l < img2->height; l++ ) {
                    px2 = color((fImage*)img2, k, l, 0);
                    
                    k2 = x + ( ((float)k/img2->width) *
                         w->width);
                    l2 = y + ( ((float)l/img2->height) *
                         w->height);
                    
                    if( k2 < img1->width && l2 < img1->height ) {
                        px1 = color((fImage*)img1, k2, l2, 0);
                        if( img2->channels_num == 4 ) {
                            alpha = px2->alpha * 
                                   (1. - w->transp);
                        } else {
                            alpha = 1. - w->transp;
                        }
                        
                        px1->r = (px1->r*(1. - alpha)) + 
                               (px2->r*alpha);
                        px1->g = (px1->g*(1. - alpha)) + 
                               (px2->g*alpha);
                        px1->b = (px1->b*(1. - alpha)) + 
                               (px2->b*alpha);
                    }
                }
            }
            
        } else if( !(w->p) ) {
            if( img2->channels_num == 4 ) {
                glDrawPixels( img2->width, img2->height,
                    GL_RGBA, GL_FLOAT,
                    w->pixels->data );
            } else {
                glDrawPixels( img2->width, img2->height,
                    GL_RGB, GL_FLOAT,
                    w->pixels->data );
            }
        }
    } else {
        //Ask for someone to load anything
        f_signal_emit_full( w, "alloc-draw", w );
    }
    
    f_signal_emit_full( w, "render2D", w );
    
    glTranslate2f( -x, -y );
}

void RenderGUI( fWindow* window ) {
    fWidget* w = window;
    
    RenderWidget( w, w->x, w->y );
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
        
        glScale2f( 1. / w->width, 1. / w->height );
        glTranslate2f( -1, -1 );
		RenderGUI(w);
        f_signal_emit_full( w, "render2D", w );
        glTranslate2f( 1, 1 );
        glScale2f( w->width, w->height );
		window_draw(w);
        
		wait(1);
	}
}

#endif